#include "stepper.h"

Stepper::Stepper(FastAccelStepperEngine& engine, uint8_t stepPin, uint8_t dirPin) {
  hdl_ = engine.stepperConnectToPin(stepPin);
  hdl_->setDirectionPin(dirPin);
  hdl_->setSpeedInHz(SPEED_HZ);
  hdl_->setAcceleration(STEP_ACCEL);
}

void Stepper::stop() {
  current_ = hdl_->getCurrentPosition();
  /* Maybe we should be careful of whether we abruptly stop the robot. */
  hdl_->stopMove();
}

void Stepper::resume() {
  int32_t delta = end_ - current_;
  hdl_->move(delta);
}

void Stepper::request(int32_t steps) {
  steps_.push(steps);
}

void Stepper::processSteps() {
  current_ = hdl_->getCurrentPosition();  
  if (current_ == end_ && !steps_.empty()) {
    const int32_t steps = steps_.front();
    steps_.pop();
    end_ += steps;
    hdl_->move(steps);
  }
}
