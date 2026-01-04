#include "pos.h"

void matrix_product(float m1[], float m2[], float ret[]){
  for(int i = 0; i < 9; i++){
    ret[i] = 0;
    for(int k = 0; k < 3; k++)
      ret[i] += m1[3*(i/3) + k] * m2[3*k+i%3];
  }
}

void inv_rot_matrix(float rx, float ry, float rz, float ret[]){
  float cx = cos(rx);
  float sx = sin(rx);
  float xm[] = {
    1., 0., 0.,
    0., cx, sx,
    0.,-sx, cx
  };

  float cy = cos(ry);
  float sy = sin(ry);
  float ym[] = {
    cy, 0.,-sy,
    0., 1., 0.,
    sy,-0., cy
  };

  float cz = cos(rz);
  float sz = sin(rz);
  float zm[] = {
    cz, sz, 0.,
   -sz, cz, 0.,
    0.,-0., 1.
  };
  
  float tmp[9];
  matrix_product(xm, ym, tmp);
  matrix_product(tmp, zm, ret);
}

void compute_final_pos(float th_center[], float obs_center[]){
  float inv_rot[9];
  inv_rot_matrix(final_rotation[0], final_rotation[1],
    final_rotation[2], inv_rot);

  for(int i = 0; i < 3; i++) {
    final_position[i] = th_center[i];
    for(int k = 0; k < 3; k++)
      final_position[i] -= inv_rot[3*i + k] * obs_center[k];
  }
}

