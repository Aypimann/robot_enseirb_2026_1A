#include "stepper.h"
#include <cmath>
#include <cstdio>

Stepper::Stepper(FastAccelStepperEngine *engine, uint8_t stepPin, uint8_t dirPin) {
  hdl_ = engine->stepperConnectToPin(stepPin);
  hdl_->setDirectionPin(dirPin);
  hdl_->setSpeedInHz(SPEED_HZ);
  hdl_->setAcceleration(STEP_ACCEL);
  end_ = 0;
  dbgcnt_ = 0;
}

/* Needed because CPP. */
Stepper::Stepper() {
  hdl_ = NULL;
  end_ = 0;
  dbgcnt_ = 0;
}

void Stepper::stop() {
  current_ = hdl_->getCurrentPosition();
  stopped_ = true;
  /* Maybe we should be careful of whether we abruptly stop the robot. */
  hdl_->stopMove();
}

void Stepper::resume() {
  if (!stopped_)
    return;
  int32_t delta = end_ - current_;
  hdl_->move(delta);
}

bool Stepper::isStopped() const {
  return stopped_;
}

void Stepper::request(int32_t steps) {
  steps_.push(steps);
  if (end_ == 0) {
    steps_.pop();
    end_ += steps;
    hdl_->move(steps);
  }
}

void Stepper::processSteps() {
  current_ = hdl_->getCurrentPosition();
  // current_ = hdl_->getCurrentPosition();
  if (current_ == end_ && !steps_.empty()) {
    // firstTime_ = false;
    const int32_t steps = steps_.front();
    steps_.pop();
    end_ += steps;
    hdl_->move(steps);
  }
}

Stepper::Direction Stepper::direction() const {
  return end_ - current_ < 0 ? Backward : Forward;
}
