#ifndef MVMTHDL_H_
#define MVMTHDL_H_

#include <Arduino.h>
#include <FastAccelStepper.h>

/* TODO: Handle when asked to stop. */
class MovementHandler {
private:
  FastAccelStepperEngine engine_;
  FastAccelStepper *stepperL_, *stepperR_;

  static void setStepper(FastAccelStepper *s, uint8_t dirPin);

  /**
   * @brief Rotate by a given number of steps.
   */
  void rotateSteps(int32_t steps);

public:
  /* Experimental values. */
  static constexpr float WHEEL_DIAMETER = 7.13;
  static constexpr float WHEEL_DISTANCE = 19.5;
  /* Defined values. */
  static constexpr uint16_t STEPS_PER_ROTATION = 200;
  static constexpr uint16_t MICROSTEPS = 8;
  MovementHandler();

  /**
   * @brief Calculate the number of steps for a distance.
   * @param dist The distance in centimeters to travel.
   * @return The number of steps the wheel must do.
   */
  static int32_t distToSteps(float dist);

  /* I don't like method override :) */
  /**
   * @brief Move the robot by the given distance in centimeters.
   * @note This function is blocking and doesn't return while the robot is moving.
   */
  void moveDist(float dist);
  /**
   * @brief Move the robot by the given number of steps.
   * @note This function is blocking and doesn't return while the robot is moving.
   */
  void moveSteps(int32_t steps);

  /**
   * @brief Rotate the robot by a given angle in degrees.
   * @note This function is blocking and doesn't return while the robot is moving.
   */
  void rotate(float angle);
};

#endif /* MVMTHDLH_ */
