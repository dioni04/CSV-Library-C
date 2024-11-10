#include "io.h"
#include "filter.h"
#include "list.h"
#include "statistic.h"
#include "comp.h"

int csvLoad(FILE* file, DATA* csv){
    char line[MAX];
    char* copy;
    char* freeAux;//ponteiro para free
    char* token;
    unsigned int i = 0;
    unsigned int j = 0;
    NaN* new = NULL;

    //loop de load
    while(fgets(line, MAX, file) != NULL){
        j = 0;

        //copia de line para strsep
        copy = strdup(line);
        if(!copy){
            printf("Error Alloc Load CSV\n");
            return 1;
        }
        freeAux = copy;
        token = copy; //para entrar no loop primeira vez

        //loop de load das colunas
        while (1){
            token = strsep(&copy,",");
            if (token == NULL)
                break;
            csv->matrix[i][j] = strdup(token);
            if(!csv->matrix[i][j]){
                printf("Error Alloc Load CSV");
                return 1;
            }
            //se for NaN adiciona na lista de NaN
            if(strlen(csv->matrix[i][j]) == 0){
                new = initializeListNaN(i, j);
                insertListNaN(&csv->NaNSet, new);
            }
            j++;
        }
        i++;
        free(freeAux);
    }
    //pos processamento da ultima coluna da matriz
    for(unsigned int i = 0; i< csv->nRows; i++){
        token = strdup(csv->matrix[i][csv->nCol-1]);
        //coloca terminador da string
        csv->matrix[i][csv->nCol-1][strlen(token)-1] = '\0';
        free(token);
    }
    return 0;
}

void csvStore(DATA* csv, FILE* file){
    for(unsigned int i = 0; i < csv->nRows; i++){
        //copia linha e colunas habilitadas no arquivo
        if(!csv->disableR[i]){
            for(unsigned int j = 0; j < csv->nCol; j++){
                if(!csv->disableC[j]){
                    fputs(csv->matrix[i][j], file);
                    fputc(',', file);
                }
            }
        //adiciona newline no final
        fputs("\n", file);
        }
    }
    return;
}

int csvStoreWrapper(DATA* csv){
    char option[MAX];
    FILE* file;

    while(1){
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: ");
        scanf("%1s", option);
        //se sim cria arquivo e escreve csv habilitados
        if(!strcmp(option, "S") || !strcmp(option, "s")){
            printf("Entre com o nome do arquivo: ");
            scanf("%s", option);
            file = fopen(option,"w+");
            if(!file){
                printf("Falha ao abrir arquivo\n");
                return 1;
            }
            csvStore(csv, file);
            fclose(file);
            printf("Arquivo gravado com sucesso\n");
            break;
        }
        else if(!strcmp(option, "N") || !strcmp(option, "n"))
            break;
        else
            printf("\nOpção Inválida\n\n");
    }
    return 0;
}

void freeMatrix(char*** matrix, unsigned int nRows, unsigned int nCols){
    for(unsigned int i = 0; i < nRows; i++){
        for (unsigned int j = 0; j < nCols ; j++)
            free(matrix[i][j]);
        free(matrix[i]);
    }
    free(matrix);
    return;
}

void freeCSV(DATA* csv){
    freeMatrix(csv->matrix, csv->nRows, csv->nCol);
    for(int j = 0; j < csv->nCol; j++)
        free(csv->type[j]);
    freeListNaN(csv->NaNSet);

    free(csv->disableR);
    free(csv->disableC);
    free(csv->sizeC);
    free(csv->type);
    return;
}

//conta quantas linhas
unsigned int count_rows(FILE* file){
    unsigned int count = 0;
    char line[MAX];
    while(fgets(line, MAX, file) != NULL)
        count++;
    return count;
}

//conta quantas colunas
unsigned int count_columns(FILE* file){
    unsigned int count = 0;
    char line[MAX];
    char* aux = NULL;

    if(fgets(line, MAX, file) != NULL){
        aux = strtok(line,",");
        while(1){
            if(aux != NULL)
                count++;
            else
                break;
            aux = strtok(NULL, ",");
        }
    }
    return count;
}

