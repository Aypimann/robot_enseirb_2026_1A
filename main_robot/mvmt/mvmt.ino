#include "detector.h"
#include "mvmthdl.h"
#include <Arduino.h>

MovementHandler hdl = MovementHandler();
int coter=1;
void setup() {
  Serial.begin(115200);
  pinMode(22,INPUT_PULLUP);
  coter=digitalRead(22)==0? 1:-1;
  setup_pince();
  while (!Serial)
    ;
}

bool done = false;
void loop() {
  
  if (!done) {
    done = true;
    hdl.moveDist(92.3);
    hdl.rotateTo(-90*coter, false);
    hdl.moveDist(19);
    hdl.rotateTo(0, false);
    hdl.moveDist(-92.325+25);
    hdl.moveDist(10);
    hdl.rotateTo(22*coter, false);
    hdl.moveDist(122);
    hdl.rotateTo(90*coter, false);
    hdl.moveDist(50);
    //retour
    hdl.moveDist(-45);
    hdl.rotateTo(23*coter, false);
    hdl.moveDist(-120);
    hdl.rotateTo(0, false);
    hdl.moveDist(-40);
  }
  
  hdl.process();
}
