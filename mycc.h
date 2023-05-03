#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024 * 128 //128 Bytes
#define push "  sw a0,0(sp)\n  addi sp,sp,-4\n" //push: sp[0] = a0 then sp--
#define pop "  lw %s,4(sp)\n  addi sp,sp,4\n" //pop: rd(%s) = sp[4] then sp++
extern char* src;
extern char* old_src;
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

// The token
extern Token* token;

void error(char *fmt, ...);
void codegen(Node* node);
void printToken();
void next();
void match(int tk);