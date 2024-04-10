#ifndef LIST_H
#define LIST_H

#include "io.h"
/*
typedef struct nodeGeneric{
    void* data;
    struct nodeGeneric* next;
}nodeGeneric;
*/

//lista para moda, elementos unicos e enderecos de NaN
typedef struct node{
    float num;
    char* string;//possui ambos dados para poder usar funcao de moda para ambos casos
    char* adressNaN;
    unsigned int count;
    struct node* next;
}NODE;


void freeList(NODE* list);

void uniqueValuesS(NODE* list);

void uniqueValuesN(NODE* list);

NODE* initializeListN(float num);

NODE* initializeListS(char* string);

NODE* onListN(NODE* list, float arrayElem);

NODE* onListS(NODE* list, char* arrayElem);

void insertList(NODE** list, NODE* newNode);

void removeListNaN(NaN** list, NaN* node);

#endif //LIST_H
