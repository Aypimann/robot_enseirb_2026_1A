#include "ESP32Encoder.h" // https://github.com/madhephaestus/ESP32Encoder.git 
 
#define _ENC_CLK_PINCE 16 // CLK ENCODER
#define _ENC_DT_PINCE  4  // DT ENCODER 
#define _MOT_A_PINCE   

#define _DEFAULT_PINCE_POS 160.0
 
float pos_pince = _DEFAULT_PINCE_POS;
float goal_pince = _DEFAULT_PINCE_POS;

const float mm_per_enc_tick = 0.0048;
ESP32Encoder encoder;

void setup () { 
  encoder.attachHalfQuad(_ENC_CLK_PINCE, _ENC_DT_PINCE);
  encoder.setCount(0);
}

int loop_pince() {
  // return if pince blocked
  float dist_goal = goal_pince - pos_pince;
  // TODO :
  if (goal_pince - 1.0f < pos_pince < goal_pince + 1.0){
    return 0;
  }
  int pince_cant_move = 0;
  if (pince_cant_move){
    goal_pince = pos_pince;
    return 1;
  }
  int dir = 1;
  if (dist_goal < 0) {
    dist_goal = - dist_goal;
    dir = -1;
  }
}
 
void loop () {
}
lolcat: ../../test_esp/test_servo/: Is a directory
// based on https://www.upesy.fr/blogs/tutorials/how-to-use-pwm-on-esp32-with-examples

#define SERVO_DATA 4

#define SERVO_PWM_FREQ 50
#define SERVO_PWM_RES 8

#define SERVO_NETRAL_POS 13
#define SERVO_LEFT_POS 1
#define SERVO_RIGHT_POS 26


#define DELAY 2000

void setup(){
  pinMode(SERVO_DATA, OUTPUT);

  ledcAttach(SERVO_DATA, SERVO_PWM_FREQ, SERVO_PWM_RES);

  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("ESP32 Echo Ready!");
}

void loop() {
  for(int i = 0; 1; i++){
    ledcWrite(SERVO_DATA, i);
    Serial.println(i);
    delay(DELAY);
  }
}
