#include "mvmthdl.h"
#include "stepper.h"
#include <Arduino.h>

MovementHandler hdl = MovementHandler();
int coter = 1;
void setup() {
  Serial.begin(115200);
  pinMode(22, INPUT_PULLUP);
  coter = digitalRead(22) == 0 ? 1 : -1;
  // setup_pince();
  while (!Serial)
    ;
}

struct Coo {
  float x;
  float y;
  Stepper::Direction dir;
};

bool done = false;
using Stepper::Forward, Stepper::Backward;
void loop() {
  Coo cos[] = {{0.0, 92.9, Forward},
               {-17.5, 92.9, Forward},
               {-17.5, 25.1, Backward},
               {-17.5, 46.4, Forward},
               {20.9253, 145.97, Forward},
               {91.206, 145.97, Backward},
               {77.0603, 145.97, Forward},
               {17.4963, 93.9283, Forward},
               {92.276, 93.9283, Backward},
               {-4.0, 93.9, Forward},
               {-4.0, -4.0, Backward}};

  if (!done) {
    done = true;
    for (int i = 0; i < 11; i++) {
      hdl.goTo(cos[i].x * coter, cos[i].y, cos[i].dir);
    }
  }

  hdl.process();
}