//defina tamanhos para formatacao
void countSize(DATA* csv){
    unsigned int maior;
    unsigned int size = 0;
    unsigned int colSize;
    unsigned int i;
    unsigned int j;
    unsigned int auxS = csv->nRows;

    for(j = 0; j < csv->nCol; j++){
        //primeiro da coluna
        maior = strlen(csv->matrix[0][j]);
        for(i = 1; i < csv->nRows; i++){
            //acha maior tamanho da coluna
            colSize = strlen(csv->matrix[i][j]);
            if(colSize > maior)
                maior = colSize;
        }
        //coloca maior valor a coluna correspondente
        csv->sizeC[j] = maior;
    }
    //define tamanho de nRows
    if(auxS == 0){
        csv->sizeR = 1;
        return;
    }
    while(auxS != 0){
        auxS = auxS/10;
        size++;
    }
    csv->sizeR = size;
    return;
}

void enterToCont(){
    char c = 'a';

    getchar();
    printf("\nPressione ENTER para continuar");
    while(c != '\n')
        c = getchar();
    printf("\n");
    return;
}

//funcao para salvar ou nao dados apos mudancas
void deleteOldData(DATA* csv, unsigned short* copy, unsigned int nRowsActive){
    char option[4];

    while(1){
        printf("Deseja descartar os dados originais? [S|N]: ");
        scanf("%1s", option);

        if(!strcmp(option, "n") || !strcmp(option, "N")){
            for(unsigned int i = 0; i < csv->nRows; i++)
                csv->disableR[i] = copy[i];
            csv->nRowsActive = nRowsActive;//recebe numero de ativos antigo
            break;
        }
        else if(!strcmp(option, "S") || !strcmp(option, "s"))
            break;
        else
            printf("\nOpção Inválida\n\n");
    }
    return;

}

//copia linha da matriz
void copyRow(char*** matrix, unsigned int previous, unsigned int iAux, DATA* csv,
            unsigned short* disableAux, unsigned short disablePrevious
){
    NaN* aux;

    for(unsigned int j = 0; j < csv->nCol; j++){
        //copia posicao anterior para nova linha
        matrix[iAux][j] = strdup(csv->matrix[previous][j]);
        //se for um NaN
        if(strlen(csv->matrix[previous][j]) == 0){
            //se posicao anterior era NaN troca na lista de NaN
            aux = onListNaN(csv->NaNSet, previous, j);
            if(aux)
                aux->row = iAux;
        }
    }
    disableAux[iAux] = disablePrevious;//recebe estado de disable anterior

    return;
}

void printRow(DATA* csv, int i){
    //para cabecario nao ter numero
    if(i > 0)
        printf("%*d ", csv->sizeR,i-1);

    else{
        //espaco vazio no cabecalho
        printf("%*s ", csv->sizeR, " ");
    }
    for(unsigned int j = 0; j < csv->nCol; j++){
        //caso seja NaN
        if(!csv->disableC[j]){
            if(strlen(csv->matrix[i][j]) == 0){
                printf("%*s ", csv->sizeC[j], "NaN");
                continue;
            }
            //printa formatado
            printf("%*s ", csv->sizeC[j], csv->matrix[i][j]);
        }
    }
    printf("\n");
    return;
}

//procura variavel e retorna indice se houver
int searchVar(DATA* csv, char* name){
    for(unsigned int j = 0; j < csv->nCol; j++)
        if(!strcmp(csv->matrix[0][j],name))
            return j;
    return -1;
}

//define tipos das variaveis
void define(DATA* csv){
    char c;
    float num;
    //int size = 0;
    //unsigned int auxS = csv->nRows;

    for(unsigned int j = 0; j < csv->nCol; j++){
        c = 'N';
        for(unsigned int i = 1; i < csv->nRows; i++){
            //caso seja NaN vai para proxima iteracao
            num = 0.0;
            if(strlen(csv->matrix[i][j]) == 0)
                continue;
            //se é string troca c para S
            num = strtof(csv->matrix[i][j], NULL);
            if(num == 0.0){
                c = 'S';
                break;
            }
        }
        if(c == 'N')
            csv->type[j] = strdup("N");
        else
            csv->type[j] = strdup("S");
    }
    return;
}

//funcao do sumario
void summary(DATA* csv){

    for(unsigned int j = 0; j < csv->nCol; j++){
        printf("%s ", csv->matrix[0][j]);
        printf("[%s]\n", csv->type[j]);
    }
    printf("%u variaveis encontradas\n", csv->nCol);
    enterToCont();
    return;
}

