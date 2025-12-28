#ifndef _NODE_H
#define _NODE_H

#ifndef _FR_CORE_H
#include "core.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node_s {
  float value;
  float grad;
  int id;
  struct node_s* operand;
  char op;
} node;

int node_count = 0;

typedef struct node_list_s {
  node n;
  struct node_list_s* next;
} node_list;

void free_node_list(node_list* l);

void backward(node* loss);

node add(node a, node b);

node mult(node a, node b);

node sqrt_node(node n);

node sin_node(node n);

node cos_node(node n);

#endif // !_NODE_H

