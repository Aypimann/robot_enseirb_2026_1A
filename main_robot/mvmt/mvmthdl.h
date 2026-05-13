#ifndef MVMTHDL_H_
#define MVMTHDL_H_

#include "stepper.h"
#include <Arduino.h>
#include <FastAccelStepper.h>
#include "detector.h"

/* TODO: Handle when asked to stop. */
class MovementHandler {
private:
  static constexpr uint16_t DETECTOR_DELAY_MS = 50;
  FastAccelStepperEngine engine_;
  Stepper stepperL_, stepperR_;
  float posX_, posY_;
  uint64_t lastPing_;
  uint8_t curDetector_;
  std::array<Detector, 4> detectors_;
  /**
   * @brief Rotate by a given number of steps.
   */
  void rotateSteps(int32_t steps);

  /* Cycle through the detectors to actualize. */
  void cycleDetector();

public:
  /* Experimental values. */
  static constexpr float WHEEL_DIAMETER = 7.32;
  static constexpr float WHEEL_DISTANCE = 18.5;
  /* Defined values. */
  static constexpr uint16_t STEPS_PER_ROTATION = 200;
  static constexpr uint16_t MICROSTEPS = 8;
  char colision_forward=0;
  char colision_backward=0;
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
   */
  void moveDist(float dist);
  /**
   * @brief Move the robot by the given number of steps.
   */
  void moveSteps(int32_t steps);

  /**
   * @brief Rotate the robot by a given angle in degrees.
   * @note This function is blocking and doesn't return while the robot is moving.
   */
  void rotate(float angle);

  /**
   * @brief Process the movements requested to the robot.
   * @note This is to be called on every loop!
   */
  void process();

  /**
   * @brief Stop the robot in its current movement.
   */
  void stop();

  /**
   * @brief Resume the robot's movement.
   * @note This can be safely called even if the robot hasn't been stopped.
   */
  void resume();

  /**
   * @return Whether the robot is stopped or not.
   */
  bool isStopped() const;

  /**
   * @brief Get the direction of the robot.
   * @return -1 if backward, 1 if forward.
   * @note This shouldn't be considered for rotation.
   */
  Stepper::Direction direction() const;

  std::array<float, 2> getPos() const;  
};

#endif /* MVMTHDLH_ */
