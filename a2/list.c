#include "list.h"
#include "io.h"

void freeList(NODE* list){
    NODE* aux = list;

    while(list){
        list = list->next;
        free(aux);
        aux = list;
    }
    return;
}

void freeListNaN(NaN* list){
    NaN* aux = list;

    while(list){
        list = list->next;
        free(aux);
        aux = list;
    }
    return;
}

void uniqueValuesS(NODE* list){
    printf("Valores unicos: [");
    while(list){
        printf("'%s'", list->string);
        list = list->next;
        if(list)
            printf(", ");
    }
    printf("]\n");
    return;
}

void uniqueValuesN(NODE* list){
    printf("Valores unicos: [");
    while(list){
        printf("%.1f", list->num);
        list = list->next;
        if(list)
            printf(", ");
    }
    printf("]\n");
    return;
}

//cria nodo da lista
NODE* initializeListN(float num){
    NODE* list = NULL;

    list = (NODE*)malloc(sizeof(NODE));
    //falha de alloc
    if(!list)
        return list;
    list->num = num;
    list->count = 1;
    list->next = NULL;
    list->string = NULL;

    return list;
}

//cria nodo da lista
NODE* initializeListS(char* string){
    NODE* list = NULL;

    list = (NODE*)malloc(sizeof(NODE));
    //falha de alloc
    if(!list)
        return list;
    list->count = 1;
    list->next = NULL;
    list->string = string;

    return list;
}

//cria nodo da lista de NaN
NaN* initializeListNaN(char* string, unsigned int i, unsigned int j){
    NaN* node = NULL;

    node = (NaN*)malloc(sizeof(NaN));
    //falha de alloc
    if(!node)
        return NULL;

    node->adress = &string;
    node->row = i;
    node->col = j;
    node->next = NULL;

    return node;
}

//procura elemento na lista e retorna endereco se tiver
NODE* onListN(NODE* list, float arrayElem){
    while(list){
        if(list->num == arrayElem)
            return list;
        list = list->next;
    }
    return NULL;
}
//procura elemento na lista e retorna endereco se tiver
NODE* onListS(NODE* list, char* arrayElem){
    while(list){
        if(!strcmp(list->string, arrayElem))
            return list;
        list = list->next;
    }
    return NULL;
}

//insere na lista
void insertList(NODE** list, NODE* newNode){
    NODE* aux = *list;
    if(!*list){
        *list = newNode;
        return;
    }
    while(aux->next)
        aux = aux->next;
    aux->next = newNode;
    return;
}


//insere na lista de NaN
void insertListNaN(NaN** list, NaN* newNode){
    NaN* aux = *list;
    if(!*list){
        *list = newNode;
        return;
    }
    while(aux->next)
        aux = aux->next;
    aux->next = newNode;
    return;
}

//remove node da lista
void removeListNaN(NaN** list, NaN* node){
    NaN* aux = *list;
    //se tiver no primeiro nodo
    if(aux == node){
        *list = aux->next;
        free(aux);
        return;
    }
    while(aux->next != node)
        aux = aux->next;
    aux->next = node->next;
    free(node);
    return;
}
