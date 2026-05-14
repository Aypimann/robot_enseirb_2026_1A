#include "mvmthdl.h"
#include <Arduino.h>

MovementHandler hdl = MovementHandler();
int coter=1;
void setup() {
  Serial.begin(115200);
  pinMode(22,INPUT_PULLUP);
  coter=digitalRead(22)==0? 1:-1;
  // setup_pince();
  while (!Serial)
    ;
}

bool done = false;
void loop() {
  
  if (!done) {
    done = true;
    // hdl.startTimer();
    // for (int i = 0; i < 5; i++) {
      // hdl.rotate(90);
    // }
    // hdl.rotate(-90.0);
    hdl.goTo(5.f, 5.f);
    hdl.rotate(180.f, true);
    hdl.moveDist(10.f * std::sqrt(2.f), true);
    // hdl.goTo(-5.f, -5.f);
    // hdl.goTo(-5.f, 5.f);
    hdl.goTo(5.f, -5.f);
    hdl.goTo(0.f, 0.f);
    hdl.rotateTo(0.0);
    // 
    // hdl.moveDist(92.3);
    // hdl.rotateTo(-90*coter, false);
    // hdl.moveDist(19);
    // hdl.rotateTo(0, false);
    // hdl.moveDist(-92.325+25);
    // hdl.moveDist(10);
    // hdl.rotateTo(22*coter, false);
    // hdl.moveDist(122);
    // hdl.rotateTo(90*coter, false);
    // hdl.moveDist(50);
    // //retour
    // hdl.moveDist(-45);
    // hdl.rotateTo(23*coter, false);
    // hdl.moveDist(-120);
    // hdl.rotateTo(0, false);
    // hdl.moveDist(-40);
  }
  
  hdl.process();
}
