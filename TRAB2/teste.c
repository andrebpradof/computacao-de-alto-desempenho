#include<stdio.h>
#include<stdlib.h>

int main(void){
    unsigned char *vetor;
    long long *contagem, maior=0, i, tam;

    contagem = (long long*)malloc(256 * sizeof(long long));
    for(int i=0;i<256;i++)
        contagem[i] = 0;

    fscanf(stdin, "%lld\n", &tam); //Lê o tamanho do vetor
    vetor=(unsigned char*)malloc(tam*sizeof(unsigned char));

    for(i=0;i<tam;i++)
        fscanf(stdin, "%c",&(vetor[i])); //Lê os elementos do vetor

    #pragma omp parallel for private(i) reduction(+: contagem[:256])
    for(int i=0;i<tam;i++){
        contagem[(short)vetor[i]] += 1;
    }

    #pragma omp parallel for private(i) reduction(max: maior)
    for (i = 1; i < 256; i++) {
        if (contagem[i] > maior)
            maior = contagem[i];
    }

    for (i = 1; i < 256; i++) {
        if (contagem[i] == maior)
            printf("%c ", (char) i);
    }
    free(vetor);
}