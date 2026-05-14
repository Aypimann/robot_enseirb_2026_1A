#ifndef MVMTHDL_H_
#define MVMTHDL_H_

#include "detector.h"
#include "stepper.h"
#include <Arduino.h>
#include <FastAccelStepper.h>

class MovementHandler {
public:
  struct Move {
    /* Non-zero to avoid false positives. */
    enum { Rotation = 4, Translation } kind;
    float by;
  };

private:
  static constexpr uint16_t DETECTOR_DELAY_MS = 50;
  static constexpr uint32_t TIMEOUT_MS = 85'000;
  FastAccelStepperEngine engine_;
  Stepper stepperL_, stepperR_;
  /* Practical position. */
  float posX2_, posY2_, angle_;
  uint64_t lastPing_, startTime_ = 0;
  uint8_t curDetector_;
  std::array<Detector, 4> detectors_;
  bool frontCollision_, backCollision_;
  std::vector<Move> moves_;
  /**
   * @brief Rotate by a given number of steps.
   */
  void rotateSteps(int32_t steps);

  /* Cycle through the detectors to actualize. */
  void cycleDetector(const uint64_t now);

  /* These are listed in the order they must be called. */
  void handleCollisions();
  void maybeResume();
  void resetCollisions();

  /* Whether the given detector is on the front or back. */
  static bool frontDetector(uint8_t index);

  /* Update self's position and angle from the given request number. */
  void updatePosition(uint32_t reqNo);

  /* Stops everything when the timer reaches TIMEOUT_MS. */
  void timeOut(const uint64_t now);  

public:
  /* Experimental values. */
  static constexpr float WHEEL_DIAMETER = 7.32;
  static constexpr float WHEEL_DISTANCE = 18.5;
  static constexpr float MOVE_MIN_DIST = 0.01;
  /* Defined values. */
  static constexpr uint16_t STEPS_PER_ROTATION = 200;
  static constexpr uint16_t MICROSTEPS = 8;
  /* Theoritical one. */
  float posX_, posY_;
  float angle_ac = 0.0f;
  MovementHandler();

  /**
   * @brief Calculate the number of steps for a distance.
   * @param dist The distance in centimeters to travel.
   * @return The number of steps the wheel must do.
   */
  static int32_t distToSteps(float dist);
  /* We only stop during a movement, not a rotation, right?
   * TODO: Figure this out, this might impact on the fly position calculations.
   */
  static float stepsToDist(int32_t steps);

  /* I don't like method override :) */
  /**
   * @brief Move the robot by the given distance in centimeters.
   * @param update Whether we update the theoritical coordinates of the bot.
   */
  void moveDist(float dist, bool update = true);
  /**
   * @brief Move the robot by the given number of steps.
   */
  void moveSteps(int32_t steps);

  /**
   * @brief Rotate the robot by a given angle in degrees.
   * @param update  Whether we update the theoritical coordinates of the bot.
   */
  void rotate(float angle, bool update = true);
  void rotateTo(float angle_aim, Stepper::Direction dir = Stepper::Forward);
  static float calcRotation(float dx, float dy);
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
  void goTo(float x, float y, Stepper::Direction dir = Stepper::Forward);
  std::array<float, 2> getPos() const;

  float getAngle() const;

  /* Start the inner timer so the bot doesn't continue running even when time
   * is out.*/
  void startTimer();
};

#endif /* MVMTHDLH_ */
