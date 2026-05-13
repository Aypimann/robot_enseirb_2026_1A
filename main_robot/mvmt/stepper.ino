#include "stepper.h"
#include <cmath>
#include <cstdio>
#include <vector>

Stepper::Stepper(FastAccelStepperEngine *engine, uint8_t stepPin,
                 uint8_t dirPin) {
  hdl_ = engine->stepperConnectToPin(stepPin);
  hdl_->setDirectionPin(dirPin);
  hdl_->setSpeedInHz(SPEED_HZ);
  hdl_->setAcceleration(STEP_ACCEL);
  end_ = 0;
  currentReq_ = -1;
}

/* Needed because CPP. */
Stepper::Stepper() {
  hdl_ = NULL;
  end_ = 0;
  currentReq_ = -1;
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
  const int32_t delta = end_ - current_;
  hdl_->move(delta);
}

bool Stepper::isStopped() const { return stopped_; }

void Stepper::request(int32_t steps) {
  Serial.printf("req steps: %d\r\n", steps);
  reqs_.push_back(steps);
}

void Stepper::processSteps() {
  current_ = hdl_->getCurrentPosition();
  if (current_ == end_ && leftInQueue() != 0) {
    currentReq_++;
    const int32_t steps = reqs_[currentReq_];
    end_ += steps;
    hdl_->move(steps);
  }
}

Stepper::Direction Stepper::direction() const {
  return end_ - current_ < 0 ? Backward : Forward;
}

uint32_t Stepper::leftInQueue() const { return reqs_.size() - currentReq_ - 1; }
int32_t Stepper::currentRequest() const { return currentReq_; }

const std::vector<int32_t> &Stepper::requests() const { return reqs_; }