void show(DATA* csv){
    unsigned int i = 0;
    short count = 0;

    printf("\n");

    //se numLin < 10 imprime tudo
    if(csv->nRows < 10){
        while(i < csv->nRows){
            if(!csv->disableR[i])
                printRow(csv, i);
            i++;
        }
    }
    //se nao imprime 10 das pontas do csv
    else{
        //6 para imprimir o cabecalho mais 5 entradas
        while(i < csv->nRows && count < 6){
            if(!csv->disableR[i]){
                printRow(csv, i);
                count++;
            }
            i++;
        }
        if(i >= csv->nRows){//chegou no final
            printf("[%d rows x %d columns]\n", csv->nRowsActive - 1, csv->nColActive);
            return;
        }
        //imprime linha de pontos
        printf("%*s", csv->sizeR, "...");//para numero da linha
        for(unsigned int j = 0; j < csv->nCol; j++)
            if(!csv->disableC[j])
                printf("%*s", csv->sizeC[j]+1, "...");//para dados do csv

        printf("\n");

        //comeca do final e vai subindo ate achar 5 linhas nao desabilitadas
        count = 0;
        i = csv->nRows-1;
        while(count < 5){
            if(!csv->disableR[i])
                count++;
            i--;
        }
        i++;
        while(count > 0){
            if(!csv->disableR[i]){
                printRow(csv, i);
                count--;
            }
            i++;
        }
    }
    //printa com -1 para nao ser representado como vetor de 0
    printf("\n[%d rows x %d columns]\n", csv->nRowsActive - 1, csv->nColActive);
    return;
}

//Wrapper para nao precisar de ENTER toda vez
void showWrapper(DATA* csv){
    show(csv);
    enterToCont();
}

//funcao de filtro
int filter(DATA* csv){
    unsigned int j;
    unsigned int nRowsActiveCopy = csv->nRowsActive;//variavel para salvar num de linhas ativas
    unsigned short* disableCopy = NULL;
    short op;
    char filter[3];
    char name[MAX];
    char value[MAX];
    void (*functions[6]) (DATA*, unsigned int, char*) = {eq, neq, lt, lte, gt, gte}; //vetor de funcao

    csv->nRowsActive = csv->nRows;//para casos de borda que fazem contador negativo

    //copia do disable
    disableCopy = (unsigned short*)malloc(csv->nRows * sizeof(unsigned short));
    for(unsigned i = 0; i < csv->nRows; i++)
        disableCopy[i] = csv->disableR[i];

    while(1){
        printf("Entre com a variavel: ");
        getchar();
        scanf("%[^\n]", name);
        j = searchVar(csv, name);
        if(j == -1){
            printf("Variavel Inválida\n");
            continue;
        }
        break;
    }

    while(1){
        printf("Escolha um filtro ( == > >= < <= != ) : ");
        scanf("%2s", filter);
        //checa se filtro e valido
        if(!strcmp(filter, "=="))
            op = 0;
        else if (!strcmp(filter, "!="))
            op = 1;
        else if (!strcmp(filter, "<"))
            op = 2;
        else if (!strcmp(filter, "<="))
            op = 3;
        else if (!strcmp(filter, ">"))
            op = 4;
        else if (!strcmp(filter, ">="))
            op = 5;
        else{
            printf("Filtro Inválido\n");
            continue;
        }
        break;
    }

    printf("Digite um valor: ");
    getchar();
    scanf("%[^\n]", value);
    getchar();

    //chama funcao em pos op no vetor
    functions[op](csv, j, value);

    show(csv);

    //wrapper da funcao para se salvar dados
    csvStoreWrapper(csv);

    deleteOldData(csv, disableCopy, nRowsActiveCopy);
    free(disableCopy);

    enterToCont();
    return 0;
}

