#include "io.h"
/*
 * Funcoes para filtro de comparacao
 * todas tem mesmos parametros para se usar um vetor de ponteiro de funcoes
 */

void eq(DATA* csv, unsigned int j, char* value){
    float numVar;
    float numMat;

    if(IS_NUM(csv->type[j])){
        numVar = strtof(value, NULL);
        for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é == desabilita
            numMat = strtof(csv->matrix[i][j], NULL);
            if(!(numMat == numVar)){
                csv->nRowsActive--;
                csv->disableR[i] = 1;
            }
        }
    }//string
    else{
        for(unsigned int i = 1; i < csv->nRows; i++)
        //se nao é == desabilita
        if(strcmp(csv->matrix[i][j],value)){
            csv->nRowsActive--;
            csv->disableR[i] = 1;
        }
    }
    return;
}

void neq(DATA* csv, unsigned int j, char* value){
    float numVar;
    float numMat;

    if(IS_NUM(csv->type[j])){
        numVar = strtof(value, NULL);
        for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é == desabilita
            numMat = strtof(csv->matrix[i][j], NULL);
            if(!(numMat != numVar)){
                csv->nRowsActive--;
                csv->disableR[i] = 1;
            }
        }
    }//string
    else{
        for(unsigned int i = 1; i < csv->nRows; i++)
        //se nao é == desabilita
        if(!strcmp(csv->matrix[i][j],value)){
            csv->nRowsActive--;
            csv->disableR[i] = 1;
        }
    }
    return;
}

void lt(DATA* csv, unsigned int j, char* value){
    float numVar;
    float numMat;

    if(IS_NUM(csv->type[j])){
        numVar = strtof(value, NULL);
        for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é == desabilita
            numMat = strtof(csv->matrix[i][j], NULL);
            if(!(numMat < numVar)){
                csv->nRowsActive--;
                csv->disableR[i] = 1;
            }
        }
    }//string
    else{
        for(unsigned int i = 1; i < csv->nRows; i++)
        //se nao é == desabilita
        if(strcmp(csv->matrix[i][j],value) >= 0){
            csv->nRowsActive--;
            csv->disableR[i] = 1;
        }
    }
    return;
}

void lte(DATA* csv, unsigned int j, char* value){
    float numVar;
    float numMat;

    if(IS_NUM(csv->type[j])){
        numVar = strtof(value, NULL);
        for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é == desabilita
            numMat = strtof(csv->matrix[i][j], NULL);
            if(!(numMat <= numVar)){
                csv->nRowsActive--;
                csv->disableR[i] = 1;
            }
        }
    }//string
    else{
        for(unsigned int i = 1; i < csv->nRows; i++)
        //se nao é == desabilita
        if(strcmp(csv->matrix[i][j],value) > 0){
            csv->nRowsActive--;
            csv->disableR[i] = 1;
        }
    }
    return;
}

void gt(DATA* csv, unsigned int j, char* value){
    float numVar;
    float numMat;

    if(IS_NUM(csv->type[j])){
        numVar = strtof(value, NULL);
        for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é == desabilita
            numMat = strtof(csv->matrix[i][j], NULL);
            if(!(numMat > numVar)){
                csv->nRowsActive--;
                csv->disableR[i] = 1;
            }
        }
    }//string
    else{
        for(unsigned int i = 1; i < csv->nRows; i++)
        //se nao é == desabilita
        if(strcmp(csv->matrix[i][j],value) <= 0){
            csv->nRowsActive--;
            csv->disableR[i] = 1;
        }
    }
    return;
}

void gte(DATA* csv, unsigned int j, char* value){
    float numVar;
    float numMat;

    if(IS_NUM(csv->type[j])){
        numVar = strtof(value, NULL);
        for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é == desabilita
            numMat = strtof(csv->matrix[i][j], NULL);
            if(!(numMat >= numVar)){
                csv->nRowsActive--;
                csv->disableR[i] = 1;
            }
        }
    }//string
    else{
        for(unsigned int i = 1; i < csv->nRows; i++)
        //se nao é == desabilita
        if(strcmp(csv->matrix[i][j],value) < 0){
            csv->nRowsActive--;
            csv->disableR[i] = 1;
        }
    }
    return;
}
