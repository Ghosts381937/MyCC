#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int BUFFER_SIZE = 1024 * 128;//128 Bytes

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "引數數量錯誤\n");
        return -1;
    }
    char* p = argv[1];
    char* temp = (char*)calloc(BUFFER_SIZE, sizeof(char));
    char* targetCode = (char*)calloc(BUFFER_SIZE, sizeof(char));
    strcat(targetCode, ".global main\n");
    strcat(targetCode, "main:\n");

    sprintf(temp, "%ld", strtol(p, &p, 10));
    strcat(targetCode, "  li a0,");strcat(targetCode, temp);strcat(targetCode, "\n");

    while(*p) {
        if(*p == '+') {
            p++;
            sprintf(temp, "%ld", strtol(p, &p, 10));
            strcat(targetCode, "  addiw a0,a0,");strcat(targetCode, temp);strcat(targetCode, "\n");
        }
        else if(*p == '-') {
            p++;
            sprintf(temp, "-%ld", strtol(p, &p, 10));
            strcat(targetCode, "  addiw a0,a0,");strcat(targetCode, temp);strcat(targetCode, "\n");
        }
        else {
            fprintf(stderr, "預料之外的文字: '%c'\n", *p);
            return 1;
        }
    }

    strcat(targetCode, "  ret\n");
    printf("%s", targetCode);
}