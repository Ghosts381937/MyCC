#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int BUFFER_SIZE = 1024 * 128;//128 Bytes

// tokens and classes
enum {
  RESERVED, 
  NUM,
};

typedef struct Token Token;

// the data of token
struct Token {
  int class; 
  Token* next;
  int val;        
};

Token* new_token(int class, int val) {
  Token* tok = (Token*)calloc(1, sizeof(Token));
  tok -> class = class;
  tok -> next = NULL;
  tok -> val = val;
  return tok;
}

// The token stream
Token* token_stream;
Token* token;




void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void next() {
  token = token -> next;
}

void match(int tk) {
  if(token -> class == tk) {
    next();
  }
  else {
    error("expected token: %d\n", tk);
  }
}

Token* tokenize(char *p) {
  Token head;
  head.val = 0;
  head.next = NULL;
  Token* cur = &head;

  while(*p) {
    if(isspace(*p)) {
      p++;
      continue;
    }
    else if(isdigit(*p)) {
      cur -> next = new_token(NUM, strtol(p, &p, 10));
    }
    else if(*p == '+' || *p == '-') {
      cur -> next = new_token(RESERVED, *p++);
    }
    else {
      error("invalid token");
    }
    cur = cur -> next;
  }

  cur -> next = new_token(EOF, 0);
  return head.next;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        error("%s: invalid number of arguments", argv[0]);
        return 1;
    }
    char* p = argv[1];
    char* temp = (char*)calloc(BUFFER_SIZE, sizeof(char));
    char* targetCode = (char*)calloc(BUFFER_SIZE, sizeof(char));
    strcat(targetCode, ".global main\n");
    strcat(targetCode, "main:\n");

    token_stream = tokenize(p);
    token = token_stream;

    if(token -> class == NUM) {//the first number
      sprintf(temp, "%d", token -> val);
      strcat(targetCode, "  li a0,");strcat(targetCode, temp);strcat(targetCode, "\n");
      match(NUM);//eat number
    }
    else {
      error("expected a number");
    }

    while(token->class != EOF) {//left expression
        if(token->val == '+') {
          match(RESERVED);
          
          sprintf(temp, "%d", token -> val);
          strcat(targetCode, "  addiw a0,a0,");strcat(targetCode, temp);strcat(targetCode, "\n");
        }
        else if(token->val == '-') {
          match(RESERVED);
          sprintf(temp, "%d", -1 * token -> val);
          strcat(targetCode, "  addiw a0,a0,");strcat(targetCode, temp);strcat(targetCode, "\n");
        }
        match(NUM);
    }

    strcat(targetCode, "  ret\n");
    printf("%s", targetCode);
}