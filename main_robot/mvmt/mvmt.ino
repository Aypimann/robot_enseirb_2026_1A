#include "detector.h"
#include "mvmthdl.h"
#include <Arduino.h>

MovementHandler hdl = MovementHandler();

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
}

bool done = false;
void loop() {
  
  if (!done) {
    done = true;
    // hdl.moveDist(100.0);
    // hdl.moveDist(-100.0);
    hdl.go_to(10,10,1);
    hdl.go_to(0,0,1);
    // if(hdl.isStopped())
    //   Serial.printf("main:%f\n",hdl.angle_ac);
    //hdl.angle_set(180,1);
  }
  
  // for (int i = 0; i < 4; i++) {
  //   detectors[i].getDistance();
  //   delay(50);
  //   if (detectors[i].hasCollision()) {
  //     if (hdl.direction() == Stepper::Forward) {
  //       if (detectors[i].isFront()) {
  //         hdl.colision_forward = 1;
  //         hdl.stop();
  //       }
  //     } else {
  //       if (!detectors[i].isFront()) {
  //         hdl.colision_backward = 1;
  //         hdl.stop();
  //       }
  //     }
  //   }

  // }
  // if ((!hdl.colision_forward) && (hdl.direction() == Stepper::Forward)){
  //   hdl.resume();
  //   //Serial.println("r1");
  // }
  // if ((!hdl.colision_backward) && (hdl.direction() == Stepper::Backward)){
  //   hdl.resume();
  //   //Serial.println("r2");
  // }
  // hdl.colision_forward = 0;
  // hdl.colision_backward = 0;

  hdl.process();
}
