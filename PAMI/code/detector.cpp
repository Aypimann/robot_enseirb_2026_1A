#include "detector.h"
#include "Arduino.h"

Detector::Detector(uint8_t echoPin, uint8_t triggerPin, bool analog) {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  echoPin_ = echoPin;
  triggerPin_ = triggerPin;
  isAnalog_ = analog;
}

/* At least 50 ms between the requests of each detector. */
float Detector::getDistance() {
  digitalWrite(triggerPin_, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin_, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin_, LOW);
  uint32_t duration = 0;

  if (isAnalog_) {
    uint32_t start = micros();
    uint32_t pStart = 0, pEnd = 0;

    while (analogRead(echoPin_) < LOGICAL_THRESHOLD)
      if (micros() - start > 20000)
        break;

    pStart = micros();
    while (analogRead(echoPin_) >= LOGICAL_THRESHOLD)
      if (micros() - start > 25000)
        break;
    pEnd = micros();
    duration = pEnd - pStart;
  } else
    duration = pulseIn(echoPin_, HIGH, 25000);

  constexpr float FACTOR = SOUND_SPEED / 2.0;
  distance_ = (duration * FACTOR);
  return distance_;
}

float Detector::cachedDistance() const { return distance_; }

bool Detector::hasCollision() const { return distance_ < DISTANCE_THRESHOLD; }
