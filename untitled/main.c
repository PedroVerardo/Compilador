#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "geraCodigo.h"
#define TAM 1000
#pragma warning(disable:4996)



static void error(const char* msg, int line) {
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}

void printa(unsigned char* hexadecimal, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        printf("%02x ", hexadecimal[i]);
    }
}

int main(void) {
    FILE* msg;
    unsigned char codigo[500];
    for(int i = 0;i<500;i++){
        codigo[i] = 0;
    }

    if ((msg = fopen("/home/pedro/CLionProjects/untitled/programa.txt", "r")) == NULL) {
        perror("nao conseguiu abrir arquivo!");
        exit(1);
    }

    funcp b = geraCodigos(msg, codigo);

    printa(codigo,40);

    printf("\nret: %d\n", (*b)(5,3));

    return 0;
}