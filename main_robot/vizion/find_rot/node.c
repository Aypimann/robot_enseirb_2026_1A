#include "node.h"

int node_count = 0;

void free_pointer_list(pointer_list* l){
  if (!l) return;
  free_pointer_list(l->next);
  free(l);
}

void free_node_list(node_list* l){
  if (!l) return;
  free_node_list(l->next);
  free(l);
}

node_list* temp_node_list = NULL;

void dfs(node* n, pointer_list** ret){
  node* ops[] = {n->operand, n->operand2};
  for(int i = 0; i < 2; i++) if (ops[i] && ops[i]->op) {
    int is_operand_in_dfs = 0;
    pointer_list* dfs_cpy = *ret;
    while(dfs_cpy && !is_operand_in_dfs){
      if (dfs_cpy->n == ops[i])
        is_operand_in_dfs = 1;
      dfs_cpy = dfs_cpy->next;
    }
    if (!is_operand_in_dfs) dfs(ops[i], ret);
  }
  pointer_list* nret = malloc(sizeof(pointer_list));
  nret->n = n;
  nret->next = *ret;
  *ret = nret;
}

/*
void node_viz(node* n, int level, node* visited[]){
  //if(level > 5) return ;
  for(int i = 0; i < 300; i++) if(visited[i] == n) return ;
  int index = 0;
  while (visited[index]) index++;
  visited[index] = n;
  char prefix[50];
  for(int i = 0; i < 50; i++) prefix[i] = '.';
  prefix[level] = 0;
  printf("%sop %c value %f, grad %f @ %x\n",
    prefix, n->op, n->value, n->grad, n);
  node* ops[] = {n->operand, n->operand2};
  for(int i = 0; i < 2; i++) if (ops[i])
    node_viz(ops[i], level+1, visited);
}*/

void backward(node* loss){
  loss->grad = 1;
  pointer_list* dfs_ret = NULL;
  dfs(loss, &dfs_ret);
  pointer_list* old_dfs = dfs_ret;
  while(dfs_ret){
    node n = *((node*)dfs_ret->n);
    switch (n.op) {
      case 0: //constant
        break;
      case 'p': // parameter
        break;
      case '+':
        n.operand->grad += n.grad;
        n.operand2->grad += n.grad;
        break;
      case '*':
        n.operand->grad += n.operand2->value * n.grad;
        n.operand2->grad += n.operand->value * n.grad;
        break;
      case 'S':
        n.operand->grad += 1/(2*n.value) * n.grad;
        break;
      case 's':
        n.operand->grad += cos(n.operand->value) * n.grad;
        break;
      case 'c':
        n.operand->grad += -sin(n.operand->value) * n.grad;
        break;
      default:
        perror(
          "Error: unable to backtrack, unknown operator.");
        return;
    }
    dfs_ret = dfs_ret->next;
  }
  free_pointer_list(old_dfs);
}

node* add(node* a, node* b){
  node r;
  r.op = a->op || b->op ? '+' : 0;
  r.value = a->value + b->value;
  r.grad = 0.;
  r.operand = r.op ? a : NULL;
  r.operand2 = r.op ? b : NULL;
  node_list* tmp = malloc(sizeof(node_list));
  tmp->n = r;
  tmp->next = temp_node_list;
  temp_node_list = tmp;
  return &tmp->n;
}

node* mult(node* a, node* b){
  node r;
  r.op = a->op || b->op ? '*' : 0;
  r.value = a->value * b->value;
  r.grad = 0.;
  r.operand = r.op ? a : NULL;
  r.operand2 = r.op ? b : NULL;
  node_list* tmp = malloc(sizeof(node_list));
  tmp->n = r;
  tmp->next = temp_node_list;
  temp_node_list = tmp;
  return &tmp->n;
}

node* sqrt_node(node* n){
  node r;
  r.op = n->op ? 'S' : 0;
  r.value = sqrt(n->value);
  r.grad = 0.;
  r.operand = r.op ? n : NULL;
  r.operand2 = NULL;
  node_list* tmp = malloc(sizeof(node_list));
  tmp->n = r;
  tmp->next = temp_node_list;
  temp_node_list = tmp;
  return &tmp->n;
}

node* sin_node(node* n){
  node r;
  r.op = n->op ? 's' : 0;
  r.value = sin(n->value);
  r.grad = 0.;
  r.operand = r.op ? n : NULL;
  r.operand2 = NULL;
  node_list* tmp = malloc(sizeof(node_list));
  tmp->n = r;
  tmp->next = temp_node_list;
  temp_node_list = tmp;
  return &tmp->n;
}

node* cos_node(node* n){
  node r;
  r.op = n->op ? 'c' : 0;
  r.value = cos(n->value);
  r.grad = 0.;
  r.operand = r.op ? n : NULL;
  r.operand2 = NULL;
  node_list* tmp = malloc(sizeof(node_list));
  tmp->n = r;
  tmp->next = temp_node_list;
  temp_node_list = tmp;
  return &tmp->n;
}

node make_const(double value){
  node r = {value, 0.F, NULL, NULL, 0};
  return r;
}

node make_parameter(double value){
  node r = {value, 0.F, NULL, NULL, 'p'};
  return r;
}

