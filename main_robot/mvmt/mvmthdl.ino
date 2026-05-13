#include "detector.h"
#include "esp32-hal.h"
#include "mvmthdl.h"
#include "stepper.h"
#include <math.h>

MovementHandler::MovementHandler() {
  engine_ = FastAccelStepperEngine();
  engine_.init();
  stepperL_ = Stepper(&engine_, 16, 4);
  stepperR_ = Stepper(&engine_, 5, 17);
  curDetector_ = 0;
  detectors_ = {Detector(34), Detector(36, true), Detector(35), Detector(39, true)};
  lastPing_ = 0;
}

bool MovementHandler::frontDetector(uint8_t index) {
  return index < 2;
}

int32_t MovementHandler::distToSteps(float dist) {
  float wheel_perimeter = M_PI * WHEEL_DIAMETER;
  float dstep = wheel_perimeter / STEPS_PER_ROTATION;
  float steps = dist / dstep;
  return roundf(steps);
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

void MovementHandler::moveDist(float dist) { moveSteps(distToSteps(dist)); }
void MovementHandler::rotate(float angle) {
  /* Convert the angle to a distance to be traveled by the bot. */
  float perimeter = WHEEL_DISTANCE * 2.0 * M_PI;
  float dist = perimeter / 2.0 * (angle / 360.0);
  int32_t steps = distToSteps(dist);
  rotateSteps(steps);
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
  stepperL_.processSteps();
  stepperR_.processSteps();
  cycleDetector();
  handleCollisions();
  // Serial.printf("| front: %d back: %d\r\n", frontCollision_, backCollision_);
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
