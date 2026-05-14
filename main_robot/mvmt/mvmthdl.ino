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

void MovementHandler::moveDist(float dist) {
  moves_.push_back({Move::Translation, dist});
  moveSteps(distToSteps(dist));
  posX_+=std::cosf(angle_ac)*dist;
  posX_+=std::sinf(angle_ac)*dist;
}

void MovementHandler::rotate(float angle) {
  moves_.push_back({Move::Rotation, angle});
  /* Convert the angle to a distance to be traveled by the bot. */
  float perimeter = WHEEL_DISTANCE * 2.0 * M_PI;
  float dist = perimeter / 2.0 * (angle / 360.0);
  int32_t steps = distToSteps(dist);
  rotateSteps(steps);
}

void MovementHandler::rotateTo(float angle_aim, char inverted) {
  angle_aim += 180.0f * inverted;
  float rotation = fmod(angle_aim - angle_ac, 360.0f);
  if (std::fabs(rotation) > 180)
    rotation = rotation - signe(rotation) * 360;
  rotate(rotation);
  // Serial.printf("cor:%f ac:%f\n",rotation,angle_ac);
  angle_ac += rotation;
}

int signe(float val) { return val < 0 ? -1 : 1; }
float MovementHandler::calc_rotation(float dx, float dy) {

  float delta_angle = 0;
  if ((std::fabs(dx) > MOVE_MIN_DIST) || (std::fabs(dy) > MOVE_MIN_DIST)) {
    delta_angle = std::atanf(dx / dy) * (180 / PI); // repètre tab
    if (dx < 0.0)
      delta_angle -= 180.0;
  }

  return delta_angle;
}
void MovementHandler::goTo(float x_aim, float y_aim, char sens) {
  float dx = x_aim - posX_;
  float dy = y_aim - posY_;

  rotateTo(calc_rotation(dx, dy), sens);
  moveDist(std::sqrt(dx * dx) + std::sqrt(dy * dy));
  posX_ += dx;
  posY_ += dy;
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
  cycleDetector();
  handleCollisions();
  maybeResume();
  resetCollisions();
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

void MovementHandler::cycleDetector() {
  /* Asynchronous way of updating the detectors. */
  const uint64_t now = millis();
  if (now >= lastPing_ + DETECTOR_DELAY_MS) {
    lastPing_ = now;
    // Serial.printf("Pinging %d\r\n", curDetector_);
    detectors_[curDetector_].getDistance();
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
