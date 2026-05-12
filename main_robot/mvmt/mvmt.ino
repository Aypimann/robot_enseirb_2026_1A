#include "detector.h"
#include "mvmthdl.h"
#include <Arduino.h>

MovementHandler hdl = MovementHandler();

Detector detectors[4] = {Detector(35), Detector(34), Detector(39, true),
                         Detector(36, true)};
float distances[4];
void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
}

bool done = false;

void loop() {
  if (!done) {
    done = true;
    hdl.moveDist(5.0);
    hdl.rotate(90.0);
    hdl.moveDist(7.0);
  }

  for (int i = 0; i < 4; i++) {
    detectors[i].getDistance();
    if (detectors[i].hasCollision())
      hdl.stop();
    else
      hdl.resume();
  }

  hdl.process();
}
