#include "io.h"

int main(int argc, char **argv){
    FILE* file = NULL;
    DATA csv;

    char* filename = NULL;
    char option;

    //Abre a arquivo se houver argumento
    if(argc == 1){
        printf("Faltou argumento de arquivo .csv\n");
        return 1;
    }
    filename = strdup(argv[1]);

    file = fopen(filename, "r");
    if(!file){
        printf("Error file open\n");
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
    csv.disableR = (unsigned short*)calloc(csv.nRows + 1, sizeof(unsigned short));
    if (!csv.disableR){
        printf("Allocation Error enable\n");
        return 1;
    }

        //alocacao vetor de enable coluna Enabled = 0 / Disabled = 1
    csv.disableC = (unsigned short*)calloc(csv.nCol + 1, sizeof(unsigned short));
    if (!csv.disableC){
        printf("Allocation Error enable\n");
        return 1;
    }

    //alocacao vetor de maior tamanho coluna
    csv.sizeC = (unsigned int*)malloc(sizeof(unsigned int) * csv.nCol);
    if (!csv.sizeC){
        printf("Allocation Error sizeC\n");
        return 1;
    }

    //alocacao vetor de tipo
    csv.type = (char**)malloc(sizeof(char*) * csv.nCol);
    if(!csv.type){
        printf("Allocation Error type\n");
        return 1;
    }

    //alocacao da matriz
    csv.matrix = (char***)malloc(sizeof(char*) * csv.nRows);
    if(!csv.matrix){
        printf("Allocation Error matrix\n");
        return 1;
    }
    for(int i = 0; i < csv.nRows; i++){
        csv.matrix[i] = (char**)malloc(sizeof(char*) * csv.nCol);
        if(!csv.matrix[i]){
             printf("Allocation Error matrix\n");
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
        scanf(" %c", &option);
        switch (option) {
            case '1':
                summary(&csv);
                break;
            case '2':
                showWrapper(&csv);
                break;
            case '3':
                filter(&csv);
                break;
            case '4':
                descriptionWrapper(&csv);
                break;
            case '5':
                sort(&csv);
                break;
            case '6':
                selection(&csv);
                break;
            case '7':
                missingData(&csv);
                break;
            case '8':
                csvStoreWrapper(&csv);
                enterToCont();
                break;
            case '9'://free da memoria
                freeCSV(&csv);
                free(filename);
                fclose(file);
                return 0;
            default:
                printf("\nOpção Inválida\n\n");
        };
    }
    return 0;
}
