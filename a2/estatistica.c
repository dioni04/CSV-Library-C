#include "statistic.h"
#include <stdio.h>

void freeList(NODE* list){
    NODE* aux = list;

    while(list){
        list = list->next;
        if(aux->string)
            free(aux->string);
        free(aux);
        aux = list;
    }
    return;
}

//cria nodo da lista
NODE* initializeList(float num){
    NODE* list = NULL;

    list = (NODE*)malloc(sizeof(NODE));
    //falha de alloc
    if(!list)
        return list;
    list->num = num;
    list->count = 1;
    list->next = NULL;

    return list;
}

//procura elemento na lista e retorna endereco se tiver
NODE* onList(NODE* list, float arrayElem){
    while(list){
        if(list->num == arrayElem)
            return list;
        list = list->next;
    }
    return NULL;
}

//insere na lista
void insertList(NODE** list, NODE* new){
    NODE* aux = *list;
    if(!*list){
        *list = new;
        return;
    }
    while(aux->next)
        aux = aux->next;
    aux->next = new;
    return;
}

float aritMean(float* array, unsigned int size){
    float sum = 0;
    unsigned int count = 0;

    //media pulando NaN
    for(unsigned int i = 1; i < size; i++){
            count++;
            sum += array[i];
    }
    return sum / count;
}

//procura nodo com maior contador na lista
NODE* searchHigherCount(NODE* list){
    NODE* aux = list;
    int check = 0;//variavel para caso nao haja moda

    if(!list)
        return NULL;

    while (!list) {
        if(list->count > aux->count){
            aux = list;
            check = 0;
        }
        else if(list->count == aux->count){
            check = 1;
        }
        list = list->next;
    }
    if(!check)
        return NULL;
    return aux;
}

//mediana
float median(float* array, unsigned int size){

    if(size % 2)
       return array[size/2 + 1];
    return (array[size/2] + array[size/2 + 1]) / 2;
}

//moda q retorna endereco do nodo, caso retorno seja NULL nao ha moda
NODE* modeF(float* array, NODE* list, unsigned int size){
    NODE* aux;
    NODE* new;
    //procura se ja esta na lista se nao insere
    for (unsigned int i = 0; i < size; i++){
        aux = onList(list, array[i]);
        if(!aux){
            new = initializeList(array[i]);
            if(!new){
                printf("Allocation Error List");
                return NULL;
            }
            insertList(&list, new);
        }
        else
            //se ja estiver na lista aumenta contador
            aux->count++;
    }
    aux = searchHigherCount(list);
    return aux;
}

float stdDeviation(float* array, float mean, unsigned int size){
    float sum = 0;

    for(unsigned int i = 0; i < size; i++)
        sum += (pow(array[i] - mean, 2));
    return sum / size;
}

float minF(float* array, unsigned int size){
    float min = array[0];

    for(unsigned int i = 1; i < size; i++)
        if(array[i] < min)
            min = array[i];
    return min;
}

float maxF(float* array, unsigned int size){
    float max = array[0];

    for(unsigned int i = 1; i < size; i++)
        if(array[i] > max)
            max = array[i];
    return max;
}
