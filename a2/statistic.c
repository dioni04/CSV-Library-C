#include "statistic.h"
#include "list.h"

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
    int check = 1;//variavel para caso nao haja moda

    if(!list)
        return NULL;

    list = list->next;
    while (list) {
        if(list->count > aux->count){
            aux = list;
            check = 1;
        }
        else if(list->count == aux->count){
            check = 0;
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
       return array[size/2];
    return (array[size/2] + array[size/2 - 1]) / 2;
}

//moda q retorna endereco do nodo, caso retorno seja NULL nao ha moda
NODE* modeN(float* array, NODE** list, unsigned int size){
    NODE* aux;
    NODE* new;
    //procura se ja esta na lista se nao insere
    for (unsigned int i = 0; i < size; i++){
        aux = onListN(*list, array[i]);
        if(!aux){
            new = initializeListN(array[i]);
            if(!new){
                printf("Allocation Error List");
                return NULL;
            }
            insertList(list, new);
        }
        else
            //se ja estiver na lista aumenta contador
            aux->count++;
    }
    return searchHigherCount(*list);
}

//moda q retorna endereco do nodo, caso retorno seja NULL nao ha moda
NODE* modeS(char** array, NODE** list, unsigned int size){
    NODE* aux;
    NODE* new;
    //procura se ja esta na lista se nao insere
    for (unsigned int i = 0; i < size; i++){
        aux = onListS(*list, array[i]);
        if(!aux){
            new = initializeListS(array[i]);
            if(!new){
                printf("Allocation Error List");
                return NULL;
            }
            insertList(list, new);
        }
        else
            //se ja estiver na lista aumenta contador
            aux->count++;
    }
    return searchHigherCount(*list);
}

float stdDeviation(float* array, float mean, unsigned int size){
    float sum = 0;

    for(unsigned int i = 0; i < size; i++)
        sum += (pow(array[i] - mean, 2));
    return sqrt(sum / size);
}

float min(float* array, unsigned int size){
    float min = array[0];

    for(unsigned int i = 1; i < size; i++)
        if(array[i] < min)
            min = array[i];
    return min;
}

float max(float* array, unsigned int size){
    float max = array[0];

    for(unsigned int i = 1; i < size; i++)
        if(array[i] > max)
            max = array[i];
    return max;
}
