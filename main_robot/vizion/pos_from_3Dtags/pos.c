#include "pos.h"

void matrix_product(float m1[], float m2[], float ret[]){
  for(int i = 0; i < 9; i++){
    ret[i] = 0;
    for(int k = 0; k < 3; k++)
      ret[i] += m1[3*(i/3) + k] * m2[3*k+i%3];
  }
}

void inv_rot_matrix(float yaw, float pitch, float roll, float ret[]){
  float cr = cos(roll);
  float sr = sin(roll);
  float rm[] = {
    1., 0., 0.,
    0., cr, sr,
    0.,-sr, cr
  };

  float cp = cos(pitch);
  float sp = sin(pitch);
  float pm[] = {
    cp, 0.,-sp,
    0., 1., 0.,
    sp,-0., cp
  };

  float cy = cos(yaw);
  float sy = sin(yaw);
  float ym[] = {
    cy, sy, 0.,
   -sy, cy, 0.,
    0.,-0., 1.
  };
  
  float tmp[9];
  matrix_product(rm, pm, tmp);
  matrix_product(tmp, ym, ret);
}

void compute_final_pos(float th_center[], float obs_center[]){
  float inv_rot[9];
  inv_rot_matrix(final_rotation[0], final_rotation[1], final_rotation[2],
    inv_rot);

  for(int i = 0; i < 3; i++) {
    final_position[i] = th_center[i];
    for(int k = 0; k < 3; k++)
      final_position[i] -= inv_rot[3*i + k] * obs_center[k];
  }
}
