#ifndef STEPPER_H_
#define STEPPER_H_

#include <queue>
#include <FastAccelStepper.h>

/**
 * @brief A class that wraps FastAccelStepper for interruption of the movement.
 */
class Stepper {
public:
  FastAccelStepper *hdl_;
  int32_t current_, end_, dbgcnt_;
  std::queue<int32_t> steps_;
  bool stopped_;

public:
  /* Defined speed and accelerations for the stepper. */
  static const uint16_t SPEED_HZ = 1000;
  static const uint16_t STEP_ACCEL = 200;

  Stepper(FastAccelStepperEngine *engine, uint8_t stepPin, uint8_t dirPin);
  /* Uninitialized */
  Stepper();

  /**
   * @brief Request a number of steps to be performed by the stepper.
   * @param steps The requested steps.
   * @note This is to keep the behaviour of the stepper async.
   */
  void request(int32_t steps);

  /* See MovementHandler documentation for methods below. */
  void stop();
  void resume();
  void processSteps();
  bool isStopped() const;
};

#endif /* STEPPER_H_ */
