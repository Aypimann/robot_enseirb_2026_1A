#include "core.h"

void free_point_list(point_list* l){
  if(!l) return;
  free_point_list(l->next);
  free(l);
}

