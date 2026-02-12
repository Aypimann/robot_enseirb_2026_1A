#include "rot.h"

#define INTOLERANCE 0.0001
#define STEP 0.003

node n0; // node const 0
node n1; // node const 1
node m1; // node const -1

node ry;
node rx; 
node rz;

node** th_vect_array = NULL;
node** obs_vect_array = NULL;

void fill_th_vec_node(void){
  th_vect_array = malloc(nb_vect * sizeof(node*));
  point_list* th_list_copy = th_list;
  for (int i = 0; i < nb_vect; i++){
    printf("th x %f, y %f, z %f\n", th_list_copy->x, th_list_copy->y, th_list_copy->z);
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
    printf("obs x %f, y %f, z %f\n", obs_list_copy->x, obs_list_copy->y, obs_list_copy->z);
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
  node* cx = cos_node(&rx);
  node* sx = sin_node(&rx);
  node* msx = mult(&m1, sx);
  node* rx_mat[] = {
   &n1,&n0,&n0,
   &n0, cx,msx,
   &n0, sx, cx
  };
  
  node* cy = cos_node(&ry);
  node* sy = sin_node(&ry);
  node* msy = mult(&m1, sy);
  node* ry_mat[] = {
    cy,&n0, sy,
   &n0,&n1,&n0,
   msy,&n0, cy
  };

  node* cz = cos_node(&rz);
  node* sz = sin_node(&rz);
  node* msz = mult(&m1, sz);
  node* rz_mat[] = {
    cz,msz,&n0,
    sz, cz,&n0,
   &n0,&n0,&n1
  };

  node* tmp[9];
  matrix_prod(rz_mat, ry_mat, tmp);
  matrix_prod(tmp, rx_mat, ret);
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

  rx = make_parameter(0.2);
  ry = make_parameter(0.1);
  rz = make_parameter(0.3);

  int itt = 0;
  float loss_val;

  do {
    node* loss = compute_loss();
    rx.grad = 0.;
    ry.grad = 0.;
    rz.grad = 0.;
    backward(loss);

    rx.value -= rx.grad * STEP;
    ry.value -= ry.grad * STEP;
    rz.value -= rz.grad * STEP;

    loss_val = loss->value;
    itt++;

    free_node_list(temp_node_list);
    temp_node_list = NULL;

  } while (loss_val/itt > INTOLERANCE);

  final_rotation[0] = rx.value;
  final_rotation[1] = ry.value;
  final_rotation[2] = rz.value;
}