int descriptionN(DATA* csv, unsigned int j){
    char** arrayS = NULL;
    float* array = NULL;
    NODE* list = NULL;
    NODE* mode = NULL;

    unsigned int size = 0;//indice para vetor

    //alocacao vetor de string para moda
    arrayS = (char**)malloc(sizeof(char*) * csv->nRows);
    if(!arrayS){
        printf("Allocation Error string array");
        return 1;
    }

    //copia elementos nao NaN para vetor
    for(unsigned int i = 1; i < csv->nRows; i++)
        if(!csv->disableR[i] && (strlen(csv->matrix[i][j]) != 0)){
            arrayS[size] = csv->matrix[i][j];
            size++;
        }

    size = 0;

    //alocacao array de float caso numerico
    array = (float*)malloc(sizeof(float) * csv->nRows);
    if(!array){
        printf("Allocation Error float array");
        return 1;
    }
    //copia elementos nao NaN para vetor
    for(unsigned int i = 1; i < csv->nRows; i++)
        if(!csv->disableR[i] && (strlen(csv->matrix[i][j]) != 0)){
            array[size] = strtof(csv->matrix[i][j], NULL);
            size++;
        }
    //ordena vetor
    qsort(array, size, sizeof(float), compFloat);

    printf("Contador: %u\n", size);
    printf("Média: %.1f\n", aritMean(array, size));
    printf("Desvio: %.1f\n", stdDeviation(array, aritMean(array, size),size));
    printf("Mediana: %.1f\n", median(array, size));

    //moda
    mode = modeS(arrayS, &list, size);
    if(mode)
        printf("Moda: %s %u vezes\n",mode->string, mode->count);
    else
        printf("Não há moda\n");

    printf("Min: %.1f\n", min(array, size));
    printf("Max: %.1f\n", max(array, size));
    uniqueValuesN(list);

    free(array);
    free(arrayS);
    freeList(list);
    return 0;
}

int descriptionS(DATA* csv, unsigned int j){
    char** array;
    NODE* mode = NULL;
    NODE* list = NULL;

    unsigned int size = 0;//indice para vetor

    array = (char**)malloc(sizeof(char*) * csv->nRows);
    if(!array){
        printf("Allocation Error string array");
        return 1;
    }

    //copia elementos nao NaN para vetor
    for(unsigned int i = 1; i < csv->nRows; i++)
        if(!csv->disableR[i] && (strlen(csv->matrix[i][j]) != 0)){
            array[size] = csv->matrix[i][j];
            size++;
        }

    printf("Contador: %u\n", size);

    mode = modeS(array, &list, size);
    if(mode)
        printf("Moda: %s %u vezes\n", mode->string, mode->count);
    else
        printf("Não há moda\n");
    uniqueValuesS(list);

    free(array);
    freeList(list);
    return 0;
}

//wrapper de descricao para tipos N e S
int descriptionWrapper(DATA* csv){
    unsigned int j;
    char name[MAX];

    //caso nao haja dados
    if(csv->nRowsActive <= 1){
        printf("Não há dados\n");
        return 0;
    }

    while(1){
        getchar();
        printf("Entre com a variavel: ");
        scanf("%[^\n]", name);
        j = searchVar(csv, name);
        if(j == -1){
            printf("Variavel Inválida\n");
            continue;
        }
        break;
    }
    //caso seja uma variavel numerica
    if(IS_NUM(csv->type[j]))
        descriptionN(csv, j);
    //caso variavel seja string
    else
        descriptionS(csv, j);

    enterToCont();
    return 0;
}

