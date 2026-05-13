#include "detector.h"
#include "mvmthdl.h"
#include <Arduino.h>

MovementHandler hdl = MovementHandler();

Detector detectors[4] = {Detector(35, false), Detector(34, true),
                         Detector(39, false, true), Detector(36, true, true)};
float distances[4];
void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("began communication");
}

bool done = false;
long timer=millis();
void loop() {
  if (!done) {
    done = true;
    hdl.moveDist(20.0);
    hdl.moveDist(-20.0);
    // hdl.rotate(90.0);
    // hdl.moveDist(7.0);
  }


  for (int i = 0; i < 4; i++) {
    detectors[i].getDistance();
    delay(50);
    if (detectors[i].hasCollision()) {
      if (hdl.direction() == Stepper::Forward) {
        if (detectors[i].isFront()) {
          hdl.colision_forward = 1;
          Serial.println("s1");
          hdl.stop();
        }
      } else {
        if (!detectors[i].isFront()) {
          hdl.colision_backward = 1;
          hdl.stop();
          Serial.println("s2");
        }
      }
    }

  }
  if ((!hdl.colision_forward) && (hdl.direction() == Stepper::Forward)){
    hdl.resume();
    //Serial.println("r1");
  }
  if ((!hdl.colision_backward) && (hdl.direction() == Stepper::Backward)){
    hdl.resume();
    //Serial.println("r2");
  }
  hdl.colision_forward = 0;
  hdl.colision_backward = 0;

  hdl.process();
}
