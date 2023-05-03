#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024 * 128 //128 Bytes
#define push "  sw a0,0(sp)\n  addi sp,sp,-4\n" //push: sp[0] = a0 then sp--
#define pop "  lw %s,4(sp)\n  addi sp,sp,4\n" //pop: rd(%s) = sp[4] then sp++
char* src;
char* old_src;
char* targetCode;
// tokens and classes (operators last and in precedence order)
// copied from c4
enum {
  Num = 128, Fun, Sys, Glo, Loc, Id,
  Char, Else, Enum, If, Int, Return, Sizeof, While,
  Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Mod, Inc, Dec, Brak
};



typedef struct Token Token;
typedef struct Node Node;

// the data of token
struct Token {
  int class; //class of token
  int val; //mainly for number     
};

struct Node {
  int class;
  Node *l;     // left
  Node *r;     // right
  int val;       // the value of NUM
};



// EBNF
// expr = equality
// equality = relational ("==" relational | "!=" relational)*
// relational = add (">" add | ">=" add | "<" add | "<=" add)*
// add = mul ("+" mul | "-" mul)* 
// mul  = term ("*" term | "/" term)*
// unary = ("+" | "-")? term
// term = num | "(" expr ")"
Node* expr();
Node* equality();
Node* relational();
Node* add();
Node* mul();
Node* unary();
Node* term();

Node* newNode(int class, Node* l, Node *r, int val) {
  Node* node = (Node*)calloc(1, sizeof(Node));
  node -> class = class;
  node -> l = l;
  node -> r = r;
  node -> val = val;
  return node;
}

// The token stream
Token* token;

//Debug
void printToken() {
  printf("token -> class: %d\n", token -> class);
  printf("token -> val: %d\n", token -> val);
}

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

//print the error
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

//print the error with location
void error_at(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = src - 1 - old_src;
  fprintf(stderr, "%s\n", old_src);
  fprintf(stderr, "%*s", pos, "");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

//get the next valid token from src
void next() {
  while(token -> val = *src) {
    //printToken();
    src++;
    if(isspace(token -> val)) {
      continue;
    }
    else if(isdigit(token -> val)) {
      token -> class = Num;
      token -> val = strtol(src - 1, &src, 10);
      return;
    }
    else if(token -> val == '<') {
      if(*src == '=') {
        src++;
        token -> class = Le;
      }
      else {
        token -> class = Lt;
      }
      return;
    }
    else if(token -> val == '>') {
      if(*src == '=') {
        src++;
        token -> class = Ge;
      }
      else {
        token -> class = Gt;
      }
      return;
    }
    else if(token -> val == '=') {
      if(*src == '=') {
        src++;
        token -> class = Eq;
      }
      return;
    }
    else if(token -> val == '!') {
      if(*src == '=') {
        src++;
        token -> class = Ne;
      }
      return;
    }
    else if(token -> val == '+' || token -> val == '-' || token -> val == '*' || token -> val == '/' || token -> val == '(' || token -> val == ')') {
      token -> class = token -> val;
      return;
    }
    else {
      error_at("invalid token");
    }
  }
  token -> class = EOF;
}


void match(int tk) {
  if(token -> class == tk) {
    next();
  }
  else {
    error_at("expected '%c'", tk);
  }
}

Node* expr() {
  return equality();
}
Node* equality() {
  Node* node = relational();
  //printToken();
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



void codeGen(Node* node) {
  if(node -> class == Num) {
    printf("  li a0,%d\n", node -> val);
    printf(push);
    return;
  }
  codeGen(node -> l);
  codeGen(node -> r);
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

int main(int argc, char** argv) {

    //initiallize
    if(argc != 2) {
        error("%s: invalid number of arguments", argv[0]);
        return 1;
    }
    src = argv[1];
    old_src = src;
    char* temp = (char*)calloc(BUFFER_SIZE, sizeof(char));
    token = (Token*)calloc(1, sizeof(Token));
    targetCode = (char*)calloc(BUFFER_SIZE, sizeof(char));
    printf(".global main\n");
    printf("main:\n");

    next();
    Node* ast = expr();
    //printAST(ast);
    codeGen(ast);
    printf(pop, "a0");
    printf("  ret\n");
}