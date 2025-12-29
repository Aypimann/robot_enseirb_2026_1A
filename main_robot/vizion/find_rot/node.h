#ifndef _NODE_H
#define _NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node_s {
  double value;
  double grad;
  struct node_s* operand;
  struct node_s* operand2;
  char op;
} node;

typedef struct pointer_list_s {
  void* n;
  struct pointer_list_s* next;
} pointer_list;

typedef struct node_list_s {
  node n;
  struct node_list_s* next;
} node_list;

extern node_list* temp_node_list;

void free_pointer_list(pointer_list* l);

void free_node_list(node_list* l);

void backward(node* loss);

node* add(node* a, node* b);

node* mult(node* a, node* b);

node* sqrt_node(node* n);

node* sin_node(node* n);

node* cos_node(node* n);

node* make_const(double value);

node* make_link_node(node** n);

node make_parameter(double value);

#endif // !_NODE_H

