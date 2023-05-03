#include "mycc.h"

// The token
Token* token;

char* src;
char* old_src;

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
    printf(".global main\n");
    printf("main:\n");

    next();
    Node* ast = expr();
    //printAST(ast);
    codegen(ast);
    printf(pop, "a0");
    printf("  ret\n");
}