#ifndef STEPPER_H_
#define STEPPER_H_

#include <queue>
#include <FastAccelStepper.h>

/**
 * @brief A class that wraps FastAccelStepper for interruption of the movement.
 */
class Stepper {
private:
  FastAccelStepper *hdl_;
  int32_t current_, end_;
  std::queue<int32_t> steps_;

public:
  /* Defined speed and accelerations for the stepper. */
  static const uint16_t SPEED_HZ = 1000;
  static const uint16_t STEP_ACCEL = 200;

  Stepper(FastAccelStepperEngine& engine, uint8_t stepPin, uint8_t dirPin);
  void request(int32_t steps);
  void stop();
  void resume();
  /* To be called on every loop. */
  void processSteps();
};

#endif /* STEPPER_H_ */
