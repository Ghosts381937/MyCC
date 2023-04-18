#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int BUFFER_SIZE = 1024 * 128;//128 Bytes

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "引數數量錯誤\n");
        return -1;
    }
    char* targetCode = (char*)calloc(BUFFER_SIZE, sizeof(char));
    strcat(targetCode, ".global main\n");
    strcat(targetCode, "main:\n");
    strcat(targetCode, "  li a0,");strcat(targetCode, argv[1]);strcat(targetCode, "\n");
    strcat(targetCode, "  jr ra\n");
    printf("%s", targetCode);
}