#include <stdio.h>
#include <stdlib.h>



int main(int argc, char** argv){

    srand(atoi(argv[1]));
    int qtd_char = atoi(argv[2]);
    int qtd_linha = atoi(argv[3]);

    for(int i = 0; i < qtd_linha; i++){
        for(int j = 0; j < qtd_char; j++){
            char letra = (char)(rand()%96 + 32);
            printf("%c", letra);
        }
        printf("\n");
    }
    return 0;
}