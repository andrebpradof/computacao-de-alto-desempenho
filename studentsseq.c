/*****************************************************************************************************************************
Q4) SOLUÇÃO SEQUENCIAL (studentsseq.c)
André Baconcelo Prado Furlanetti (10748305)
George Alexandre Gantus (10691988)
Leonardo Prado Dias (10684642)
Pedro Paulo Herzog Junior (10284692)
Victor Felipe Domingues do Amaral (10696506)
*****************************************************************************************************************************/

/*
Bibliotecas utilizadas no programa
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/*
Funções utilizadas ao longo da execução do programa
*/
void mergeSort(int arr[], int n);
void merge(int arr[], int l, int m, int r);
float countingSort(int array[], int size);
float desvPad(float mean, int arr[], int n);
float media(int *GRADES, int tam);

/*
Defines utilizados para melhorar a leitura do código
*/
#define MENOR 0
#define MAIOR 1
#define MEDIANA 2
#define MEDIA 3
#define DESVPAD 4
#define MAX_NOTAS 100

/*
Função principal
*/
int main(int argc, char** argv){
    if(argc != 5)
        return 0;
    int R = atoi(argv[1]);  // Regiões
    int C = atoi(argv[2]);  // Cidades 
    int A = atoi(argv[3]);  // Alunos
    int seed = atoi(argv[4]);   // Semente do rand()
    srand(seed);

    int T = R*C*A;          // Total de elementos
    int regionSize = A*C;
    int betterCityIndex = 0;
    int biggerCityMedia = 0;
    int betterRegionIndex = 0;
    int biggerRegionMedia = 0;
    
    int *GRADES = (int*)malloc(T*sizeof(int));        // Vetor com todos os elementos
    float *CITIES = (float*)malloc(5*R*C*sizeof(float));     // Vetor com os dados resultantes das cidades[menor, maior, mediana, média, DP]
    float *REGIONS = (float*)malloc(5*R*sizeof(float));     // Vetor com os dados resultantes das regiões[menor, maior, mediana, média, DP]
    float *BRAZIL = (float*)malloc(5*sizeof(float));     // Vetor com os dados resultantes do Brasil [menor, maior, mediana, média, DP]
    
    for(int i = 0; i < T; i++)          // Gera as notas dos alunos
        GRADES[i] = rand()%101;


   double t1 = omp_get_wtime(); 
   
    for(int i = 0, j = 0; i < T; i = i + A, j+=5){
        CITIES[j + MEDIA] = countingSort(&GRADES[i], A); // MEDIA E ORDENA

        CITIES[j + MENOR] = GRADES[i]; // Menor nota daquela cidade
        CITIES[j + MAIOR] = GRADES[i+A-1]; // Maior nota daquela cidade
        
        if((A%2)==0)
            CITIES[j + MEDIANA] = (GRADES[i+(A/2)-1] + GRADES[i+(A/2)])/2.0; // Mediana daquela cidade, caso A seja par
        else
            CITIES[j + MEDIANA] = GRADES[i+(A/2)]; // Mediana daquela cidade, caso A seja ímpar
        if(CITIES[j + MEDIA] >= biggerCityMedia){
          biggerCityMedia = CITIES[j + MEDIA];
          betterCityIndex = (i+1)/A;
        } 
        CITIES[j + DESVPAD] = desvPad(CITIES[j+MEDIA], &GRADES[i], A); // Desvio padrão daquela cidade;
    }
    
    
    for(int i = 0, j = 0; i < T; i = i + (regionSize), j+=5){
        REGIONS[j + MEDIA] = countingSort(&GRADES[i], regionSize); // MEDIA E ORDENA
        REGIONS[j + MENOR] = GRADES[i]; // Menor nota daquela cidade
        REGIONS[j + MAIOR] = GRADES[i+(regionSize)-1]; // Maior nota daquela cidade
        
        if((A%2)==0)
            REGIONS[j + MEDIANA] = (GRADES[i+((regionSize)/2)-1] + GRADES[i+((regionSize)/2)])/2.0; // Mediana daquela cidade, caso A seja par
        else
            REGIONS[j + MEDIANA] = GRADES[i+((regionSize)/2)]; // Mediana daquela cidade, caso A seja ímpar

        if(REGIONS[j + MEDIA] >= biggerRegionMedia){
          biggerRegionMedia = REGIONS[j + MEDIA];
          betterRegionIndex = (i+1)/(C*A);
        } 
        REGIONS[j + DESVPAD] = desvPad(REGIONS[j+MEDIA], &GRADES[i], (regionSize)); // Desvio padrão daquela cidade;
    }

    BRAZIL[MEDIA] = countingSort(GRADES, T);
    BRAZIL[MENOR] = GRADES[0];
    BRAZIL[MAIOR] = GRADES[T-1];
    BRAZIL[MEDIANA] = T % 2 == 0? (GRADES[T/2 -1] + GRADES[T/2])/2 : GRADES[T/2];
    BRAZIL[DESVPAD] = desvPad(BRAZIL[MEDIA], GRADES, T);
    
    
    double t2 = omp_get_wtime();



    // PRINT SECTION //
    
    for(int i = 0; i < R; i++){
       for(int j = 5*i*C ; j < (5*C)*(i+1); j = j + 5)
            printf("Reg %d - Cid %d: menor: %.0f, maior: %.0f, mediana: %.2f, média: %.2f e DP: %.2f\n", i, (j / 5) % C, CITIES[j+MENOR], CITIES[j+MAIOR], CITIES[j+MEDIANA], CITIES[j+MEDIA], CITIES[j+DESVPAD]);
        
        printf("\n"); 
        printf("\n");
    }
    
    for(int i = 0; i < R*5; i+=5)
    {
        printf("Reg %d: menor: %.0f, maior: %.0f, mediana: %.2f, média: %.2f e DP: %.2f\n", i / 5, REGIONS[i+MENOR], REGIONS[i+MAIOR], REGIONS[i+MEDIANA], REGIONS[i+MEDIA], REGIONS[i+DESVPAD]);
    }
    printf("\n"); 
    printf("\n");

    printf("Brasil: menor: %.0f, maior: %.0f, mediana: %.2f, média: %.2f e DP: %.2f\n", BRAZIL[MENOR], BRAZIL[MAIOR], BRAZIL[MEDIANA], BRAZIL[MEDIA], BRAZIL[DESVPAD]);
    printf("\n"); 
    printf("\n");

    printf("Melhor região: Região %d\n", betterRegionIndex);


    printf("Melhor cidade: Região %d, Cidade %d\n", betterCityIndex / R, betterCityIndex % C);
    printf("\n"); 
    printf("\n");

    printf("Tempo de resposta sem considerar E/S, em segundos: %.3fs\n", (t2-t1));
    

    return 0;
}

