/********************************************************************************
* *               Trabalho I - SSC0903 - Computação de alto desempenho          *     
*   André Baconcelo Prado Furlanetti (10748305)                                 *
*   George Alexandre Gantus (10691988)                                          *
*   Leonardo Prado Dias (10684642)                                              *
*   Pedro Paulo Herzog Junior (10284692)                                        *
*   Victor Felipe Domingues do Amaral (10696506)                                *
*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define SIZE 1001   // Tamanho máximo da linha
#define NUM_MAX_CHAR 128   // Intervalo ASCII 
#define NUM_MIN_CHAR 32   //  Intervalo ASCII
#define BLOCKSIZE 100
struct caractere {
    char caractere;
    int freq;
};

typedef struct caractere Caractere;

void radixsort(Caractere vetor[], int inicio, int tamanho);
void arruma_empate(Caractere vetor[], int inicio, int tamanho);

int main(){
    char **textLines = (char**) malloc(BLOCKSIZE*sizeof(char *));
    int currentSize = 100;
    int numLines = 0;
    
    while(1){
        char *text = calloc(SIZE, sizeof(char));
        char* result = fgets(text, SIZE, stdin);
        if(result == NULL)
            break;
        textLines[numLines] = text;
        numLines += 1;
        if(numLines == currentSize){
            currentSize += BLOCKSIZE;
            textLines = (char**)realloc(textLines,currentSize*sizeof(char *));
        }
    }

    double t1 = omp_get_wtime();
    //aqui todos os dados foram lidos no vetor textSize e i é o numero de linhas
    Caractere **charLists = (Caractere**)malloc(numLines*sizeof(Caractere*));
    #pragma omp parallel for firstprivate(numLines)
    for(int j = 0; j < numLines; j++){

        Caractere *caractere = (Caractere*)malloc(NUM_MAX_CHAR*sizeof(Caractere));

        for(int i = 0; i < NUM_MAX_CHAR; i++){
            caractere[i].caractere = i;
            caractere[i].freq = 0;
        }
        char* line = textLines[j];
    
        for(int i = 0; line[i] != '\n'; i++)
            caractere[(int)line[i]].freq++;
        
        radixsort(caractere, NUM_MIN_CHAR, NUM_MAX_CHAR);
        arruma_empate(caractere, NUM_MIN_CHAR, NUM_MAX_CHAR);
        charLists[j] = caractere;
    }

    double t2 = omp_get_wtime();

    printf("Time lapsed: %lf\n", t2 - t1);

    
    for(int j = 0; j < numLines; j++){
        for(int i = NUM_MIN_CHAR; i < NUM_MAX_CHAR; i++)
            if(charLists[j][i].freq != 0)
                printf("%c %d\n", charLists[j][i].caractere, charLists[j][i].freq);
        printf("\n");
    }
    
    return 0;
}

void radixsort(Caractere vetor[], int inicio, int tamanho) {
    int i;
    Caractere *b;
    Caractere maior = vetor[inicio];
    int exp = 1;

    b = (Caractere*)calloc(tamanho, sizeof(Caractere));

    for (i = inicio + 1; i < tamanho; i++) {
        if (vetor[i].freq > maior.freq)
    	    maior = vetor[i];
    }

    while (maior.freq/exp > 0) {
        int bucket[10] = { 0 };
    	for (i = inicio; i < tamanho; i++)
    	    bucket[(vetor[i].freq / exp) % 10]++;
    	for (i = 1; i < 10; i++)
    	    bucket[i] += bucket[i - 1];
    	for (i = tamanho - 1; i >= inicio; i--){
    	    b[--bucket[(vetor[i].freq / exp) % 10]] = vetor[i];
        }
    	for (i = inicio; i < tamanho; i++)
    	    vetor[i] = b[i - inicio];
    	exp *= 10;
    }

    free(b);
}


void arruma_empate(Caractere vetor[], int inicio, int tamanho){
    int i = inicio;
    while(i < tamanho-1){
        if(vetor[i].freq == vetor[i+1].freq){
            int indiceAux = i;
            while(vetor[indiceAux].freq == vetor[indiceAux+1].freq){
                int indiceAux2 = indiceAux; 
                int indiceMaior = indiceAux2;
                while (vetor[indiceAux2].freq == vetor[indiceAux2+1].freq)
                {
                    if(vetor[indiceAux2+1].caractere > vetor[indiceAux2].caractere){
                        indiceMaior = indiceAux2 + 1;
                    }
                    indiceAux2++;
                }
                Caractere aux = vetor[indiceAux];
                vetor[indiceAux] = vetor[indiceMaior];
                vetor[indiceMaior] = aux;
                indiceAux++;
            }
            i = indiceAux + 1;
            continue;
            
        }
        i++;
    }
}