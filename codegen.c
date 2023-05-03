#include "mycc.h"

void codegen(Node* node) {
  if(node -> class == Num) {
    printf("  li a0,%d\n", node -> val);
    printf(push);
    return;
  }
  codegen(node -> l);
  codegen(node -> r);
  printf(pop, "a1");
  printf(pop, "a0");
  switch(node -> class) {
    case '+':
      printf("  add a0,a0,a1\n");
      break;
    case '-':
      printf("  sub a0,a0,a1\n");
      break;
    case '*':
      printf("  mul a0,a0,a1\n");
      break;
    case '/':
      printf("  div a0,a0,a1\n");
      break;
    case Eq:
      printf("  sub a0,a0,a1\n");
      printf("  seqz a0,a0\n");
      break;
    case Ne:
      printf("  sub a0,a0,a1\n");
      printf("  snez a0,a0\n");
      break;
    case Lt:
      printf("  slt a0,a0,a1\n");
      break;
    case Le:
      printf("  sgt a0,a0,a1\n");
      printf("  xori a0,a0,1\n");
      break;
    case Gt:
      printf("  sgt a0,a0,a1\n");
      break;
    case Ge:
      printf("  slt a0,a0,a1\n");
      printf("  xori a0,a0,1\n");
      break;
  }
  printf(push);
  return;
}