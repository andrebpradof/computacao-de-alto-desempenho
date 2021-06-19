#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define SIZE 1001
#define NUM_MAX_CHAR 128
#define NUM_MIN_CHAR 32
struct caractere {
    char caractere;
    int freq;
};

typedef struct caractere Caractere;

void radixsort(Caractere vetor[], int inicio, int tamanho);
void arruma_empate(Caractere vetor[], int inicio, int tamanho);

int main(){
    char text[SIZE];
    memset(text, '\0', SIZE*sizeof(char));
    

    while(fgets(text, SIZE, stdin) != NULL){
        Caractere caractere[NUM_MAX_CHAR];

        for(int i = 0; i < NUM_MAX_CHAR; i++){
            caractere[i].caractere = i;
            caractere[i].freq = 0;
        }

        for(int i = 0; text[i] != '\n'; i++)
            caractere[text[i]].freq++;
        
        radixsort(caractere, NUM_MIN_CHAR, NUM_MAX_CHAR);
        arruma_empate(caractere, NUM_MIN_CHAR, NUM_MAX_CHAR);

        for(int i = NUM_MIN_CHAR; i < NUM_MAX_CHAR; i++)
            if(caractere[i].freq != 0)
                printf("%c %d\n", caractere[i].caractere, caractere[i].freq);
        printf("\n");

        memset(text, '\0', SIZE*sizeof(char));
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

    for(int i = inicio; i < tamanho-1; i++){
        if(vetor[i].freq == vetor[i+1].freq){
            int j = i+1;
            while(j < tamanho && vetor[j].freq == vetor[j+1].freq)
                j++;
            
        }
    }
}