#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "geraCodigo.h"
#pragma warning(disable:4996)

struct dict_linhas{
    int linha;
    int val_ini;
};
typedef struct dict_linhas Dict_linhas;




static void error(const char* msg, int line)
{
    fprintf(stderr, "erro %s na linha %d\n", msg, line);
    exit(EXIT_FAILURE);
}

unsigned char* concatena(unsigned char* codigo, int* pos, unsigned char* vec, int vec_tam )
{
    for (int i = 0; i < vec_tam; i++)
    {
        codigo[(*pos)++] = vec[i];
    }
}

funcp geraCodigos(FILE* msg, unsigned char codigo[])
{
    //infos do arquivo
    int line = 1;
    int  c;

    //vetor auxiliar para poder dar o jmp para uma parte do codigo
    Dict_linhas dict[30];
    dict[0].linha = line;
    dict[0].val_ini = 8;

    //union para manipular os bytes de um int
    union {
        int i;
        unsigned char b[4];
    } dist;

    //codigo assebly em hexadecimal
    int pos = 0;

    //instrucoes da pilha
    codigo[pos++] = 0x55; /*pushq*/
    //mov %rsp, %rbp
    codigo[pos++] = 0x48;
    codigo[pos++] = 0x89;
    codigo[pos++] = 0xe5;
    //subq $16, %rsp
    codigo[pos++] = 0x48;
    codigo[pos++] = 0x83;
    codigo[pos++] = 0xec;
    codigo[pos++] = 0x10;


    while ((c = getc(msg)) != EOF)
    {
        switch (c)
        {

            case 'r':
            { /* retorno */
                char var0;
                int idx0;
                if (fscanf(msg, "et %c%d", &var0, &idx0) != 2)
                    error("comando invalido r\n", line);
                if(var0 == '$')
                {
                        dist.i = idx0;
                        codigo[pos++] = 0xb8; /*mov*/
                        //bytes do inteiro.
                        codigo[pos++] = dist.b[0];
                        codigo[pos++] = dist.b[1];
                        codigo[pos++] = dist.b[2];
                        codigo[pos++] = dist.b[3];
                }

                else if(var0 == 'v')
                {
                        int pilha_pos = 16 - 4*idx0;
                        codigo[pos++] = 0x8b;
                        codigo[pos++] = 0x45;
                        codigo[pos++] = 0xf0 + pilha_pos;
                }
                else if(var0 == 'p')
                {
                    if(idx0 == 1)
                        {
                            codigo[pos++] = 0x89;
                            codigo[pos++] = 0xf8;
                        }
                    else
                        {
                            codigo[pos++] = 0x89;
                            codigo[pos++] = 0xf0;
                        }

                }
                    codigo[pos++] = 0xc9;
                    codigo[pos++] = 0xc3;

                printf("%d ret %c%d\n", line, var0, idx0);
                break;
            }
            case 'v':
            {/* atribuição e op. aritmetica */

                char var0 = c, var1, op;
                int idx0, idx1;
                if (fscanf(msg, "%d %c= %c%d", &idx0, &op, &var1, &idx1) != 4)
                    error("comando invalido p\n", line);
                int pilha_pos = 16 - 4*idx0;
                if(op == '+')
                {
                    if(var1 == '$')
                    {
                        dist.i = idx1;
                        codigo[pos++] = 0x81;
                        codigo[pos++] = 0x45;
                        codigo[pos++] = 0xf0 + pilha_pos;
                        codigo[pos++] = dist.b[0];
                        codigo[pos++] = dist.b[1];
                        codigo[pos++] = dist.b[2];
                        codigo[pos++] = dist.b[3];
                    }
                    else if(var1 == 'v')
                    {
                        int pilha_pos_dir = 16 - 4*idx1;
                        codigo[pos++] = 0x8b;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos_dir;
                        codigo[pos++] = 0x01;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos;
                    }
                    if(var1 == 'p')
                    {
                        if(idx1 == 1)
                        {
                            codigo[pos++] = 0x01;
                            codigo[pos++] = 0x7d;
                            codigo[pos++] = 0xf0 + pilha_pos;
                        }
                        else
                        {
                            codigo[pos++] = 0x01;
                            codigo[pos++] = 0x75;
                            codigo[pos++] = 0xf0 + pilha_pos;
                        }
                    }
                }
                if(op == '-')
                {
                    if(var1 == '$')
                    {
                        dist.i = idx1;
                        codigo[pos++] = 0xb9;
                        codigo[pos++] = dist.b[0];
                        codigo[pos++] = dist.b[1];
                        codigo[pos++] = dist.b[2];
                        codigo[pos++] = dist.b[3];
                        codigo[pos++] = 0x29;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos;
                    }
                    else if(var1 == 'v')
                    {
                        int pilha_pos_dir = 16 - 4*idx1;
                        codigo[pos++] = 0x8b;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos_dir;
                        codigo[pos++] = 0x29;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos;
                    }
                    if(var1 == 'p')
                    {
                        if(idx1 == 1)
                        {
                            codigo[pos++] = 0x29;
                            codigo[pos++] = 0x7d;
                            codigo[pos++] = 0xf0 + pilha_pos;
                        }
                        else
                        {
                            codigo[pos++] = 0x29;
                            codigo[pos++] = 0x75;
                            codigo[pos++] = 0xf0 + pilha_pos;
                        }
                    }
                }
                if(op == '*')
                {
                    if(var1 == '$')
                    {
                        dist.i = idx1;
                        codigo[pos++] = 0xb9;
                        codigo[pos++] = dist.b[0];
                        codigo[pos++] = dist.b[1];
                        codigo[pos++] = dist.b[2];
                        codigo[pos++] = dist.b[3];
                        codigo[pos++] = 0x8b;
                        codigo[pos++] = 0x55;
                        codigo[pos++] = 0xf0 + pilha_pos;
                        codigo[pos++] = 0x0f;
                        codigo[pos++] = 0xaf;
                        codigo[pos++] = 0xca;
                        codigo[pos++] = 0x89;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos;
                    }
                    else if(var1 == 'v')
                    {
                        int pilha_pos_dir = 16 - 4*idx1;
                        codigo[pos++] = 0x8b;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos_dir;
                        codigo[pos++] = 0x8b;
                        codigo[pos++] = 0x55;
                        codigo[pos++] = 0xf0 + pilha_pos;
                        codigo[pos++] = 0x0f;
                        codigo[pos++] = 0xaf;
                        codigo[pos++] = 0xca;
                        codigo[pos++] = 0x89;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos;
                    }
                    // O valor de p1 ou p2 esta sedo alterado, pq eu sou burro e multipliquei o propio %esi ou %edi
                    if(var1 == 'p')
                    {
                        if(idx1 == 1)
                        {
                            codigo[pos++] = 0x8b;
                            codigo[pos++] = 0x4d;
                            codigo[pos++] = 0xf0 + pilha_pos;
                            codigo[pos++] = 0x6b;
                            codigo[pos++] = 0xff;
                            codigo[pos++] = 0x02;
                            codigo[pos++] = 0x89;
                            codigo[pos++] = 0x7d;
                            codigo[pos++] = 0xf0 + pilha_pos;
                        }
                        else
                        {
                            codigo[pos++] = 0x8b;
                            codigo[pos++] = 0x4d;
                            codigo[pos++] = 0xf0 + pilha_pos;
                            codigo[pos++] = 0x6b;
                            codigo[pos++] = 0xf6;
                            codigo[pos++] = 0x02;
                            codigo[pos++] = 0x89;
                            codigo[pos++] = 0x75;
                            codigo[pos++] = 0xf0 + pilha_pos;
                        }
                    }
                }
                if(op == ':')
                {
                    if(var1 == '$')
                    {
                        dist.i = idx1;
                        codigo[pos++] = 0xc7;
                        codigo[pos++] = 0x45;
                        codigo[pos++] = 0xf0 + pilha_pos;
                        codigo[pos++] = dist.b[0];
                        codigo[pos++] = dist.b[1];
                        codigo[pos++] = dist.b[2];
                        codigo[pos++] = dist.b[3];
                    }
                    else if(var1 == 'v')
                    {
                        int pilha_pos_dir = 16 - 4*idx1;
                        codigo[pos++] = 0x8b;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos_dir;
                        codigo[pos++] = 0x89;
                        codigo[pos++] = 0x4d;
                        codigo[pos++] = 0xf0 + pilha_pos;
                    }
                    if(var1 == 'p')
                    {
                        if(idx1 == 1)
                        {
                            codigo[pos++] = 0x89;
                            codigo[pos++] = 0x7d;
                            codigo[pos++] = 0xf0 + pilha_pos;
                        }
                        else
                        {
                            codigo[pos++] = 0x89;
                            codigo[pos++] = 0x75;
                            codigo[pos++] = 0xf0 + pilha_pos;
                        }
                    }
                }
                printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1);
                break;
            }
            case 'p': { /* atribuição e op. aritmetica */
                char var0 = c, var1, op;
                unsigned char val;
                int idx0, idx1;
                if (fscanf(msg, "%d %c= %c%d", &idx0, &op, &var1, &idx1) != 4)
                    error("comando invalido p\n", line);
                if(op == '+')
                {

                }
                else if(op == '-')
                {
                    if(var1 == '$')
                    { //o maior numero possivel para a multiplicacao e 255 nao sei pq
                        unsigned char val2;
                        idx0 == 1? (val = 0xd7): (val = 0xd6);
                        dist.i = idx1;
                        codigo[pos++] = 0xba;
                        codigo[pos++] = dist.b[0];
                        codigo[pos++] = dist.b[1];
                        codigo[pos++] = dist.b[2];
                        codigo[pos++] = dist.b[3];
                        codigo[pos++] = 0x29;
                        codigo[pos++] = val;
                    }
                    else if(var1 == 'v')
                    {
                        idx0 == 1? (val = 0x7d): (val = 0x75);
                        int pilha_pos_dir = 16 - 4*idx1;
                        codigo[pos++] = 0x2b;
                        codigo[pos++] = val;
                        codigo[pos++] = 0xf0 + pilha_pos_dir;
                    }
                    if(var1 == 'p')
                    {
                        if(idx0 == 1 && idx1 == 1)
                            val = 0xff;
                        else if(idx0 == 1 && idx1 == 2)
                            val = 0xf7;
                        else if(idx0 == 2 && idx1 == 2)
                            val = 0xf6;
                        else if(idx0 == 2 && idx1 == 1)
                            val = 0xfe;
                        codigo[pos++] = 0x29;
                        codigo[pos++] = val;
                    }
                }
                else if(op == '*')
                {
                    if(var1 == '$')
                    { //o maior numero possivel para a multiplicacao e 255 nao sei pq
                        unsigned char val2;
                        idx0 == 1? (val = 0xfa): (val = 0xf2);
                        dist.i = idx1;
                        codigo[pos++] = 0xba;
                        codigo[pos++] = dist.b[0];
                        codigo[pos++] = dist.b[1];
                        codigo[pos++] = dist.b[2];
                        codigo[pos++] = dist.b[3];
                        codigo[pos++] = 0x0f;
                        codigo[pos++] = 0xaf;
                        codigo[pos++] = val;
                    }
                    else if(var1 == 'v')
                    {
                        idx0 == 1? (val = 0x7d): (val = 0x75);
                        int pilha_pos_dir = 16 - 4*idx1;
                        codigo[pos++] = 0x0f;
                        codigo[pos++] = 0xaf;
                        codigo[pos++] = val;
                        codigo[pos++] = 0xf0 + pilha_pos_dir;
                    }
                    if(var1 == 'p')
                    {
                        if(idx0 == 1 && idx1 == 1)
                            val = 0xff;
                        else if(idx0 == 1 && idx1 == 2)
                            val = 0xfe;
                        else if(idx0 == 2 && idx1 == 2)
                            val = 0xf6;
                        else if(idx0 == 2 && idx1 == 1)
                            val = 0xf7;
                        codigo[pos++] = 0x0f;
                        codigo[pos++] = 0xaf;
                        codigo[pos++] = val;
                    }
                }
                else if(op == ':')
                {
                    if(var1 == '$')
                    {
                        idx0 == 1? (val = 0xbf): (val = 0xbe);
                        dist.i = idx1;
                        codigo[pos++] = val;
                        codigo[pos++] = dist.b[0];
                        codigo[pos++] = dist.b[1];
                        codigo[pos++] = dist.b[2];
                        codigo[pos++] = dist.b[3];
                    }
                    else if(var1 == 'v')
                    {//nao sei onde mas esta com problema
                        idx0 == 1? (val = 0xd7): (val = 0xd6);
                        int pilha_pos_dir = 16 - 4*idx1;
                        codigo[pos++] = 0x8b;
                        codigo[pos++] = 0x55;
                        codigo[pos++] = 0x70 + pilha_pos_dir;
                        codigo[pos++] = 0x89;
                        codigo[pos++] = val;
                    }
                    if(var1 == 'p')
                    {
                        if(idx0 == 1 && idx1 == 1)
                            val = 0xff;
                        else if(idx0 == 1 && idx1 == 2)
                            val = 0xf7;
                        else if(idx0 == 2 && idx1 == 2)
                            val = 0xf6;
                        else if(idx0 == 2 && idx1 == 1)
                            val = 0xfe;
                        codigo[pos++] = 0x89;
                        codigo[pos++] = val;
                    }
                }
                printf("%d %c%d %c= %c%d\n", line, var0, idx0, op, var1, idx1);
                break;
            }
            case 'i':
            { /* desvio condicional */
                char var0;
                int idx0, n1, n2;
                if (fscanf(msg, "f %c%d %d %d", &var0, &idx0, &n1, &n2) != 4)
                    error("comando invalido i", line);  
                printf("%d if %c%d %d %d\n", line, var0, idx0, n1, n2);
                break;
            }
            case 'g':
            { /* desvio incondicional */
                int n1;
                if (fscanf(msg, "o %d", &n1) != 1)
                    error("comando invalido g", line);
                printf("%d go %d\n", line, n1);
                break;
            }
            default: error("comando desconhecido", line);
        }
        line++;

        fscanf(msg, " ");
    }
    return (funcp)codigo;
}