/*
Função que ordena os vetores do programa, com O(n). Retorna a média dos dados.
 */
float countingSort(int array[], int size) {
    float soma = 0;
    int* output = (int*)malloc(size*sizeof(int));
    int count[MAX_NOTAS + 1];

    for (int i = 0; i <= MAX_NOTAS; ++i) {
        count[i] = 0;
    }

    for (int i = 0; i < size; i++) {
        count[array[i]]++;
        soma += array[i];
    }

    for (int i = 1; i <= MAX_NOTAS; i++) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        output[count[array[i]] - 1] = array[i];
        count[array[i]]--;
    }

    for (int i = 0; i < size; i++) {
        array[i] = output[i];
    }
    free(output);
    return soma/size; // Retorna a média
}

/*
    Função para o cálculo da media O(n)
*/
float media(int *GRADES, int tam)
{
    int j; 
    float media = 0;
    
    for(j = 0; j < tam; j++)
    {
        media = media + GRADES[j];
    }
    media = media/tam;

    return media;
}

/*
    Função para o cálculo do desvio padrão O(n)
*/
float desvPad(float mean, int arr[], int n)
{
    float sum = 0;
    for(int i = 0; i < n; i++){
        sum += (arr[i] - mean)*(arr[i] - mean);
    }
    sum /= n;
    return sqrt(sum);
}