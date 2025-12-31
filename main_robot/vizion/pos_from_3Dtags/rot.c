#include "rot.h"

#define INTOLERANCE 0.0001
#define STEP 0.003

node n0; // node const 0
node n1; // node const 1
node m1; // node const -1

node pitch;
node roll; 
node yaw;

node** th_vect_array = NULL;
node** obs_vect_array = NULL;

void fill_th_vec_node(void){
  th_vect_array = malloc(nb_vect * sizeof(node*));
  point_list* th_list_copy = th_list;
  for (int i = 0; i < nb_vect; i++){
    th_vect_array[i] = malloc(3 * sizeof(node));
    th_vect_array[i][0] = make_const(th_list_copy-> x);
    th_vect_array[i][1] = make_const(th_list_copy-> y);
    th_vect_array[i][2] = make_const(th_list_copy-> z);
    th_list_copy = th_list_copy->next;
  }
}

void fill_obs_vec_node(void){
  int len=0;
  for(point_list* l = obs_list; l; l = l->next) len++;
  if (len != nb_vect) perror("wrong obs vect nb\n");
  obs_vect_array = malloc(len * sizeof(node*));
  point_list* obs_list_copy = obs_list;
  for (int i = 0; i < len; i++){
    obs_vect_array[i] = malloc(3 * sizeof(node));
    obs_vect_array[i][0] = make_const(obs_list_copy-> x);
    obs_vect_array[i][1] = make_const(obs_list_copy-> y);
    obs_vect_array[i][2] = make_const(obs_list_copy-> z);
    obs_list_copy = obs_list_copy->next;
  }
}

// for 3x3 matrix
void matrix_prod(node* mat1[], node* mat2[], node* ret[]){
  for(int i = 0; i < 9; i++) ret[i] = &n0;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      for(int k = 0; k < 3; k++)
        ret[i*3+j] =
          add(ret[i*3+j], mult(mat1[3*i + k], mat2[3*k + j]));
}

void rot_matrix(node* ret[]){
  node* cp = cos_node(&pitch);
  node* sp = sin_node(&pitch);
  node* msp = mult(&m1, sp);
  node* pitch_mat[] = {
    cp,&n0, sp,
   &n0,&n1,&n0,
   msp,&n0, cp
  };

  node* cy = cos_node(&yaw);
  node* sy = sin_node(&yaw);
  node* msy = mult(&m1, sy);
  node* yaw_mat[] = {
    cy,msy,&n0,
    sy, cy,&n0,
   &n0,&n0,&n1
  };

  node* cr = cos_node(&roll);
  node* sr = sin_node(&roll);
  node* msr = mult(&m1, sr);
  node* roll_mat[] = {
   &n1,&n0,&n0,
   &n0, cr,msr,
   &n0, sr, cr
  };
  
  node* tmp[9];
  matrix_prod(pitch_mat, roll_mat, tmp);
  matrix_prod(yaw_mat, tmp, ret);
}

void rotate_vector(node* mat[], node vect[], node* ret[]){
  for(int i = 0; i < 3; i++){
    ret[i] = &n0;
    for(int k = 0; k < 3; k++)
      ret[i] = add(ret[i], mult(mat[3*i + k], vect + k));
  }
}

node* compute_loss(){
  node* loss = &n0;
  node* mat[9];
  rot_matrix(mat);
  for(int i = 0; i < nb_vect; i++){
    node* rotated[3];
    rotate_vector(mat, obs_vect_array[i], rotated);
    node* diff[3];
    for(int j = 0; j < 3; j++)
      diff[j] = add(th_vect_array[i] + j, mult(&m1, rotated[j]));
    loss = add(loss, sqrt_node(
      add(
        mult(diff[0], diff[0]), add(
        mult(diff[1], diff[1]), 
        mult(diff[2], diff[2]))
      )
    ));
  }

  return loss;
}

void compute_final_rot(void){
  if (!th_vect_array)
    fill_th_vec_node();
  if (!obs_vect_array)
    fill_obs_vec_node();

  n0 = make_const(0.);
  n1 = make_const(1.);
  m1 = make_const(-1.);

  pitch = make_parameter(0.1);
  roll = make_parameter(0.2);
  yaw = make_parameter(0.3);

  int itt = 0;
  float loss_val;

  do {
    node* loss = compute_loss();
    pitch.grad = 0.;
    yaw.grad = 0.;
    roll.grad = 0.;
    backward(loss);

    pitch.value -= pitch.grad * STEP;
    yaw.value -= yaw.grad * STEP;
    roll.value -= roll.grad * STEP;

    loss_val = loss->value;
    itt++;

    free_node_list(temp_node_list);
    temp_node_list = NULL;

  } while (loss_val/itt > INTOLERANCE);

  final_rotation[0] = yaw.value;
  final_rotation[1] = pitch.value;
  final_rotation[2] = roll.value;
}

