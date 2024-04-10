#include "io.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
    FILE* file = NULL;
    DATA csv;

    char* filename = NULL;
    int option;

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
    csv.disableR = NULL;
    csv.type = NULL;
    csv.NaNSet = NULL;

    csv.nRows = count_rows(file);
    csv.nRowsActive = csv.nRows;
    rewind(file);

    csv.nCol = count_columns(file);
    csv.nColActive = csv.nCol;
    rewind(file);

    //alocacao vetor de enable linha Enabled = 0 / Disabled = 1
    csv.disableR = (unsigned int*)calloc(csv.nRows + 1, sizeof(unsigned int));
    if (!csv.disableR){
        perror("Allocation Error enable");
        return 1;
    }

        //alocacao vetor de enable coluna Enabled = 0 / Disabled = 1
    csv.disableC = (unsigned int*)calloc(csv.nCol + 1, sizeof(unsigned int));
    if (!csv.disableC){
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

    countSize(&csv);
    define(&csv);
    while(1) {
        printf("1) Sumário do Arquivo\n2) Mostrar\n3) Filtros\n4) Descrição dos Dados\n5) Ordenação\n6) Seleção\n7) Dados Faltantes\n8) Salvar Dados\n9) Fim\n");
        printf("Opção: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                summary(&csv);
                break;
            case 2:
                showWrapper(&csv);
                break;
            case 3:
                filter(&csv);
                break;
            case 4:
                descriptionWrapper(&csv);
                break;
            case 5:
                break;
            case 6:
                selection(&csv);
                break;
            case 7:
                missingData(&csv);
                break;
            case 8:
                csvStoreWrapper(&csv);
                break;
            case 9://free da memoria
                freeCSV(&csv);
                free(filename);
                fclose(file);
                return 0;
            default:
                printf("Opção Inválida\n\n");
        };
    }

/*
    printf("Entre com a variavel: ");
    scanf("%[^\n]", name);
    getchar();

    printf("Escolha um filtro ( == > >= < <= != ) : ");
    scanf("%2s", filter);
    printf("Digite um valor: ");
    getchar();
    scanf("%[^\n]", variable);
    getchar();
    filtro(&csv, name, filter, variable);

    printf("Entre com a variavel: ");
    scanf("%[^\n]", variable);
    getchar();

    descriptionWrapper(&csv, variable);
*/
    selection(&csv);
    //free da memoria
    freeCSV(&csv);
    free(filename);
    fclose(file);
    return 0;
}
