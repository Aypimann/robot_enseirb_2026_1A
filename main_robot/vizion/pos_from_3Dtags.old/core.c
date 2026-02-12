#include "core.h"

float final_rotation[3];
float final_position[3];

point_list* obs_list = NULL;
point_list* th_list = NULL;
int nb_vect = 0;

void free_point_list(point_list* l){
  if(!l) return;
  free_point_list(l->next);
  free(l);
}


void compute(void){
  // centering sets
  float th_center[] = {0., 0., 0.};
  float obs_center[] = {0., 0., 0.};
  point_list* tlc = th_list;
  point_list* olc = obs_list;
  while (tlc) {
    th_center[0] += tlc->x;
    th_center[1] += tlc->y;
    th_center[2] += tlc->z;

    obs_center[0] += olc->x;
    obs_center[1] += olc->y;
    obs_center[2] += olc->z;

    tlc = tlc->next;
    olc = olc->next;
  }

  for(int j = 0; j < 3; j++){
    th_center[j] /= nb_vect;
    obs_center[j] /= nb_vect;
  }

  tlc = th_list;
  olc = obs_list;
  while (tlc) {
    tlc->x -= th_center[0];
    tlc->y -= th_center[1];
    tlc->z -= th_center[2];

    olc->x -= obs_center[0];
    olc->y -= obs_center[1];
    olc->z -= obs_center[2];

    tlc = tlc->next;
    olc = olc->next;
  }

  compute_final_rot();
  compute_final_pos(th_center, obs_center);
}
