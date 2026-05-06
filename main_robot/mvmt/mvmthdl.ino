#include <math.h>
#include "mvmthdl.h"

MovementHandler::MovementHandler(): engine_(FastAccelStepperEngine()) {
  engine_.init();
  /* TODO: Handle when those are NULL. */
  stepperL_ = engine_.stepperConnectToPin(16);
  stepperR_ = engine_.stepperConnectToPin(5);
  setStepper(stepperL_, 4);
  setStepper(stepperR_, 17);
}

void MovementHandler::setStepper(FastAccelStepper *s, uint8_t dirPin) {
  s->setDirectionPin(dirPin);
  s->setSpeedInHz(1000);
  s->setAcceleration(200);
}

int32_t MovementHandler::distToSteps(float dist) {
  float wheel_perimeter = M_PI * WHEEL_DIAMETER;
  float dstep = wheel_perimeter / STEPS_PER_ROTATION;
  float steps = dist / dstep;
  return roundf(steps);
}

template <bool block>
void MovementHandler::moveSteps(int32_t steps) {
  steps *= MICROSTEPS;
  stepperL_->move(steps);
  stepperR_->move(-steps);
  /* The while loop is used to make these blocking. */
  if constexpr (block)
    while (stepperL_->isRunning() || stepperR_->isRunning());
}

template <bool block>
void MovementHandler::rotateSteps(int32_t steps) {
  steps *= MICROSTEPS;
  stepperL_->move(steps);
  stepperR_->move(steps);
  if constexpr (block)
    while (stepperL_->isRunning() || stepperR_->isRunning());
}

template <bool block>
void MovementHandler::moveDist(float dist) {
  moveSteps<block>(distToSteps(dist));
}

template <bool block>
void MovementHandler::rotate(float angle) {
  /* Convert the angle to a distance to be traveled by the bot. */
  float perimeter = WHEEL_DISTANCE * 2.0 * M_PI;
  float dist = perimeter / 2.0 * (angle / 360.0);
  int32_t steps = distToSteps(dist);
  rotateSteps<block>(steps);
}
