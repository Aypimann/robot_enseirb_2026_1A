#include "mvmthdl.h"
#include "stepper.h"
#include <Arduino.h>

MovementHandler hdl = MovementHandler();

void setup() {}

bool done = false;

void loop() {
  if (!done) {
    done = true;
    hdl.moveDist<false>(20.0);
    hdl.rotate<false>(90.0);
    hdl.moveDist<false>(10.0);
  }

  /* New API ? Will be put inside MovementHandler */
  if constexpr (false) {
    FastAccelStepperEngine engine = FastAccelStepperEngine();
    Stepper left = Stepper(engine, 16, 4), right = Stepper(engine, 5, 17);
    int32_t steps = MovementHandler::distToSteps(20.0);
    left.request(steps);
    right.request(steps);

    left.processSteps();
    right.processSteps();
  }
}
