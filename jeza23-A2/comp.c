#include "io.h"

/*
 * Biblioteca de funcoes de comparacao para qsort
 */

int compFloat(const void* a, const void* b){
    const float* numA = (const float*)a;
    const float* numB = (const float*)b;

    if (*numA < *numB)
        return -1;
    else if (*numA > *numB)
        return 1;
    return 0;
}

int compFloatA(const void* a, const void* b){
    const SORT_AUX* structA = (const SORT_AUX*)a;
    const SORT_AUX* structB = (const SORT_AUX*)b;
    float numA = atof(structA->string);
    float numB = atof(structB->string);

    if(numA > numB)
        return 1;
    if(numA < numB)
        return -1;
    return 0;
}

int compFloatD(const void* a, const void* b){
    return -compFloatA(a, b);
}

int compStringA(const void* a, const void* b){
    const SORT_AUX* structA = (const SORT_AUX*)a;
    const SORT_AUX* structB = (const SORT_AUX*)b;
    const char* strA = structA->string;
    const char* strB = structB->string;

    return strcmp(strA, strB);
}

int compStringD(const void* a, const void* b){
    return -compStringA(a,b);
}
