#ifndef _FR_CORE_H
#define _FR_CORE_H

#include <stdlib.h>

#ifndef _NODE_H
#include "node.h"
#endif

typedef struct point_list_s {
  float x, y, z;
  struct point_list_s* next;
} point_list;

void free_point_list(point_list* l);

extern point_list* obs_list;
extern point_list* th_list;

extern float final_rotation_matrix[9];
extern int nb_vect;

extern node*** th_vect_array;
extern node*** obs_vect_array;

void train();

#endif // !_FR_CORE_H

