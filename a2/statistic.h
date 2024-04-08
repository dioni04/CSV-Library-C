#include "io.h"

#ifndef STATISTIC_H
#define STATISTIC_H

//lista para moda e elementos unicos
typedef struct node{
    float num;
    char* string;//possui ambos dados para poder usar funcao de moda para ambos casos
    unsigned int count;
    struct node* next;
}NODE;

void freeList(NODE* list);

float aritMean(float* array, unsigned int size);

NODE* modeF(float* array, NODE* list, unsigned int size);
float median(float* array, unsigned int size);
float stdDeviation(float* array, float mean, unsigned int size);

float minF(float* array, unsigned int size);
float maxF(float* array, unsigned int size);

#endif //STATISTIC_H
