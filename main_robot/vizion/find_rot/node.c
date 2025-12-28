#incude "node.h"

void free_node_list(node_list* l){
  if (!l) return;
  free_node_list(l->next);
  free(l);
}

void free_node(node n){
  if(!n.op || n.op == 'p') return;
  free_node(n.operand);
  if(n.op == '+' || n.op == '*')
    free_node(n.operand + 1);
  free(n.operandters);
}

void reverse_node_list(node_list l, node_list** ret){
  node_list* tmp = malloc(sizeof(node_list));
  tmp->n = l.n;
  tmp->next = *ret;
  if(l.next) reverse(*l.next, &tmp);
  *ret = tmp;
}

void dfs(node n, node_list** ret){
  for (
    int i = (n.op && n.op != 'p') ?
      ((n.op == '+' || n.op == '*') ? 2 : 1):0;
    i; i++
  ) if (n.operand[i-1].op && n.operand[i-1].op != 'p') {
    int is_operand_i_in_dfs = 0;
    node_list* dfs_cpy = *ret;
    while(dfs_cpy && !is_operand_i_in_dfs)
      if (dfs_cpy->n == n.operand[i-1])
        is_operand_i_in_dfs = 1;
    dfs(n.operand[i-1], ret);
  }
  node_list* dfs = malloc(sizeof(node_list));
  dfs->n = n;
  dfs->next = *ret;
  *ret = dfs;
}

void backward(node* loss){
  loss->grad = 1;
  node_list* dfs_ret = NULL;
  dfs(*loss, &dfs_ret);
  node_list* rev_dfs = NULL;
  reverse_node_list(*dfs, &rev_dfs);
  free_node_list(dfs)
  while(rev_dfs){
    node n = rev_dfs->n;
    switch (n.op) {
      case 0: #constant
        break;
      case 'p': # parameter
        break;
      case '+':
        n.operand[0].grad += n.grad;
        n.operand[1].grad += n.grad;
        break;
      case '*':
        n.operand[0].grad += n.operand[1].value * n.grad;
        n.operand[1].grad += n.operand[0].value * n.grad;
        break;
      case 'S':
        n.operand[0].grad += 1/(2*n.value) * n.grad;
        break;
      case 's':
        n.operand[0].grad += cos(n.operand[0].value) * n.grad;
        break;
      case 'c':
        n.operand[0].grad += -sin(n.operand[0].value) * n.grad;
        break;
      default:
        perror(
          "Error: unable to backtrack, unknown '%c' operator.",
          n.op
        );
        return;
    }
  }
}

node add(node* a, node* b){
  node r;
  r.op = a->op || b->op ? '+' : 0;
  r.value = a->value + b->value;
  if(!r.op) return r;
  r.grad = 0.;
  r.operand = malloc(2*sizeof(node));
  r.operand = a;
  r.operand + 1 = b;
  return r;
}

node mult(node* a, node* b){
  node r;
  r.op = a->op || b->op ? '*' : 0;
  r.value = a->value * b->value;
  if(!r.op) return r;
  r.grad = 0.;
  r.operand = malloc(2*sizeof(node));
  r.operand = a;
  r.operand + 1 = b;
  return r;
}

node sqrt_node(node* n){
  node r;
  r.op = n->op ? 'S' : 0;
  r.value = sqrt(a->value);
  if(!r.op) return r;
  r.grad = 0.;
  r.operand = malloc(sizeof(node));
  r.operand = n;
  return r;
}

node sin_node(node* n){
  node r;
  r.op = n->op ? 's' : 0;
  r.value = sin(a->value);
  if(!r.op) return r;
  r.grad = 0.;
  r.operand = malloc(sizeof(node));
  r.operand = n;
  return r;
}

node cos_node(node* n){
  node r;
  r.op = n->op ? 'c' : 0;
  r.value = cos(a->value);
  if(!r.op) return r;
  r.grad = 0.;
  r.operand = malloc(sizeof(node));
  r.operand = n;
  return r;
}

