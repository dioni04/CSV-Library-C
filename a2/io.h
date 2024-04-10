#ifndef IO_H
#define IO_H


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define IS_NUM(S)((!strcmp(S,"N")) ? (1):(0))
#define MAX 1024

//lista com enderecos de dados NaN e seus dados
typedef struct NaN{
    char** adress;
    unsigned int row;
    unsigned int col;
    struct NaN* next;
}NaN;

//estrutura principal do csv
typedef struct csv{
    unsigned int nRows;
    unsigned int nRowsActive;
    unsigned int nCol;
    unsigned int nColActive;
    unsigned int sizeR; //tamanho de nRows
    unsigned int* sizeC; //maior valor da coluna para formatacao
    unsigned int* disableR; //disable da linha
    unsigned int* disableC;
    char** type; //vetor de tipos
    char*** matrix;
    NaN* NaNSet;
}DATA;

unsigned int count_rows(FILE* file);
unsigned int count_columns(FILE* file);


void freeCSV(DATA* csv);
int csvLoad(FILE* file, DATA* csv);
void define(DATA* csv);
void summary(DATA* csv);
void show(DATA* csv);
void showWrapper(DATA* csv);
void countSize(DATA* csv);
int filter(DATA* csv);

int descriptionWrapper(DATA* csv);
void selection(DATA* csv);

NaN* initializeListNaN(char* string, unsigned int i, unsigned int j);
void insertListNaN(NaN** list, NaN* newNode);
void freeListNaN(NaN* list);

int missingData(DATA* csv);

int csvStoreWrapper(DATA* csv);

#endif //IO_H
