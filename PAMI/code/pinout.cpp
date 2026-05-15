#include "pinout.h"

#define DETECTOR_TRIG 26
#define DETECTOR_ECHO 25

#define ENABLE_L 23
#define ENABLE_R 5

#define MOTOR_L_A 22
#define MOTOR_L_B 21
#define MOTOR_R_A 19
#define MOTOR_R_B 18

#define ENCODER_L_A 16
#define ENCODER_L_B 17
#define ENCODER_R_A 15
#define ENCODER_R_B 4

#define SERVO_L 13
#define SERVO_R 14

#define PWM_FREQ 50
#define PWM_RES 8
#define PWM_MAX_VALUE 255

ESP32Encoder encoder_l;
ESP32Encoder encoder_r;

Detector detector;

void setup_pins() { 
  Serial.println("waiting");

  // detector
  detector = Detector(DETECTOR_ECHO, DETECTOR_TRIG);

  //ignore enable pins
  pinMode(ENABLE_L, OUTPUT);
  pinMode(ENABLE_R, OUTPUT);
  digitalWrite(ENABLE_L, HIGH);
  digitalWrite(ENABLE_R, HIGH);

  //encoder
  Serial.println("encoder");
  encoder_l.attachHalfQuad(ENCODER_L_A, ENCODER_L_B);
  encoder_l.setCount(0);
  encoder_r.attachHalfQuad(ENCODER_R_A, ENCODER_R_B);
  encoder_r.setCount(0);

  //motor
  Serial.println("motor");
  pinMode(MOTOR_L_A, OUTPUT);
  pinMode(MOTOR_L_B, OUTPUT);
  pinMode(MOTOR_R_A, OUTPUT);
  pinMode(MOTOR_R_B, OUTPUT);
  ledcAttach(MOTOR_L_A, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_L_B, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_R_A, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_R_B, PWM_FREQ, PWM_RES);

  //servo
  Serial.println("servo");
  pinMode(SERVO_L, OUTPUT);
  pinMode(SERVO_R, OUTPUT);
  ledcAttach(SERVO_L, PWM_FREQ, PWM_RES);
  ledcAttach(SERVO_R, PWM_FREQ, PWM_RES);
}

void set_l_motor(float power){
  if(power > 0) {
    ledcWrite(MOTOR_L_B, 0);
    Serial.println("lb 0, la 1");
    Serial.println(power * PWM_MAX_VALUE);
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

void set_l_servo(int pos){
    ledcWrite(SERVO_L, pos);
}

void set_r_servo(int pos){
    ledcWrite(SERVO_R, pos);
}
