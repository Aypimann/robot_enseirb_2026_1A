#ifndef _FR_CORE_H
#define _FR_CORE_H

#include "pos.h"
#include "rot.h"

typedef struct point_list_s {
  float x, y, z;
  struct point_list_s* next;
} point_list;

void free_point_list(point_list* l);

extern point_list* obs_list;
extern point_list* th_list;

extern float final_rotation[3];
extern float final_position[3];
extern int nb_vect;

void compute(void);

#endif // !_FR_CORE_H

