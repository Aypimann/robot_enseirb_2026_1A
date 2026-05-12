#include "mvmthdl.h"
#include "stepper.h"
#include "detector.h"
#include <Arduino.h>
#include <memory>

// MovementHandler hdl = MovementHandler();

FastAccelStepperEngine engine = FastAccelStepperEngine();
std::unique_ptr<Stepper> left = std::unique_ptr<Stepper>(), right = std::unique_ptr<Stepper>();
// Stepper left = Stepper(&engine, 16, 4), right = Stepper(&engine, 5, 17);
// Detector a = Detector(35), b = Detector(34), c = Detector(39, true), d = Detector(36, true);
void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  engine.init();
  left = std::make_unique <Stepper>(&engine, 16, 4);
  right = std::make_unique<Stepper>(&engine, 5, 17);
}

bool done = false;

void loop() {
  if (!done) {
    done = true;
    int32_t steps = MovementHandler::distToSteps(05.0) * 8;
    left->request(-steps);
    right->request(steps);
    float perimeter = MovementHandler::WHEEL_DISTANCE * 2.0 * M_PI;
    float dist = perimeter / 2.0 * (90.0 / 360.0);
    steps = MovementHandler::distToSteps(dist) * 8;
    left->request(-steps);
    right->request(-steps);
    steps = MovementHandler::distToSteps(07.0) * 8;
    left->request(-steps);
    right->request(steps);
  }

  // left.hdl_->move(700);
  // right.hdl_->move(700);

  left->processSteps();
  right->processSteps();
  
  // if (!done) {
  //   done = true;
  //   hdl.moveDist(20.0);
  //   hdl.rotate(90.0);
  //   hdl.moveDist(10.0);
  // }

  /* New API ? Will be put inside MovementHandler */
  // if constexpr (false) {
  //   FastAccelStepperEngine engine = FastAccelStepperEngine();
  //   Stepper left = Stepper(engine, 16, 4), right = Stepper(engine, 5, 17);
  //   int32_t steps = MovementHandler::distToSteps(20.0);
  //   left.request(steps);
  //   right.request(-steps);

  //   left.processSteps();
  //   right.processSteps();
  // }
}
