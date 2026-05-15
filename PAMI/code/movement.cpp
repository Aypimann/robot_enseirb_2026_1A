#include "movement.h"

float l_last_mes_pos, r_last_mes_pos;
float goal;
char mode;
float dist_goal;

const float l_mm_per_enc = 0.03;
const float r_mm_per_enc = 0.0173;

void mes_pos() {
  l_last_mes_pos = encoder_l.getCount() * l_mm_per_enc;
  r_last_mes_pos = encoder_r.getCount() * r_mm_per_enc;
}

float compute_motor_power() {
  if(dist_goal > 2) return .25;
  else if (dist_goal < -2) return -.25;
  else return dist_goal / 8.;
}

void movement_hand(){
  char debug[50];
  sprintf(debug, "mode : %c\n", mode);
  Serial.println(debug);
  Serial.println(detector.getDistance());
  if (0 && detector.hasCollision()) {
    set_l_motor(0);
    set_r_motor(0);
    return;
  }
  float l_cur_pos = encoder_l.getCount() * l_mm_per_enc;
  float r_cur_pos = encoder_r.getCount() * r_mm_per_enc;

  float diff = r_cur_pos - r_last_mes_pos
    - l_cur_pos + l_last_mes_pos;
  float comm = r_cur_pos - r_last_mes_pos
    + l_cur_pos - l_last_mes_pos;

  sprintf(debug, "diff : %f, comm: %f\n", diff, comm);
  Serial.println(debug);

  if (mode == 'f'){
    dist_goal = goal - comm;
    float power = compute_motor_power();
    if (-10 < diff && diff < 10) {
      set_l_motor(power);
      set_r_motor(power);
      return;
    }
    if (diff < 0) {
      set_l_motor(power);
      set_r_motor(0);
      return;
    }
    set_l_motor(0);
    set_r_motor(power);
    return;
  }
  /*
  if (mode == 'r'){
    dist_goal = goal - diff;
    float power = compute_motor_power();
    if (-10 < comm && comm < 10) {
      set_l_motor(-power);
      set_r_motor(power);
    } else if (comm < 0) {
      set_l_motor(-power);
      set_r_motor(0);
    } else {
      set_l_motor(0);
      set_r_motor(power);
    }
  }
  if (mode == 'c') {
    // not implemented
  }
  */
}

void backforward(float dist) {
  mes_pos();
  mode = 'f';
  goal = dist * 2;
}
