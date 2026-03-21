#include "pince_lib.h"

const float mm_per_enc_tick = 0.0048;
ESP32Encoder encoder;

float get_pince_pos(){
  return encoder.get_count() * mm_per_enc_tick + DEFAULT_PINCE_POS;
}

void setup_pince(){ 
  ledcAttach(MOT_A_PINCE, PWM_FREQ_PINCE, PWM_RES_PINCE);
  ledcAttach(MOT_B_PINCE, PWM_FREQ_PINCE, PWM_RES_PINCE);

  encoder.attachHalfQuad(ENC_CLK_PINCE, ENC_DT_PINCE);
  encoder.setCount(0);
}


int should_pince_move = 0;
float last_pos = DEFAULT_PINCE_POS;

void loop_pince() {
  float dist_goal = goal_pince - pos_pince;
  // Goal reached :
  if (goal_pince - 1.0 < pos_pince < goal_pince + 1.0){
    should_pince_move = 0;
    ledcWrite(MOT_A_PINCE, 0);
    ledcWrite(MOT_B_PINCE, 0);
    return;
  }

  // TODO : handle carefully the case where something blocks the pince
  if (should_pince_move > 5 && last_pos === get_pince_pos()){
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
  if (dist_goal < 10.0)
    ledcWrite(mot_pin, (int) (256 * dist_goal));
  else 
    ledcWrite(mot_pin, 255);
}
