#ifndef LIST_H
#define LIST_H

#include "io.h"

//lista para moda, elementos unicos e enderecos de NaN
typedef struct node{
    char* string;//possui ambos dados para poder usar funcao de moda para ambos casos
    unsigned int count;
    struct node* next;
}NODE;


void freeList(NODE* list);

void uniqueValuesS(NODE* list);

void uniqueValuesN(NODE* list);

NODE* initializeListS(char* string);

NODE* onListS(NODE* list, char* arrayElem);

NaN* onListNaN(NaN* list, unsigned int i, unsigned int j);

void insertList(NODE** list, NODE* newNode);

void removeListNaN(NaN** list, NaN* node);

#endif //LIST_H
