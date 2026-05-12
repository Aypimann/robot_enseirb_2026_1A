#ifndef DETECTOR_H_
#define DETECTOR_H_
#include <Arduino.h>

class Detector {
private:
  uint8_t echoPin_;
  bool isAnalog_;

public:
  static constexpr uint8_t TRIGGER_PIN = 23;
  static constexpr uint8_t SOUND_SPEED = 0.0343;
  static constexpr uint16_t LOGICAL_THRESHOLD = 2000;
  static constexpr float DISTANCE_THRESHOLD = 15.0;

  Detector(uint8_t echoPin, bool analog = false);

  /**
   * @brief Calculates the distance from the given echo.
   */
  float getDistance();
};

#endif /* DETECTOR_H_ */
