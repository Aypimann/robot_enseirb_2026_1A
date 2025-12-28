#ifndef _NODE_H
#define _NODE_H

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

extern int node_count;

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

node make_parameter(float value);

node make_const(float value);

#endif // !_NODE_H

