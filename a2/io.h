#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024

//estrutura principal do csv
typedef struct csv{
    unsigned int nRows;
    unsigned int nCol;
    unsigned int sizeR; //tamanho de nRows
    unsigned int* sizeC; //maior valor da coluna para formatacao
    unsigned int* disable; //disable da linha
    char** type; //vetor de tipos
    char*** matrix;
}DATA;


unsigned int count_rows(FILE* file);
unsigned int count_columns(FILE* file);


void freeCSV(DATA* csv);
int csvLoad(FILE* file, DATA* csv);
void definir(DATA* csv);
void mostrar(DATA* csv);
void countSize(DATA* csv);
int filtro(DATA* csv, char* name, char* filter, char* variable);

#endif //IO_H
