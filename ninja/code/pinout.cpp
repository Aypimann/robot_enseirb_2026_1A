#include "pinout.h"

#define ENCODER_L_A 33
#define ENCODER_L_A 33
#define ENCODER_R_A 35
#define ENCODER_R_B 34

#define MOTOR_L_A 13
#define MOTOR_L_B 12
#define MOTOR_R_A 14
#define MOTOR_R_B 25

#define STRAT_BUTTON 21

#define CRAB_SERVO 26
#define GRAB_SERVO 2

#define PWM_FREQ 50
#define PWM_RES 16
#define PWM_MAX_VALUE 65535

void setup() { 
  sleep(5);

  //encoder
  encoder_l.attachHalfQuad (ENCODER_L_A, ENCODER_L_B);
  encoder_l.setCounT(0);
  encoder_r.attachHalfQuad (ENCODER_R_A, ENCODER_R_B);
  encoder_r.setCounT(0);

  //motor
  pinMode(MOTOR_L_A, OUTPUT);
  pinMode(MOTOR_L_B, OUTPUT);
  pinMode(MOTOR_R_A, OUTPUT);
  pinMode(MOTOR_R_B, OUTPUT);
  ledcAttach(MOTOR_L_A, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_L_B, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_R_A, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_R_B, PWM_FREQ, PWM_RES);
  
  //strat button
  pinMode(STRAT_BUTTON, INPUT_PULLUP);

  //servo
  pinMode(CRAB_SERVO, OUTPUT);
  pinMode(GRAB_SERVO, OUTPUT);
  ledcAttach(CRAB_SERVO, PWM_FREQ, PWM_RES);
  ledcAttach(GRAB_SERVO, PWM_FREQ, PWM_RES);
}

void set_l_motor(float power){
  if(power > 0) {
    ledcWrite(MOTOR_L_B, 0);
    ledcWrite(MOTOR_L_A, power * PWM_MAX_VALUE);
  } else {
    ledcWrite(MOTOR_L_A, 0);
    ledcWrite(MOTOR_L_B, (-power) * PWM_MAX_VALUE);
  }
}

void set_r_motor(float power){
  if(power > 0) {
    ledcWrite(MOTOR_R_B, 0);
    ledcWrite(MOTOR_R_A, power * PWM_MAX_VALUE);
  } else {
    ledcWrite(MOTOR_R_A, 0);
    ledcWrite(MOTOR_R_B, (-power) * PWM_MAX_VALUE);
  }
}

char get_team() {
  return digitalRead(STRAT_BUTTON);
}

inline void set_crab_servo(int pos){
    ledcWrite(CRAB_SERVO, pos);
}

inline void set_grab_servo(int pos){
    ledcWrite(GRAB_SERVO, pos);
}
