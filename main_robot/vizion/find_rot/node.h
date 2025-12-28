#ifndef _NODE_H
#define _NODE_H

#ifndef _STDIO_H
#include <stdio.h>
#endif
#ifndef _STDLIB_H
#include <stdlib.h>
#endif
#ifndef _MATH_H
#include <math.h>
#endif

typedef struct node_s {
  float value;
  float grad;
  struct node_s* operand;
  char op;
} node;

typedef struct node_list_s {
  node n;
  struct node_list_s* next;
} node_list;

void free_node_list(node_list* l);

void backward(node* loss);

node add(node* a, node* b);

node mult(node* a, node* b);

node sqrt_node(node* n);

node sin_node(node* n);

node cos_node(node* n);

#endif // !_NODE_H

