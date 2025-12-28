#ifndef _FR_CORE_H
#define _FR_CORE_H

#ifndef _STDLIB_H
#inculde <stdlib.h>
#endif // !_STDLIB_H

typedef struct point_list_s {
  float x, y, z;
  struct point_list_s* next;
} point_list;

void free_point_list(point_list* l);

#endif // !_FR_CORE_H

