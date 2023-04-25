#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int BUFFER_SIZE = 1024 * 128;//128 Bytes
char* src;
char* old_src;

// tokens and classes
enum {
  RESERVED, 
  NUM,
};

typedef struct Token Token;

// the data of token
struct Token {
  int class; 
  int val;        
};

// The token stream
Token* token;

//Debug
void printToken() {
  printf("token -> class: %d\n", token -> class);
  printf("token -> val: %d\n", token -> val);
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
    else if(token -> val == '+' || token -> val == '-') {
      token -> class = RESERVED;
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


int main(int argc, char** argv) {
    if(argc != 2) {
        error("%s: invalid number of arguments", argv[0]);
        return 1;
    }
    src = argv[1];
    old_src = src;
    char* temp = (char*)calloc(BUFFER_SIZE, sizeof(char));
    char* targetCode = (char*)calloc(BUFFER_SIZE, sizeof(char));
    token = (Token*)calloc(1, sizeof(Token));
    strcat(targetCode, ".global main\n");
    strcat(targetCode, "main:\n");

    next();

    //the first number
    if(token -> class == NUM) {
      sprintf(temp, "%d", token -> val);
      strcat(targetCode, "  li a0,");strcat(targetCode, temp);strcat(targetCode, "\n");
      //eat the first number
      match(NUM);
    }
    else {
      error("expected a number");
    }
    //left expression
    while(token -> class >= 0) {
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