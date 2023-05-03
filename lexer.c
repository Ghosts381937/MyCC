#include "mycc.h"

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