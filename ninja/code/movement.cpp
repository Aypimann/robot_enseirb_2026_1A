#include "movement.h"

int l_last_mes_enc_count, r_last_mes_enc_count;
int goal;
char mode;
int dist_goal;

const float twice_encoder_per_mm = 1;
const float twice_encoder_per_degree = 1;

inline void mes_pos() {
  l_last_mes_enc_count = encoder_l.getCount();
  r_last_mes_enc_count = encoder_r.getCount();
}

float compute_motor_power() {
  if(dist_goal > 40) return 1.;
  else if (dist_goal < -40) return -1;
  else return dist_goal / 40.;
}

void movement_hand(){
  char debug[50];
  sprintf(debug, "mode : %c\n", mode);
  Serial.println(debug);
  int l_cur_enc_count = encoder_l.getCount();
  int r_cur_enc_count = encoder_r.getCount();

  int diff = r_cur_enc_count - r_last_mes_enc_count
    - l_cur_enc_count + l_last_mes_enc_count;
  int comm = r_cur_enc_count - r_last_mes_enc_count
    + l_cur_enc_count - l_last_mes_enc_count;

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
}

void backforward(float dist) {
  mes_pos();
  mode = 'f';
  goal = dist * twice_encoder_per_mm;
}

void rotate(float degrees) {
  mes_pos();
  mode = 'r';
  goal = degrees * twice_encoder_per_degree;
}

void crab(int x) {
  mode = 'c';
}
