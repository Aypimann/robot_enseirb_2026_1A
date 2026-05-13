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
    // hdl.moveSteps(200);
    hdl.moveDist(30.0);
    hdl.moveDist(-30.0);
    // hdl.rotate(90.0);
    // hdl.moveDist(7.0);
  }

  hdl.process();
}
