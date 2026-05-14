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
    Serial.printf("%f %f %f\r\n", hdl.posX_, hdl.posY_, hdl.angle_ac);
    hdl.goTo(10.0, 10.0, 1);
    Serial.printf("%f %f %f\r\n", hdl.posX_, hdl.posY_, hdl.angle_ac);
    hdl.rotateTo(90.0, 1);
    Serial.printf("%f %f %f\r\n", hdl.posX_, hdl.posY_, hdl.angle_ac);
    hdl.goTo(20.0, 10.0, 1);
    Serial.printf("%f %f %f\r\n", hdl.posX_, hdl.posY_, hdl.angle_ac);
    hdl.goTo(0.0, 0.0, 1);
    Serial.printf("%f %f %f\r\n", hdl.posX_, hdl.posY_, hdl.angle_ac);
    hdl.rotateTo(0.0, 1);
    Serial.printf("%f %f %f\r\n", hdl.posX_, hdl.posY_, hdl.angle_ac);
  }
  
  hdl.process();
}
