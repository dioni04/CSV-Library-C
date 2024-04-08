#include "io.h"
#include "statistic.h"

int compFloat(const void* a, const void* b){
    float* numA = (float*)a;
    float* numB = (float*)b;

    if(*numA < *numB)
        return -1;
    else if(*numA > *numB)
        return 1;
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
    free(csv->matrix);
    free(csv->disable);
    free(csv->sizeC);
    free(csv->type);
    return;
}

//conta quantas linhas
unsigned int count_rows(FILE* file){
    unsigned int count = 0;
    char line[SIZE];
    while(fgets(line, SIZE, file) != NULL)
        count++;
    return count;
}

//conta quantas colunas
unsigned int count_columns(FILE* file){
    unsigned int count = 0;
    char line[SIZE];
    char* aux = NULL;

    if(fgets(line, SIZE, file) != NULL){
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

void countSize(DATA* csv){
    unsigned int maior;
    unsigned int size;
    unsigned int i;
    unsigned int j;

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
        if(strlen(csv->matrix[i][j]) == 0){
            printf("%*s ", csv->sizeC[j], "NaN");
            continue;
        }
        //printa formatado
        printf("%*s ", csv->sizeC[j], csv->matrix[i][j]);
    }
    printf("\n");
    return;
}

void definir(DATA* csv){
    char c;
    float num;
    int size = 0;
    int auxS = csv->nRows;

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
    printf("\nPressione ENTER para continuar");

    //getchar para enter
    //getchar();//limpa buffer
    while(c != '\n')
        c = getchar();
    printf("\n");
    return;
}

void mostrar(DATA* csv){
    unsigned int i = 0;
    short count = 0;
    char c = 'a';

    printf("\n");

    //se numLin < 10 imprime tudo
    if(csv->nRows < 10){
        while(i < csv->nRows){
            if(!csv->disable[i])
                printLinha(csv, i);
            i++;
        }
    }
    //se nao imprime 10 das pontas do csv
    else{
        //6 para imprimir o cabecalho mais 5 entradas
        while(count < 6){
            if(!csv->disable[i]){
                printLinha(csv, i);
                count++;
            }
            i++;
        }
        //imprime linha de pontos
        printf("%*s", csv->sizeR, "...");//para numero da linha
        for(unsigned int j = 0; j < csv->nCol; j++)
            printf("%*s", csv->sizeC[j]+1, "...");//para dados do csv

        printf("\n");

        //comeca do final e vai subindo ate achar 5 linhas nao desabilitadas
        count = 0;
        i = csv->nRows-1;
        while(count < 5){
            if(!csv->disable[i])
                count++;
            i--;
        }
        while(count != 0){
            if(!csv->disable[i]){
                printLinha(csv, i);
                count--;
            }
            i++;
        }
    }
    //printa com -1 para nao ser representado como vetor de 0
    printf("[%d rows x %d columns]\n", csv->nRows - 1, csv->nCol);
    printf("\nPressione ENTER para continuar");

    //getchar para enter
    //getchar();//limpa buffer
    while(c != '\n')
        c = getchar();
    printf("\n");
    return;
}

void csvStore(DATA* csv, FILE* file){
    for(unsigned int i = 0; i < csv->nRows; i++){
        //copia linha habilitadas no arquivo
        if(!csv->disable[i]){
            for(unsigned int j = 0; j < csv->nCol; j++){
                fputs(csv->matrix[i][j], file);
                fputc(',', file);
            }
        //adiciona newline no final
        fputs("\n", file);
        }
    }
    return;
}

void opFilterNumeric(DATA* csv, unsigned int j, short op, char* variable){
    float numVar = strtof(variable, NULL);
    float numMat;

    switch (op) {
        case 1:
            for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é == desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat == numVar))
                    csv->disable[i] = 1;
            }
            break;
        case 2:
            for(unsigned int i = 1; i < csv->nRows; i++){
             //se nao é != desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat != numVar))
                    csv->disable[i] = 1;
            }
            break;
        case 3:
            for(unsigned int i = 1; i < csv->nRows; i++){
             //se nao é < desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat < numVar))
                    csv->disable[i] = 1;
            }
            break;
        case 4:
            for(unsigned int i = 1; i < csv->nRows; i++){
             //se nao é <= desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat <= numVar))
                    csv->disable[i] = 1;
            }
            break;
        case 5:
            for(unsigned int i = 1; i < csv->nRows; i++){
             //se nao é > desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat > numVar))
                    csv->disable[i] = 1;
            }
            break;
        case 6:
            for(unsigned int i = 1; i < csv->nRows; i++){
            //se nao é >= desabilita
                numMat = strtof(csv->matrix[i][j], NULL);
                if(!(numMat >= numVar))
                    csv->disable[i] = 1;
            }
    }
    return;
}

