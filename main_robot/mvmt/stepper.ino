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

void Stepper::request(int32_t steps) { reqs_.push_back(steps); }

void Stepper::processSteps() { processSteps(std::move(Stepper::noop)); }

template <typename F> void Stepper::processSteps(F &&onFinished) {
  current_ = hdl_->getCurrentPosition();
  if (current_ == end_) {
    if (currentReq_ != -1 && callbackNo_ == 0) {
      onFinished();
      callbackNo_++;
    }
    if (leftInQueue() != 0) {
      callbackNo_ = 0;
      currentReq_++;
      /* Just to make sure we're indeed running. */
      stopped_ = false;
      int32_t steps = reqs_[currentReq_];
      end_ += steps;
      hdl_->move(steps);
      stopped_ = false;
    }
  }
}

Stepper::Direction Stepper::direction() const {
  return end_ - current_ < 0 ? Backward : Forward;
}

uint32_t Stepper::leftInQueue() const { return reqs_.size() - currentReq_ - 1; }
int32_t Stepper::currentRequest() const { return currentReq_; }
const std::vector<int32_t> &Stepper::requests() const { return reqs_; }
int32_t Stepper::position() const { return hdl_->getCurrentPosition(); }
void Stepper::noop() {}
