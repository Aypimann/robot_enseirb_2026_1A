#include <Arduino.h>
#include "mvmthdl.h"

MovementHandler hdl = MovementHandler();

void setup() {
}

bool done = false;

void loop() {
  if (!done) {
    done = true;
    hdl.moveDist(20.0);
    hdl.rotate(90.0);
    hdl.moveDist(10.0);
  }
}

