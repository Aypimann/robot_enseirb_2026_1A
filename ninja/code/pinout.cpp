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
  
  
  pinMode(STRAT_BUTTON, INPUT_PULLUP);

  //servo ledc...
}

void set_l_motor(float power){

}

void set_r_motor(float power){

}

char get_team() {
  return digitalRead(STRAT_BUTTON);
}

void set_crab_servo(int pos){

}

void set_grab_servo(int pos){

}
