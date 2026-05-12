#include "detector.h"

Detector::Detector(uint8_t echoPin, bool analog) {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(echoPin, INPUT);
  echoPin_ = echoPin;
  isAnalog_ = analog;
}

/* At least 50 ms between the requests of each detector. */
float Detector::getDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
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
  float distance = (duration * FACTOR);
  return distance;
}
