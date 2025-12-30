#include "core.h"

#define INTOLERANCE 0.1
#define STEP 0.03

void free_point_list(point_list* l){
  if(!l) return;
  free_point_list(l->next);
  free(l);
}
point_list* obs_list = NULL;
point_list* th_list = NULL;

float final_rotation_matrix[9];

node n0;
node n1;
node m1;

// 3x3 matrix
void matrix_prod(node* mat1[], node* mat2[], node* ret[]){
  for(int i = 0; i < 9; i++) ret[i] = &n0;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      for(int k = 0; k < 3; k++)
        ret[i*3+j] =
          add(ret[i*3+j], mult(mat1[3*i + k], mat2[3*k + j]));
}

#define PI 3.14159265359
#define RND_CST 2.92583616e-09 // 2pi/maxrand

// we don't need true rand so we ca keep
// default seed

node pitch;
node roll; 
node yaw;

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

  for(int i = 0; i < 9; i++)
    final_rotation_matrix[i] = ret[i]->value;
}

void rotate_vector(node* mat[], node vect[], node* ret[]){
  for(int i = 0; i < 3; i++){
    ret[i] = &n0;
    for(int k = 0; k < 3; k++)
      ret[i] = add(ret[i], mult(mat[3*i + k], vect + k));
  }
}

int nb_vect = 0;
node** th_vect_array = NULL;
node** obs_vect_array = NULL;

void fill_th_vec_node(){
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

void fill_obs_vec_node(){
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

void train(){
  n0 = make_const(0.);
  n1 = make_const(1.);
  m1 = make_const(-1.);
  if (!th_vect_array)
    fill_th_vec_node();
  if (!obs_vect_array)
    fill_obs_vec_node();

  pitch = make_parameter(0.);
  roll = make_parameter(0.5);
  yaw = make_parameter(0.);

  int itt = 0;
  float loss_val;

  do {
    //printf("hy1\n");
    node* loss = compute_loss();
    //printf("hy2\n");
    int len = 0;
    node_list* lcp = temp_node_list;
    while (lcp) {
      len++;
      lcp = lcp->next;
    }
    //printf("tmp len %d\n", len);
    pitch.grad = 0.;
    yaw.grad = 0.;
    roll.grad = 0.;
    backward(loss);
    //printf("hy3\n");

    /*
    printf("pitch grad : %f\n", pitch.grad);
    printf("yay grad : %f\n", yaw.grad);
    printf("roll grad : %f\n\n", roll.grad);

    node* n_cpy = loss;
    while (n_cpy) {
      printf("op %c val %f grad %f @ %x\n",
        n_cpy->op, n_cpy->value, n_cpy->grad, n_cpy);
      if (n_cpy->operand2 && n_cpy->operand2->op)
        n_cpy = n_cpy->operand2;
      else n_cpy = n_cpy->operand;
    }

    printf("roll : op %c val %f grad %f @ %x\n",
      roll.op, roll.value, roll.grad, &roll);
    node_list* ln = temp_node_list;
    while(ln){
      if(ln->n.operand == &roll)
        printf(" op %c val %f grad %f @ %x\n",
          ln->n.op, ln->n.value, ln->n.grad, &ln->n);
      ln = ln->next;
    }*/

    pitch.value -= pitch.grad * STEP;
    yaw.value -= yaw.grad * STEP;
    roll.value -= roll.grad * STEP;
    //printf("hy4\n");

    loss_val = loss->value;
    free_node_list(temp_node_list);
    //printf("hy5\n");
    temp_node_list = NULL;

    itt++;
    /*
    printf("loss : %f\n\n", loss_val);
    printf(
      "r : %f, l : %f, cond : %d\n\n",
      loss_val/itt,
      INTOLERANCE,
      loss_val/itt > INTOLERANCE
    );
    */
  } while (loss_val/itt > INTOLERANCE);
}