//funcao de ordenacao
void sort(DATA* csv){
    char*** matrixAux = NULL;
    char option;
    char variable[MAX];

    unsigned short control = 1;
    unsigned short* disableAux = NULL;
    unsigned int j;
    unsigned int iAux = 1;

    SORT_AUX* array = NULL;//vetor de struct auxiliar para sort

    //aloca matriz auxiliar
    matrixAux = (char***)malloc(sizeof(char*) * csv->nRows);
    if(!matrixAux){
        printf("Allocation Error matrixAux\n");
        return;
    }
    for(unsigned int i = 0; i < csv->nRows; i++){
        matrixAux[i] = (char**)malloc(sizeof(char*) * csv->nCol);
        if(!matrixAux[i]){
            printf("Allocation Error matrixAux\n");
            return;
        }
    }

    //vetor para troca de disable
    disableAux = (unsigned short*)calloc(csv->nRows, sizeof(unsigned short));
    if(!disableAux){
        printf("Allocation Error disableAux\n");
        return;
    }

    //vetor para sort
    array = (SORT_AUX*)malloc(sizeof(SORT_AUX) * csv->nRows);
    if(!array){
        printf("Allocation Error matrixAux\n");
        return;
    }

    while(1){
        getchar();
        printf("Entre com a variavel: ");
        scanf("%[^\n]", variable);

        j = searchVar(csv, variable);

        if(j == -1){
            printf("Variavel Inválida\n");
            continue;
        }
        break;
    }

    //copia para vetor
    for(unsigned int i = 0; i < csv->nRows - 1; i++, iAux++){
        array[i].previousRow = iAux;
        array[i].string = strdup(csv->matrix[iAux][j]);
        if(!array[i].string){
            printf("Allocation Error matrixAux\n");
            return;
        }
    }

    while(control){
        printf("Selecione uma opcao [A]scendente ou [D]escrescente: ");
        getchar();
        scanf("%c", &option);
        //ordena vetor com qsort
        switch (option) {
            case ('A'):
                if(IS_NUM(csv->type[j]))
                    qsort(array, csv->nRows-1, sizeof(SORT_AUX),compFloatA);//numerico A

                else
                    qsort(array, csv->nRows-1, sizeof(SORT_AUX), compStringA);//string A
                control = 0;
                break;
            case 'D':
                if(IS_NUM(csv->type[j]))
                    qsort(array, csv->nRows-1, sizeof(SORT_AUX),compFloatD);//numerico D
                else
                    qsort(array, csv->nRows-1, sizeof(SORT_AUX), compStringD);//string D
                control = 0;
                break;
            default:
                printf("\nOpção Inválida\n\n");
        }
    }
    iAux = 1;

    //copia cabecalho
    copyRow(matrixAux, 0, 0, csv, disableAux, 0);
    //copia linhas na matriz baseado no vetor ordenado
    for(unsigned int i = 0; i < csv->nRows-1; i++, iAux++){
            copyRow(matrixAux, array[i].previousRow, iAux, csv, disableAux, csv->disableR[array[i].previousRow]);
    }

    while(1){
        //se quiser trocar troca matriz pela auxiliar
        printf("Deseja descartar os dados originais? [S|N]: ");
        getchar();
        scanf("%c", &option);
        if(option == 'S'|| option == 's'){
            freeMatrix(csv->matrix, csv->nRows, csv->nCol);
            csv->matrix = matrixAux;
            free(csv->disableR);
            csv->disableR = disableAux;
            break;
        }
        else if(option == 'N' || option == 'n'){
            freeMatrix(matrixAux, csv->nRows, csv->nCol);
            free(disableAux);
            break;
        }
        else
            printf("\nOpção Inválida\n\n");
    }
    //free vetor de struct
    for(unsigned int i = 0; i < csv->nRows-1; i++)
        free(array[i].string);
    free(array);

    enterToCont();
    return;
}

//funcao de selecao
void selection(DATA* csv){
    unsigned int j;
    char variables[MAX];
    char* token;

    //desabilita todos
    for(j = 0; j < csv->nCol; j++)
        csv->disableC[j] = 1;
    csv->nColActive = 0;
    //le tokens possiveis e desabilita coluna depois
    getchar();
    printf("Entre com a variaveis que deseja selecionar (separadas por espaço): ");
    scanf("%[^\n]", variables);
    token = strtok(variables," ");

    j = searchVar(csv, token);
    if(j != -1){
        csv->disableC[j] = 0;
        csv->nColActive++;
    }

    while(1){
        token = strtok(NULL," ");
        if(!token)
            break;
        j = searchVar(csv, token);
        if(j != -1){
            csv->disableC[j] = 0;
            csv->nColActive++;
        }
    }
    getchar();
    show(csv);
    csvStoreWrapper(csv);

    //reabilita todos
    for(j = 0; j < csv->nCol; j++)
        csv->disableC[j] = 0;
    csv->nColActive = csv->nCol;

    enterToCont();
    return;
}

