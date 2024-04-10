#include "io.h"
#include "list.h"
#include "statistic.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

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
            perror("Error Alloc Load CSV");
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
                perror("Error Alloc Load CSV");
                return 1;
            }
            //se for NaN adiciona na lista de NaN
            if(strlen(csv->matrix[i][j]) == 0){
                new = initializeListNaN(csv->matrix[i][j], i, j);
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
    char* option;
    FILE* file;

    printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: ");
    scanf("%ms", &option);
    //se sim cria arquivo e escreve csv habilitados
    if(!strcmp(option, "S") || !strcmp(option, "s")){
        free(option);
        printf("Entre com o nome do arquivo: ");
        scanf("%ms", &option);
        file = fopen(option,"w+");
        if(!file){
            printf("Falha ao abrir arquivo\n");
            return 1;
        }
        csvStore(csv, file);
        fclose(file);
        printf("Arquivo gravado com sucesso\n");
    }
    free(option);
    return 0;
}

void freeCSV(DATA* csv){
    for(int i = 0; i < csv->nRows; i++){
        for (int j = 0; j < csv->nCol ; j++)
            free(csv->matrix[i][j]);
        free(csv->matrix[i]);
    }
    for(int j = 0; j < csv->nCol; j++)
        free(csv->type[j]);
    freeListNaN(csv->NaNSet);
    free(csv->matrix);
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
    unsigned int size;
    unsigned int i;
    unsigned int j;
    unsigned int auxS = csv->nRows;

    for(j = 0; j < csv->nCol; j++){
        //primeiro da coluna
        maior = strlen(csv->matrix[0][j]);
        for(i = 1; i < csv->nRows; i++){
            //acha maior tamanho da coluna
            size = strlen(csv->matrix[i][j]);
            if(size > maior)
                maior = size;
        }
        //coloca maior valor a coluna correspondente
        csv->sizeC[j] = maior;
    }
    //define tamanho de nRows
    while(auxS != 0){
        auxS = auxS/10;
        size++;
    }
    csv->sizeR = size;
    return;
}

void printLinha(DATA* csv, int i){
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

int searchVar(DATA* csv, char* name){
    for(unsigned int j = 0; j < csv->nCol; j++)
        if(!strcmp(csv->matrix[0][j],name))
            return j;
    return -1;
}

void enterToCont(){
    char c = 'a';

    printf("Pressione ENTER para continuar");
    while(c != '\n')
        c = getchar();
    printf("\n");
    return;
}

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
    /*
    //defini tamanho de nRows
    while(auxS != 0){
        auxS = auxS/10;
        size++;
    }
    csv->sizeR = size;

    for(unsigned int j = 0; j < csv->nCol; j++){
        printf("%s ", csv->matrix[0][j]);
        printf("[%s]\n", csv->type[j]);
    }
    printf("%u variaveis encontradas\n", csv->nCol);

    //getchar para enter
    //getchar();//limpa buffer
    enterToCont();
    return;
    */
    return;
}

void summary(DATA* csv){

    for(unsigned int j = 0; j < csv->nCol; j++){
        printf("%s ", csv->matrix[0][j]);
        printf("[%s]\n", csv->type[j]);
    }
    printf("%u variaveis encontradas\n", csv->nCol);

    getchar();
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
                printLinha(csv, i);
            i++;
        }
    }
    //se nao imprime 10 das pontas do csv
    else{
        //6 para imprimir o cabecalho mais 5 entradas
        while(count < 6){
            if(!csv->disableR[i]){
                printLinha(csv, i);
                count++;
            }
            i++;
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
                printLinha(csv, i);
                count--;
            }
            i++;
        }
    }
    //printa com -1 para nao ser representado como vetor de 0
    printf("[%d rows x %d columns]\n", csv->nRowsActive - 1, csv->nColActive);
    return;
}

//Wrapper para nao precisar de ENTER toda vez
void showWrapper(DATA* csv){
    show(csv);
    getchar();
    enterToCont();
}

