#ifndef _FR_CORE_H
#define _FR_CORE_H

#include <stdlib.h>

typedef struct point_list_s {
  float x, y, z;
  struct point_list_s* next;
} point_list;

void free_point_list(point_list* l);

point_list* obs_list = NULL;
point_list* th_list = NULL;

float final_rotation_matrix[9];

#endif // !_FR_CORE_H

