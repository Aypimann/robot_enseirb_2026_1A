#include "core.h"

void free_point_list(point_list* l){
  if(!l) return;
  free_point_list(l->next);
  free(l);
}
point_list* obs_list = NULL;
point_list* th_list = NULL;

float final_rotation_matrix[9];

node n0 = make_const(0.F);
node n1 = make_const(1.F);
node m1 = make_const(-1.F);

// 3x3 matrix
void matrix_prod(node m1[], node m2[], node ret[]){
  for(int i = 0; i < 9; i++) ret[i] = n0;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      for(int k = 0; k < 3; k++)
        ret[i] = add(ret[i], mult(m1[3*i + k], m2[3*k + j]));
}

#define PI 3.14159265359
#define RND_CST 2.92583616e-09 // 2pi/maxrand

// we don't need true rand so we ca keep
// default seed

node pitch = make_parameter((float)rand() * RND_CST);
node roll = make_parameter((float)rand() * RND_CST);
node yaw = make_parameter((float)rand() * RND_CST);

void rot_matrix(node ret[]){
  node cp = cos_node(pitch);
  node sp = sin_node(pitch);
  node msp = mult(m1, sp);
  node pitch_mat[] = {
    cp, n0, sp,
    n0, n1, n0,
   msp, n0, cp
  }

  node cy = cos_node(yaw);
  node sy = sin_node(yaw);
  node msy = mult(m1, sy);
  node yaw_mat[] = {
    cy,msy, n0,
    sy, cy, n0,
    n0, n0, n1
  }

  node cr = cos_node(roll);
  node sr = sin_node(roll);
  node msr = mult(m1, sp);
  node roll_mat[] = {
    n1, n0, n0,
    n0, cr,msr,
    n0, sr, cr
  }
  
  node tmp[9];
  matrix_prod(pitch_mat, roll_mat, tmp);
  matrix_prod(yaw_mat, tmp, ret);
}

void rotate_vector(node mat[], node vect[], node ret[]){
  for(int i = 0; i < 3; i++){
    ret[i] = n0;
    for(int k = 0; k < 3; k++)
      ret[i] = add(ret[i], mult(mat[3*i + k], vect[k]));
  }
}

int nb_vect = 0;
node** th_vect_array;
node** obs_vect_array;

void fill_th_vec_node(){
  int len=0;
  for(point_list* l = th_list; l; l = l->next) len++;
  th_vect_array = malloc(len * sizeof(node*));
  point_list* th_list_copy = th_list;
  for (int i = 0; i < len; i++){
    th_vect_array[i] = malloc(sizeof(node));
    th_vect_array[i][0] = make_const(th_list_copy-> x);
    th_vect_array[i][1] = make_const(th_list_copy-> y);
    th_vect_array[i][2] = make_const(th_list_copy-> z);
    th_list_copy = th_list_copy->next;
  }
}

void fill_obs_vec_node(){
  int len=0;
  for(point_list* l = obs_list; l; l = l->next) len++;
  if (len != nb_vect) perror("wrong obs vect nb\n")
  obs_vect_array = malloc(len * sizeof(node*));
  point_list* obs_list_copy = obs_list;
  for (int i = 0; i < len; i++){
    obs_vect_array[i] = malloc(sizeof(node));
    obs_vect_array[i][0] = make_const(obs_list_copy-> x);
    obs_vect_array[i][1] = make_const(obs_list_copy-> y);
    obs_vect_array[i][2] = make_const(obs_list_copy-> z);
    obs_list_copy = obs_list_copy->next;
  }
}

node compute_loss(){
  node loss = n0;
  node mat[9];
  rot_matrix(mat);
  for(int i = 0; i < nb_vect; i++){
    node rotated[3];
    rotate_vector(mat, obs_vect_array[i], tmp);
    node diff[3];
    for(int j = 0; j < 3; j++)
      diff[j] = add(th_vect_array[i][j], mult(m1, rotated[j]));
    loss = add(loss, sqrt_node(
      add(
        mult(diff[0], diff,[0]), add(
        mult(diff[1], diff,[1]), 
        mult(diff[2], diff,[2])
      ));
    ));
  }
  return loss;
}