void opFilterNumeric(DATA* csv, unsigned int j, short op, char* variable){
    float numVar = strtof(variable, NULL);
    float numMat;

    switch (op) {
        case 1:
            for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é == desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat == numVar)){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            }
            break;
        case 2:
            for(unsigned int i = 1; i < csv->nRows; i++){
             //se nao é != desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat != numVar)){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            }
            break;
        case 3:
            for(unsigned int i = 1; i < csv->nRows; i++){
             //se nao é < desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat < numVar)){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            }
            break;
        case 4:
            for(unsigned int i = 1; i < csv->nRows; i++){
             //se nao é <= desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat <= numVar)){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            }
            break;
        case 5:
            for(unsigned int i = 1; i < csv->nRows; i++){
             //se nao é > desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat > numVar)){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            }
            break;
        case 6:
            for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é >= desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat >= numVar)){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            }
    }
    return;
}

void opFilterString(DATA* csv, unsigned int j, short op, char* variable){
    switch (op) {
        case 1:
            for(unsigned int i = 1; i < csv->nRows; i++)
            //se nao é == desabilita
                if(strcmp(csv->matrix[i][j],variable)){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            break;
        case 2:
            for(unsigned int i = 1; i < csv->nRows; i++)
             //se nao é != desabilita
                if(!strcmp(csv->matrix[i][j],variable)){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            break;
        case 3:
            for(unsigned int i = 1; i < csv->nRows; i++)
             //se nao é < desabilita
                if(strcmp(csv->matrix[i][j],variable) >= 0){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            break;
        case 4:
            for(unsigned int i = 1; i < csv->nRows; i++)
             //se nao é <= desabilita
                if(strcmp(csv->matrix[i][j],variable) > 0){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            break;
        case 5:
            for(unsigned int i = 1; i < csv->nRows; i++)
             //se nao é > desabilita
                if(strcmp(csv->matrix[i][j],variable) <= 0){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
            break;
        case 6:
            for(unsigned int i = 1; i < csv->nRows; i++)
            //se nao é >= desabilita
                if(strcmp(csv->matrix[i][j],variable) < 0){
                    csv->nRowsActive--;
                    csv->disableR[i] = 1;
                }
        return;
    }
}

int filter(DATA* csv){
    unsigned int j;
    short op;
    char option[2];
    char filter[3];
    char name[MAX];
    char value[MAX];

    getchar();
    printf("Entre com a variavel: ");
    scanf("%[^\n]", name);
    getchar();
    j = searchVar(csv, name);
    if(j == -1){
        printf("Variavel Inválida\n");
        return 1;
    }

    printf("Escolha um filtro ( == > >= < <= != ) : ");
    scanf("%2s", filter);
     //TROCAR ISSO POR PONTEIRO PRA FUNCOES
    //checa se filter e valido
    if(!strcmp(filter, "=="))
        op = 1;
    else if (!strcmp(filter, "!="))
        op = 2;
    else if (!strcmp(filter, "<"))
        op = 3;
    else if (!strcmp(filter, "<="))
        op = 4;
    else if (!strcmp(filter, ">"))
        op = 5;
    else if (!strcmp(filter, ">="))
        op = 6;
    else{
        printf("Filtro incorreto \n");
        return 1;
    }

    printf("Digite um valor: ");
    getchar();
    scanf("%[^\n]", value);
    getchar();

    //chama funcao dependo do tipo de dado
    if(IS_NUM(csv->type[j]))
        opFilterNumeric(csv, j, op, value);
    else
        opFilterString(csv, j, op, value);

    show(csv);

    //wrapper da funcao para se salvar dados
    csvStoreWrapper(csv);

    printf("Deseja descartar os dados originais? [S|N]: ");
    scanf("%1s", option);

    //TALVEZ TENHA Q MUDAR ISSO POIS NAO FUNCIONA TODOS OS CASOS
    //se nao coloca 0 em todo vetor disable
    if(!strcmp(option, "N") || !strcmp(option, "n"))
        for(unsigned int i = 0; i < csv->nRows; i++)
            csv->disableR[i] = 0;

    getchar();
    enterToCont();
    return 0;
}

int compFloat(const void* a, const void* b){
    float* numA = (float*)a;
    float* numB = (float*)b;

    if (*numA < *numB)
        return -1;
    else if (*numA > *numB)
        return 1;
    return 0;
}

int descriptionN(DATA* csv, unsigned int j){
    float* array = NULL;
    NODE* list = NULL;
    NODE* mode = NULL;

    unsigned int size = 0;//indice para vetor

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

    mode = modeN(array, &list, size);
    if(mode)
        printf("Moda: %.1f %u vezes\n",mode->num, mode->count);
    else
        printf("Não há moda\n");

    printf("Min: %.1f\n", min(array, size));
    printf("Max: %.1f\n", max(array, size));
    uniqueValuesN(list);

    free(array);
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

int descriptionWrapper(DATA* csv){
    unsigned int j;
    char name[MAX];

    getchar();
    printf("Entre com a variavel: ");
    scanf("%[^\n]", name);
    j = searchVar(csv, name);
    if(j == -1){
        printf("Variavel Inválida\n");
        return 1;
    }
    //caso seja uma variavel numerica
    if(IS_NUM(csv->type[j]))
        descriptionN(csv, j);
    //caso variavel seja string
    else
        descriptionS(csv, j);

    getchar();
    enterToCont();
    return 0;
}

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

    //habilita todos
    for(j = 0; j < csv->nCol; j++)
        csv->disableC[j] = 0;
    csv->nColActive = csv->nCol;

    getchar();
    enterToCont();
    return;
}

int missingData(DATA* csv){
    NaN* aux;
    float* array = NULL;

    unsigned int size;
    int option;

    char stringAux[MAX];
    char* temp;

    while(1){
        printf("1) Listar registros com NaN\n2) Substituir pela media\n3) Substituir pelo proximo valor valido\n4) Remover registros com NaN\n5) Voltar ao menu principal\n");
        printf("Opção: ");
        scanf("%d", &option);
        switch (option){
            case 1://printar
                //desabilita todos
                for(unsigned int i = 1; i < csv->nRows; i++)
                    csv->disableR[i] = 1;
                //printLinha(csv, 0);//primeira linha
                aux = csv->NaNSet;
                while(aux){
                    //re-habilita linha de NaN
                    csv->disableR[csv->NaNSet->row] = 0;
                    aux = aux->next;
                }
                show(csv);
                csvStoreWrapper(csv);
                break;
            case 2://media
                array = (float*)malloc(sizeof(float) * csv->nRows);
                while(aux){
                    size = 0;
                    //se for variavel numerica coloca media
                    if(IS_NUM(csv->type[aux->col])){
                        //copia para vetor de float para media
                        for(unsigned int i = 1; i < csv->nRows; i++)
                            if(!csv->disableR[i] && (strlen(csv->matrix[i][aux->col]) != 0)){
                                array[size] = strtof(csv->matrix[i][aux->col], NULL);
                                size++;
                        }
                    }
                    //copia media para string na matriz
                    sprintf(stringAux, "%.1f", aritMean(array, size));
                    //realoca memoria do ponteiro para media
                    temp = strdup(stringAux);
                    if(!temp){
                        printf("Alloc Fail Temp");
                        return 1;
                    }
                    free(*aux->adress);
                    *aux->adress = temp;
                    //remove nodo da lista
                    removeListNaN(&csv->NaNSet, aux);
                    aux = aux->next;
                }
                free(array);
                csvStoreWrapper(csv);
                break;
            case 3:
                //procura valor valido na coluna e duplica string
                for(unsigned int i = aux->row + 1; i < csv->nCol; i++){
                    //se fr valido da free e copia no ponteiro
                    if(strlen(csv->matrix[i][aux->col]) != 0){
                        free(*aux->adress);
                        temp = strdup(csv->matrix[i][aux->col]);
                        if(!temp){
                            printf("Alloc Fail Temp");
                            return 1;
                        }
                        *aux->adress = temp;
                        //remove lista
                        removeListNaN(&csv->NaNSet, aux);
                        break;
                    }
                }
                csvStoreWrapper(csv);
                break;
            case 4: //desabilita linhas com NaN
                while(aux){
                    csv->disableR[aux->row] = 1;
                    aux = aux->next;
                }
                csvStoreWrapper(csv);
                break;
            case 5:
                show(csv);
                return 0;
            default:
                printf("Opção Inválida");
        };
    }
}
