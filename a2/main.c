#include "io.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
    FILE* file = NULL;
    DATA csv;

    char* filename = NULL;
    char* variable = NULL;
    char* filter = NULL;
    char* name = NULL;

    //Abre a arquivo se houver argumento
    if(argc == 1){
        perror("Faltou argumento de arquivo .csv");
        return 1;
    }
    filename = strdup(argv[1]);

    file = fopen(filename, "r");
    if(!file){
        perror("Error file open");
        return 1;
    }

    //inicializacao de dados
    csv.matrix = NULL;
    csv.disable = NULL;
    csv.type = NULL;

    csv.nRows = count_rows(file);
    rewind(file);

    csv.nCol = count_columns(file);
    rewind(file);

    //alocacao vetor de enable Enabled = 0 / Disabled = 1
    csv.disable = (unsigned int*)calloc(csv.nRows + 1, sizeof(unsigned int));
    if (!csv.disable){
        perror("Allocation Error enable");
        return 1;
    }

    //alocacao vetor de maior tamanho coluna
    csv.sizeC = (unsigned int*)malloc(sizeof(unsigned int) * csv.nCol);
    if (!csv.sizeC){
        perror("Allocation Error sizeC");
        return 1;
    }

    //alocacao vetor de tipo
    csv.type = (char**)malloc(sizeof(char*) * csv.nCol);
    if(!csv.type){
        perror("Allocation Error type");
        return 1;
    }

    //alocacao da matriz
    csv.matrix = (char***)malloc(sizeof(char*) * csv.nRows);
    if(!csv.matrix){
        perror("Allocation Error matrix");
        return 1;
    }
    for(int i = 0; i < csv.nRows; i++){
        csv.matrix[i] = (char**)malloc(sizeof(char*) * csv.nCol);
        if(!csv.matrix[i]){
             perror("Allocation Error matrix");
            return 1;
        }
    }
    csvLoad(file, &csv);
    rewind(file);

    definir(&csv);
    countSize(&csv);
    mostrar(&csv);

    printf("Entre com a variavel: ");
    scanf("%ms", &name);
    printf("Escolha um filtro ( == > >= < <= != ) : ");
    scanf("%ms", &filter);
    printf("Digite um valor: ");
    scanf("%ms", &variable);
    filtro(&csv, name, filter, variable);

    //free da memoria
    freeCSV(&csv);
    free(filename);
    free(name);
    free(filter);
    free(variable);
    fclose(file);
    return 0;
}
