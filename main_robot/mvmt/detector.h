#ifndef DETECTOR_H_
#define DETECTOR_H_
#include <Arduino.h>

class Detector {
private:
  uint8_t echoPin_;
  bool isAnalog_, isFront_;
  float distance_;

public:
  static constexpr uint8_t TRIGGER_PIN = 23;
  static constexpr float SOUND_SPEED = 0.0343;
  static constexpr uint16_t LOGICAL_THRESHOLD = 2000;
  /* The minimum distance before we consider it a collision. */
  static constexpr float DISTANCE_THRESHOLD = 20.0;

  Detector(uint8_t echoPin, bool isFront, bool analog = false);
  /* Default impl because CPP. */
  Detector() = default;

  /**
   * @brief Calculates the distance from the given echo.
   * @return The newly calculated distance.
   */
  float getDistance();

  /**
   * @brief Gets the previously calculated distance.
   * @return The distance cached by the detector.
   * @note This doesn't trigger anything.
   */
  float cachedDistance() const;

  /**
   * @brief Does this detector detect a collision with another object?
   * @return Whether the above is true or not.
   */
  bool hasCollision() const;

  /**
   * @brief Is the detector on the front or the back?
   */
  bool isFront() const;
};

#endif /* DETECTOR_H_ */
