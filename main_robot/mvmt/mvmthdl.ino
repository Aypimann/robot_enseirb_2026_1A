#include "esp32-hal.h"
#include "mvmthdl.h"
#include "stepper.h"
#include <math.h>

MovementHandler::MovementHandler() {
  engine_ = FastAccelStepperEngine();
  engine_.init();
  stepperL_ = Stepper(&engine_, 16, 4);
  stepperR_ = Stepper(&engine_, 5, 17);
  curDetector_ = 0;
  detectors_ = {{Detector(35, false), Detector(34, true),
                 Detector(39, false, true), Detector(36, true, true)}};
}

int32_t MovementHandler::distToSteps(float dist) {
  float wheel_perimeter = M_PI * WHEEL_DIAMETER;
  float dstep = wheel_perimeter / STEPS_PER_ROTATION;
  float steps = dist / dstep;
  return roundf(steps);
}

void MovementHandler::moveSteps(int32_t steps) {
  steps *= MICROSTEPS;
  stepperL_.request(-steps);
  stepperR_.request(steps);
}

void MovementHandler::rotateSteps(int32_t steps) {
  steps *= MICROSTEPS;
  stepperL_.request(steps);
  stepperR_.request(steps);
}

void MovementHandler::moveDist(float dist) { moveSteps(distToSteps(dist)); }
void MovementHandler::rotate(float angle) {
  /* Convert the angle to a distance to be traveled by the bot. */
  float perimeter = WHEEL_DISTANCE * 2.0 * M_PI;
  float dist = perimeter / 2.0 * (angle / 360.0);
  int32_t steps = distToSteps(dist);
  rotateSteps(steps);
}
void  MovementHandler::set_angle(float angle_aim,char inverted){
  angle_aim+=180.0f*(1-inverted);
  float rotation=fmod(angle_aim-angle_ac,360.0f);
  if(abs(rotation)>180)
    rotation=rotation-signe(rotation)*360;
  rotate(rotation);
  Serial.printf("cor:%f ac:%f\n",rotation,angle_ac);
  angle_ac+=rotation;
}

int signe(float val){
  return val<0 ? -1:1;
}
float MovementHandler::calc_rotation(float dx,float dy){
  
  float delta_angle=0;
  if((abs(dx)>MOVE_MIN_DIST)||(abs(dy)>MOVE_MIN_DIST)){
    delta_angle=atan(dy/dx)*(180/PI);//repètre tab
    if(dx<0)
      delta_angle= delta_angle-180;
  }
  
  return delta_angle;
}
void MovementHandler::go_to(float x_aim,float y_aim,char sens) {
  float dx=x_aim-posX_;
  float dy=y_aim-posY_;
  
  set_angle(calc_rotation(dx,dy),sens);
  moveDist(sqrt(pow(dx,2)+pow(dy,2)));
}
void MovementHandler::stop() {
  stepperL_.stop();
  stepperR_.stop();
}

void MovementHandler::resume() {
  stepperL_.resume();
  stepperR_.resume();
}

void MovementHandler::process() {
  stepperL_.processSteps();
  stepperR_.processSteps();
  cycleDetector();
}

bool MovementHandler::isStopped() const {
  return stepperL_.isStopped() && stepperR_.isStopped();
}

using Direction = Stepper::Direction;
Stepper::Direction MovementHandler::direction() const {
  if (stepperL_.direction() == Direction::Forward &&
      stepperR_.direction() == Direction::Backward) {
    return Direction::Backward;
  } else
    return Direction::Forward;
}

std::array<float, 2> MovementHandler::getPos() const {
  return std::array<float, 2>{posX_, posY_};
}

void MovementHandler::cycleDetector() {
  /* Asynchronous way of updating the detectors. */
  const uint64_t now = millis();
  if (now >= lastPing_ + DETECTOR_DELAY_MS) {
    lastPing_ = now;
    detectors_[curDetector_].getDistance();
    curDetector_ = (curDetector_ + 1) % detectors_.size();
  }
}
