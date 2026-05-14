#include "stepper.h"
#include <cmath>
#include <cstdio>
#include <vector>

Stepper::Stepper(FastAccelStepperEngine *engine, uint8_t stepPin,
                 uint8_t dirPin) {
  *this = Stepper();
  hdl_ = engine->stepperConnectToPin(stepPin);
  hdl_->setDirectionPin(dirPin);
  hdl_->setSpeedInHz(SPEED_HZ);
  hdl_->setAcceleration(STEP_ACCEL);
}

/* Needed because CPP. */
Stepper::Stepper() {
  hdl_ = NULL;
  end_ = 0;
  currentReq_ = -1;
  stopped_ = false;
  callbackNo_ = 0;
}

void Stepper::stop() {
  current_ = hdl_->getCurrentPosition();
  stopped_ = true;
  /* WARNING: Must be forceStop! */
  hdl_->forceStop();
}

void Stepper::resume() {
  if (!stopped_)
    return;
  stopped_ = false;
  const int32_t delta = end_ - current_;
  hdl_->move(delta);
}

bool Stepper::isStopped() const { return stopped_; }

Stepper::Direction Stepper::direction() const {
  return end_ - current_ < 0 ? Backward : Forward;
}

void Stepper::move(int32_t steps) { hdl_->move(steps); }

int32_t Stepper::currentRequest() const { return currentReq_; }
int32_t Stepper::position() const { return hdl_->getCurrentPosition(); }

template <typename F> void Stepper::onFinished(F &&f) {
  if (current_ == end_) {
    if (currentReq_ != -1 && callbackNo_ != currentReq_) {
      f();
      callbackNo_ = currentReq_;
    }
  }
}