//funcao de tratamento de dados NaN
int missingData(DATA* csv){
    NaN* aux;
    NaN* auxNext;
    float* array = NULL;
    float* means = NULL;

    unsigned short* disableCopy = NULL;
    unsigned int nRowsActiveCopy = csv->nRowsActive;
    unsigned int size;

    char option;
    char stringAux[MAX];
    char* temp;

    while(1){
        printf("1) Listar registros com NaN\n2) Substituir pela media\n3) Substituir pelo proximo valor valido\n4) Remover registros com NaN\n5) Voltar ao menu principal\n");
        printf("Opção: ");
        scanf(" %c", &option);
        switch (option){
            case '1'://printar
                if(!csv->NaNSet){
                    printf("\nNão há dados NaN\n");
                    enterToCont();
                    break;
                }
                //copia do disable
                disableCopy = (unsigned short*)malloc(sizeof(unsigned short) * csv->nRows);
                for(unsigned i = 0; i < csv->nRows; i++)
                    disableCopy[i] = csv->disableR[i];

                //desabilita todos
                csv->nRowsActive = 0;
                for(unsigned int i = 1; i < csv->nRows; i++)
                    csv->disableR[i] = 1;

                aux = csv->NaNSet;
                while(aux){
                    //re-habilita linha de NaN
                    csv->disableR[aux->row] = 0;
                    aux = aux->next;
                    csv->nRowsActive++;
                }
                show(csv);
                csvStoreWrapper(csv);
                printf("\n");
                getchar();
                deleteOldData(csv, disableCopy, nRowsActiveCopy);
                enterToCont();
                break;
            case '2'://media
                if(!csv->NaNSet){
                    printf("\nNão há dados NaN\n");
                    enterToCont();
                    break;
                }

                //alocacao de vetores para operacoes
                array = (float*)malloc(sizeof(float) * csv->nRows);
                if(!array){
                    printf("Alloc Fail float array");
                    return 1;
                }
                means = (float*)malloc(sizeof(float) * csv->nCol);
                if(!means){
                    printf("Alloc Fail means array");
                    return 1;
                }

                //definicao das medias a priori
                for(unsigned int j = 0; j < csv->nCol; j++){
                    //se for numero carrega vetor e calcula media
                    if(IS_NUM(csv->type[j])){
                        size = 0;
                        for(unsigned int i = 1; i < csv->nRows; i++){
                            if(!csv->disableR[i] && (strlen(csv->matrix[i][j]) != 0)){
                                array[size] = strtof(csv->matrix[i][j], NULL);
                                size++;
                            }
                        }
                        means[j] = aritMean(array, size);
                    }
                }
                aux = csv->NaNSet;
                while(aux){
                    auxNext = aux->next;
                    //se for variavel numerica coloca media
                    if(IS_NUM(csv->type[aux->col])){
                        //copia media para string aux
                        sprintf(stringAux, "%.1f", means[aux->col]);
                        //copia para temp
                        temp = strdup(stringAux);
                        if(!temp){
                            printf("Alloc Fail Temp");
                            return 1;
                        }
                        free(csv->matrix[aux->row][aux->col]);
                        csv->matrix[aux->row][aux->col] = temp;
                        //remove nodo da lista
                        removeListNaN(&csv->NaNSet, aux);
                    }
                    aux = auxNext;
                }
                free(array);
                free(means);
                csvStoreWrapper(csv);
                enterToCont();
                break;
            case '3':
                if(!csv->NaNSet){
                    printf("\nNão há dados NaN\n");
                    enterToCont();
                    break;
                }
                aux = csv->NaNSet;
                while(aux){
                    auxNext = aux->next;
                    //procura valor valido na coluna e duplica string
                    for(unsigned int i = (aux->row + 1); i < csv->nRows; i++){
                        //se for valido da free e copia no ponteiro
                        if(strlen(csv->matrix[i][aux->col]) != 0){
                            temp = strdup(csv->matrix[i][aux->col]);
                            if(!temp){
                                printf("Alloc Fail Temp");
                                return 1;
                            }
                            free(csv->matrix[aux->row][aux->col]);
                            //posica da matriz recebe nova string
                            csv->matrix[aux->row][aux->col] = temp;
                            //remove lista
                            removeListNaN(&csv->NaNSet, aux);
                            break;
                        }
                    }
                    aux = auxNext;
                }
                csvStoreWrapper(csv);
                enterToCont();
                break;
            case '4': //desabilita linhas com NaN
                if(!csv->NaNSet){
                    printf("\nNão há dados NaN\n");
                    enterToCont();
                    break;
                }
                aux = csv->NaNSet;
                while(aux){
                    csv->nRowsActive--;
                    csv->disableR[aux->row] = 1;
                    aux = aux->next;
                }
                csvStoreWrapper(csv);
                printf("\n");
                enterToCont();
                break;
            case '5':
                free(disableCopy);
                printf("\n");
                return 0;
            default:
                printf("\nOpção Inválida\n\n");
                getchar();
        };
    }
}