void opFilterString(DATA* csv, unsigned int j, short op, char* variable){
    switch (op) {
        case 1:
            for(unsigned int i = 1; i < csv->nRows; i++)
            //se nao é == desabilita
                if(strcmp(csv->matrix[i][j],variable))
                    csv->disable[i] = 1;
            break;
        case 2:
            for(unsigned int i = 1; i < csv->nRows; i++)
             //se nao é != desabilita
                if(!strcmp(csv->matrix[i][j],variable))
                    csv->disable[i] = 1;
            break;
        case 3:
            for(unsigned int i = 1; i < csv->nRows; i++)
             //se nao é < desabilita
                if(strcmp(csv->matrix[i][j],variable) >= 0)
                    csv->disable[i] = 1;
            break;
        case 4:
            for(unsigned int i = 1; i < csv->nRows; i++)
             //se nao é <= desabilita
                if(strcmp(csv->matrix[i][j],variable) > 0)
                    csv->disable[i] = 1;
            break;
        case 5:
            for(unsigned int i = 1; i < csv->nRows; i++)
             //se nao é > desabilita
                if(strcmp(csv->matrix[i][j],variable) <= 0)
                    csv->disable[i] = 1;
            break;
        case 6:
            for(unsigned int i = 1; i < csv->nRows; i++)
            //se nao é >= desabilita
                if(strcmp(csv->matrix[i][j],variable) < 0)
                    csv->disable[i] = 1;
        return;
    }
}

int filtro(DATA* csv, char* name, char* filter, char* variable){
    unsigned int j;
    short op;
    char c = 'a';
    char* option;
    FILE* file = NULL;


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

    //acha coluna
    for(j = 0; j < csv->nCol; j++)
        if(!strcmp(csv->matrix[0][j],name))
            break;
    //se nao achou
    if(j == csv->nCol){
        printf("Variavel Inválida\n");
        return 1;
    }

    //chama funcao dependo do tipo de dado
    if(!strcmp(csv->type[j],"N"))
        opFilterNumeric(csv, j, op, variable);
    else
        opFilterString(csv, j, op, variable);

    mostrar(csv);

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

    printf("Deseja descartar os dados originais? [S|N]: ");
    scanf("%ms", &option);

    //TALVEZ TENHA Q MUDAR ISSO POIS NAO FUNCIONA TODOS OS CASOS
    //se nao coloca 0 em todo vetor disable
    if(!strcmp(option, "N") || !strcmp(option, "n"))
        for(unsigned int i = 0; i < csv->nRows; i++)
            csv->disable[i] = 0;
    free(option);

    printf("Pressione ENTER para continuar");
    getchar();
    while(c != '\n')
        c = getchar();
    printf("\n");
    return 0;
}

int csvLoad(FILE* file, DATA* csv){
    char line[SIZE];
    char* copy;
    char* freeAux;//ponteiro para free
    char* token;
    unsigned int i = 0;
    unsigned int j = 0;

    //loop de load
    while(fgets(line, SIZE, file) != NULL){
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

int description(DATA* csv, char* name){
    float* array = NULL;
    NODE* list = NULL;
    NODE* mode = NULL;
    unsigned int j;
    unsigned int aux = 0;//indice para vetor
    unsigned int size = 1;//tamanho vetor

    //acha coluna
    for(j = 0; j < csv->nCol; j++)
        if(!strcmp(csv->matrix[0][j],name))
            break;
    //se nao achou
    if(j == csv->nCol){
        printf("Variavel Inválida\n");
        return 1;
    }
    //caso seja uma variavel numerica
    if(!strcmp(csv->type[j],"N")){
        //alocacao array de float caso numerico
        array = (float*)malloc(sizeof(float) * csv->nRows);
        if(!array){
            printf("Allocation Error float array");
            return 1;
        }
        //copia elementos nao NaN para vetor
        for(unsigned int i = 1; i < csv->nRows; i++)
            if(strlen(csv->matrix[i][j]) != 0){
                array[aux] = strtof(csv->matrix[i][j], NULL);
                size++;
                aux++;
            }
        //ordena vetor
        qsort(array, size, sizeof(float), compFloat);
        mode = modeF(array, list, size);

        printf("Contador: %hd\n", size);
        printf("Média: %f\n", aritMean(array, size));
        printf("Desvio: %f\n", stdDeviation(array, aritMean(array, size),size));
        printf("Mediana: %f\n", median(array, size));
        printf("Moda: %f %hd vezes",mode->num, mode->count);
        printf("Min: %f\n", minF(array, size));
        printf("Max: %f\n", maxF(array, size));

        freeList(list);
    }
    //caso variavel seja string
    else
}
