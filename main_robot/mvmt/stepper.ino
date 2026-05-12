#include "stepper.h"
#include <cstdio>

Stepper::Stepper(FastAccelStepperEngine *engine, uint8_t stepPin, uint8_t dirPin) {
  hdl_ = engine->stepperConnectToPin(stepPin);
  hdl_->setDirectionPin(dirPin);
  hdl_->setSpeedInHz(SPEED_HZ);
  hdl_->setAcceleration(STEP_ACCEL);
  firstTime_ = true;
  end_ = 0;
  dbgcnt_ = 0;
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
  char buf[32];
  std::sprintf(buf, "req: %d end: %d", steps, end_);
  Serial.println(buf);
  if (end_ == 0) {
    steps_.pop();
    end_ += steps;
    hdl_->move(steps);
  }
}

void Stepper::processSteps() {
  int tmp;
  char buf[64];
  tmp = hdl_->getCurrentPosition();
  if (current_ != tmp) {
  }
  current_ = tmp;
  // current_ = hdl_->getCurrentPosition();
  if (current_ == end_ && !steps_.empty()) {
    // firstTime_ = false;
    const int32_t steps = steps_.front();
    steps_.pop();
    std::sprintf(buf, "left in queue: %d -> %d\n", steps_.size(), steps);
    Serial.println(buf);
    end_ += steps;
    hdl_->move(steps);
  }
}
