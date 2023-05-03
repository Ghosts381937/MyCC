#include "mycc.h"

void printAST(Node* node) {
  if(!node) {
    return;
  }
  // current node
  printf("node: %p\n", node);
  printf("node -> type: %d\n", node -> class);
  printf("node -> l: %p\n", node -> l);
  printf("node -> r: %p\n", node -> r);
  printf("node -> val: %d\n", node -> val);

  // left
  printAST(node -> l);
  // right
  printAST(node -> r);

  return;
}

Node* newNode(int class, Node* l, Node *r, int val) {
  Node* node = (Node*)calloc(1, sizeof(Node));
  node -> class = class;
  node -> l = l;
  node -> r = r;
  node -> val = val;
  return node;
}

Node* expr() {
  return equality();
}
Node* equality() {
  Node* node = relational();
  for(;;) {
    if(token -> class == Eq) {
      match(Eq);
      node = newNode(Eq, node, relational(), 0);
    }
    else if(token -> class == Ne) {
      match(Ne);
      node = newNode(Ne, node, relational(), 0);
    }
    else {
      return node;
    }
  }
}
Node* relational() {
  Node* node = add();
  for(;;) {
    if(token -> class == Lt) {
      match(Lt);
      node = newNode(Lt, node, add(), 0);
    }
    else if(token -> class == Le) {
      match(Le);
      node = newNode(Le, node, add(), 0);
    }
    else if(token -> class == Gt) {
      match(Gt);
      node = newNode(Gt, node, add(), 0);
    }
    else if(token -> class == Ge) {
      match(Ge);
      node = newNode(Ge, node, add(), 0);
    }
    else {
      return node;
    }
  }
}
Node* add() {
  Node* node = mul();
  for(;;) {
    if(token->val == '+') {
      match('+');
      node = newNode('+', node, mul(), 0);
    }
    else if(token->val == '-') {
      match('-');
      node = newNode('-', node, mul(), 0);
    }
    else {
      return node;
    }
  }
}
Node* mul() {
  Node* node = unary();
  for(;;) {
    if(token->val == '*') {
      match('*');
      node = newNode('*', node, unary(), 0);
    }
    else if(token->val == '/') {
      match('/');
      node = newNode('/', node, unary(), 0);
    }
    else {
      return node;
    }
  }
}
Node* unary() {
  int sign = 1;
  if(token -> val == '+') {
    sign = 1;
    match('+');
  }
  else if(token -> val == '-') {
    sign = -1;
    match('-');
  }
  Node* node = term();
  node -> val = sign * node -> val;
  return node;
}
Node* term() {
  Node* node;
  if(token -> val == '(') {
    match('(');
    node = expr();
    match(')');
  }
  else if(token -> class == Num) {
    node = newNode(Num, NULL, NULL, token -> val);
    match(Num);
  }
  return node;
}