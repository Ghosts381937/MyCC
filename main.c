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
// class of tokens 
enum {
  RESERVED, 
  NUM,
}Class;

typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_NUM,
} NodeType;

typedef struct Token Token;
typedef struct Node Node;

// the data of token
struct Token {
  int class; 
  int val;        
};

struct Node {
  NodeType type;
  Node *l;     // left
  Node *r;     // right
  int val;       // the value of NUM
};

Node* expr();
Node* mul();
Node* term();

Node* newNode(NodeType type, Node* l, Node *r, int val) {
  Node* node = (Node*)calloc(1, sizeof(Node));
  node -> type = type;
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
  printf("node -> type: %d\n", node -> type);
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
    src++;
    if(isspace(token -> val)) {
      continue;
    }
    else if(isdigit(token -> val)) {
      token -> class = NUM;
      token -> val = strtol(src - 1, &src, 10);
      return;
    }
    else if(token -> val == '+' || token -> val == '-' || token -> val == '*' || token -> val == '/') {
      token -> class = RESERVED;
      return;
    }
    else if(token -> val == '(' || token -> val == ')') {
      token -> class = token -> val;
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
Node* term() {
  Node* node;
  if(token->val == '(') {
    match('(');
    node = expr();
    match(')');
  }
  else if(token -> class == NUM) {
    node = newNode(ND_NUM, NULL, NULL, token -> val);
    match(NUM);
  }
  return node;
}
Node* mul() {
  Node* node = term();
  for(;;) {
    if(token->val == '*') {
      match(RESERVED);
      node = newNode(ND_MUL, node, term(), 0);
    }
    else if(token->val == '/') {
      match(RESERVED);
      node = newNode(ND_DIV, node, term(), 0);
    }
    else {
      return node;
    }
  }
}
Node* expr() {
  Node* node = mul();
  for(;;) {
    if(token->val == '+') {
      match(RESERVED);
      node = newNode(ND_ADD, node, mul(), 0);
    }
    else if(token->val == '-') {
      match(RESERVED);
      node = newNode(ND_SUB, node, mul(), 0);
    }
    else {
      return node;
    }
  }
}

void codeGen(Node* node) {
  if(node -> type == ND_NUM) {
    printf("  li a0,%d\n", node -> val);
    printf(push);
    return;
  }
  codeGen(node -> l);
  codeGen(node -> r);
  printf(pop, "a1");
  printf(pop, "a0");
  switch(node -> type) {
    case ND_ADD:
      printf("  add a0,a0,a1\n");
      break;
    case ND_SUB:
      printf("  sub a0,a0,a1\n");
      break;
    case ND_MUL:
      printf("  mul a0,a0,a1\n");
      break;
    case ND_DIV:
      printf("  div a0,a0,a1\n");
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