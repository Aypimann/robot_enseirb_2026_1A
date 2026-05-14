#ifndef STEPPER_H_
#define STEPPER_H_

#include <FastAccelStepper.h>
#include <vector>

/**
 * @brief A class that wraps FastAccelStepper for interruption of the movement.
 */
class Stepper {
public:
  /* Defined enumeration for clarity. */
  enum Direction { Forward = 1, Backward = -1 };

private:
  FastAccelStepper *hdl_;
  int32_t current_, end_;
  bool stopped_;
  /* A simple counter to avoid callling the lambda in processSteps multiple
   * times. */
  uint32_t callbackNo_;
  int32_t currentReq_;

public:
  /* Defined speed and accelerations for the stepper. */
  static const uint16_t SPEED_HZ = 1000;
  static const uint16_t STEP_ACCEL = 2000;

  Stepper(FastAccelStepperEngine *engine, uint8_t stepPin, uint8_t dirPin);
  /* Uninitialized */
  Stepper();

  /**
   * @brief Gets the direction of the currently requested movement.
   * @return Either forward or backward depending on the direction.
   */
  Direction direction() const;

  /* See MovementHandler documentation for methods below. */
  void stop();
  void resume();
  bool isStopped() const;
  void move(int32_t steps);

  template <typename F> void onFinished(F &&f);

  /* Return -1 if no request has been processed. It's the ID of the request. */
  int32_t currentRequest() const;

  /* Position of the inner fast accel stepper. */
  int32_t position() const;
};

#endif /* STEPPER_H_ */
