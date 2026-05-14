#include "detector.h"
#include "esp32-hal.h"
#include "mvmthdl.h"
#include "stepper.h"
#include <cmath>

MovementHandler::MovementHandler() {
  engine_ = FastAccelStepperEngine();
  engine_.init();
  stepperL_ = Stepper(&engine_, 16, 4);
  stepperR_ = Stepper(&engine_, 5, 17);
  curDetector_ = 0;
  detectors_ = {Detector(34), Detector(36, true), Detector(35),
                Detector(39, true)};
  lastPing_ = 0;
}

bool MovementHandler::frontDetector(uint8_t index) { return index < 2; }

int32_t MovementHandler::distToSteps(float dist) {
  constexpr float wheel_perimeter = M_PI * WHEEL_DIAMETER;
  constexpr float dstep = wheel_perimeter / STEPS_PER_ROTATION;
  float steps = dist / dstep;
  return roundf(steps);
}

/* Can be used when the robot is stopped during its movement.
 * Shouldn't be used with rotation though. */
float MovementHandler::stepsToDist(int32_t steps) {
  constexpr float wheel_perimeter = M_PI * WHEEL_DIAMETER;
  constexpr float dstep = wheel_perimeter / STEPS_PER_ROTATION;
  return (float)steps * dstep;
}

void MovementHandler::moveSteps(int32_t steps) {
  steps *= MICROSTEPS;
  stepperL_.request(-steps);
  stepperR_.request(steps);
}

void MovementHandler::rotateSteps(int32_t steps) {
  steps *= MICROSTEPS;
  stepperL_.request(steps);
  stepperR_.request(steps);
}

void MovementHandler::moveDist(float dist, bool update) {
  if (update) {
    posX_ += dist * std::cosf(angle_ac * M_PI / 180.f);
    posY_ += dist * std::sinf(angle_ac * M_PI / 180.f);
  }
  moves_.push_back({Move::Translation, dist});
  moveSteps(distToSteps(dist));
}

void MovementHandler::rotate(float angle, bool update) {
  moves_.push_back({Move::Rotation, angle});
  /* Convert the angle to a distance to be traveled by the bot. */
  float perimeter = WHEEL_DISTANCE * 2.0 * M_PI;
  float dist = perimeter / 2.0 * (angle / 360.0);
  int32_t steps = distToSteps(dist);
  if (update)
    angle_ac += angle;
  rotateSteps(steps);
}

float sign(float x) { return 1 - 2 * std::signbit(x); }
void MovementHandler::rotateTo(float angle_aim, Stepper::Direction dir) {
  angle_aim = 90.f - angle_aim;
  angle_aim += 180.0f * (dir == Stepper::Backward);
  float rotation = std::fmodf(angle_aim - angle_ac, 360.f);
  if (std::fabs(rotation) > 180.0)
    rotation -= 360.f * sign(rotation);
  rotate(rotation, false);
  angle_ac += rotation;
}

float MovementHandler::calcRotation(float dx, float dy) {
  float delta_angle = 0.0;
  /* Check for negligible distances. */
  if (std::fabs(dx) > MOVE_MIN_DIST || std::fabs(dy) > MOVE_MIN_DIST) {
    delta_angle = std::atanf(dy / dx) * (180.0 / M_PI);
    if (signbit(dx))
      delta_angle -= 180.0;
  }
  return delta_angle;
}

void MovementHandler::goTo(float xAim, float yAim, Stepper::Direction dir) {
  float dx = xAim - posX_;
  float dy = yAim - posY_;

  float angle = calcRotation(dx, dy);
  rotateTo(angle, dir);
  moveDist(std::sqrt(dx * dx + dy * dy), false);
  posX_ = xAim;
  posY_ = yAim;
}
void MovementHandler::stop() {
  stepperL_.stop();
  stepperR_.stop();
}

void MovementHandler::resume() {
  stepperL_.resume();
  stepperR_.resume();
}

void MovementHandler::process() {
  /* If one stepper is finished, both are finished. We never fire only one. */
  stepperL_.processSteps();
  stepperR_.processSteps([this] {
    int32_t req = stepperR_.currentRequest();
    updatePosition(req);
  });
  const uint64_t now = millis();
  cycleDetector(now);
  handleCollisions();
  maybeResume();
  resetCollisions();
  timeOut(now);
}

bool MovementHandler::isStopped() const {
  return stepperL_.isStopped() && stepperR_.isStopped();
}

using Direction = Stepper::Direction;
Stepper::Direction MovementHandler::direction() const {
  if (stepperL_.direction() == Direction::Forward &&
      stepperR_.direction() == Direction::Backward) {
    return Direction::Backward;
  } else
    return Direction::Forward;
}

std::array<float, 2> MovementHandler::getPos() const {
  return std::array<float, 2>{posX_, posY_};
}

void MovementHandler::cycleDetector(const uint64_t now) {
  /* Asynchronous way of updating the detectors. */
  if (now >= lastPing_ + DETECTOR_DELAY_MS) {
    lastPing_ = now;
    float dbg = detectors_[curDetector_].getDistance();
    // Serial.printf("[%d] %f -> %d\r\n", curDetector_, dbg,
    //               detectors_[curDetector_].hasCollision());
    curDetector_ = (curDetector_ + 1) % detectors_.size();
  }
}

void MovementHandler::handleCollisions() {
  for (uint8_t i = 0; i < 4; i++) {
    // Serial.printf("%f ", detectors_[i].cachedDistance());
    if (!detectors_[i].hasCollision())
      continue;

    if (frontDetector(i) && direction() == Stepper::Forward) {
      frontCollision_ = true;
      stop();
    } else if (!frontDetector(i) && direction() == Stepper::Backward) {
      backCollision_ = true;
      stop();
    }
  }
}

void MovementHandler::resetCollisions() {
  /* Used to actualize them again. */
  frontCollision_ = false;
  backCollision_ = false;
}

void MovementHandler::maybeResume() {
  if (!frontCollision_ && direction() == Stepper::Forward)
    resume();
  if (!backCollision_ && direction() == Stepper::Backward)
    resume();
}

void MovementHandler::updatePosition(uint32_t reqNo) {
  Move m = moves_[reqNo];
  if (m.kind == Move::Rotation)
    angle_ += m.by;
  else if (m.kind == Move::Translation) {
    float rads = angle_ * M_PI / 180.0;
    const float dist = m.by;
    posX2_ += dist * std::cosf(rads);
    posY2_ += dist * std::sinf(rads);
  }
}

float MovementHandler::getAngle() const { return angle_; }

void MovementHandler::startTimer() {
  if (startTime_ == 0)
    startTime_ = millis();
}

void MovementHandler::timeOut(const uint64_t now) {
  if (now >= startTime_ + TIMEOUT_MS) {
    stepperL_.finishAll();
    stepperR_.finishAll();
    stepperL_.stop();
    stepperR_.stop();
    while (true)
      ;
  }
}
