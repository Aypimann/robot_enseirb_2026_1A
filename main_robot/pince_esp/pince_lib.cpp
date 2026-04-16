#include "pince_lib.h"

const float default_pince_pos = 160.0;
 
float goal_pince = default_pince_pos;
int is_pince_blocked = 0;

const float mm_per_enc_tick = 0.0048;
ESP32Encoder encoder;

float get_pince_pos(){
  return encoder.getCount() * mm_per_enc_tick + default_pince_pos;
}

void setup_pince(){ 
  ledcAttach(MOT_A_PINCE, PWM_FREQ_PINCE, PWM_RES_PINCE);
  ledcAttach(MOT_B_PINCE, PWM_FREQ_PINCE, PWM_RES_PINCE);

  encoder.attachHalfQuad(ENC_CLK_PINCE, ENC_DT_PINCE);
  encoder.setCount(0);
}


int should_pince_move = 0;
float last_pos = default_pince_pos;

int loop_pince() {
  // return if something blocked the pince

  float pos_pince = get_pince_pos();
  float dist_goal = goal_pince - pos_pince;
  Serial.println("\n\n\n\nnew loop !");
  Serial.println(dist_goal);
  // Goal reached :
  if (-1.0 <= dist_goal && dist_goal <= 1.0){
    should_pince_move = 0;
    ledcWrite(MOT_A_PINCE, 0);
    ledcWrite(MOT_B_PINCE, 0);
    return 0;
  }

  // TODO : handle carefully the case where something blocks the pince
  if (0) {//should_pince_move > 5 && last_pos == get_pince_pos()){
    should_pince_move = 0;
    goal_pince = pos_pince;
    ledcWrite(MOT_A_PINCE, 0);
    ledcWrite(MOT_B_PINCE, 0);
    return 1;
  }
  last_pos = get_pince_pos();
  should_pince_move ++;

  // activate mot_pin rotates the motor the right way
  int mot_pin = MOT_A_PINCE;
  int other_mot = MOT_B_PINCE;
  // if we have to go backward, we swap pins
  if (dist_goal < 0) {
    dist_goal = - dist_goal;
    mot_pin ^= other_mot;
    other_mot ^= mot_pin;
    mot_pin ^= other_mot;
  }

  ledcWrite(other_mot, 0);
  if (dist_goal < 10.0){
    Serial.println((int) (256 * dist_goal));
    ledcWrite(mot_pin, (int) (256 * dist_goal));}
  else 
    ledcWrite(mot_pin, 255);
  return 2;
}
