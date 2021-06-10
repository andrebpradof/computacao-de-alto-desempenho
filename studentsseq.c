#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void mergeSort(int arr[], int n);
void merge(int arr[], int l, int m, int r);

#define MENOR 0
#define MAIOR 1
#define MEDIANA 2
#define MEDIA 3
#define DESVPAD 4

float media(int *GRADES, int A, int i)
{
    int j; 
    float media = 0;
    
    for(j = i; j < (i + A); j++)
    {
        media = media + GRADES[j];
    }
    media = media/A;

    return media;
}

float desvPad(float mean, int arr[], int n){
    float sum = 0;
    for(int i = 0; i < n; i++){
        sum += (arr[i] - mean)*(arr[i] - mean);
    }
    sum /= n;
    return sqrt(sum);
}

int main(int argc, char** argv){

    if(argc != 5)
        return 0;
    int R = atoi(argv[1]);  // Regiões
    int C = atoi(argv[2]);  // Cidades 
    int A = atoi(argv[3]);  // Alunos
    int seed = atoi(argv[4]);   // Semente do rand()
    srand(seed);

    int T = R*C*A;          // Total de elementos

    int *GRADES = (int*)malloc(T*sizeof(int));        // Vetor com todos os elementos
    float *CITIES = (float*)malloc(5*R*C*sizeof(float));     // Vetor com os dados resultantes das cidades[menor, maior, mediana, média, DP]
    float *REGIONS = (float*)malloc(5*R*sizeof(float));     // Vetor com os dados resultantes das regiões[menor, maior, mediana, média, DP]
    float *BRAZIL = (float*)malloc(5*sizeof(float));     // Vetor com os dados resultantes do Brasil [menor, maior, mediana, média, DP]
    
    for(int i = 0; i < T; i++)          // Gera as notas dos alunos
        GRADES[i] = rand()%101;

    float aux;
   double t1 = omp_get_wtime(); 
    for(int i = 0, j = 0; i < T; i = i + A, j+=5)
    {
        mergeSort(&GRADES[i], A);
        CITIES[j + MENOR] = GRADES[i]; // Menor nota daquela cidade
        CITIES[j + MAIOR] = GRADES[i+A-1]; // Maior nota daquela cidade
        
        if((A%2)==0)
            CITIES[j + MEDIANA] = (GRADES[i+(A/2)-1] + GRADES[i+(A/2)])/2.0; // Mediana daquela cidade, caso A seja par
        else
            CITIES[j + MEDIANA] = GRADES[i+(A/2)]; // Mediana daquela cidade, caso A seja ímpar

        CITIES[j + MEDIA] = media(GRADES, A, i); // Media daquela cidade
        CITIES[j + DESVPAD] = desvPad(CITIES[j+MEDIA], &GRADES[i], A); // Desvio padrão daquela cidade;
    }
   double t2 = omp_get_wtime();

   int k;
    for(int j = 0; j < R; j++)
    {
        k = 0;
       for(int i = 5*j*C ; i < (5*C)*(j+1); i = i + 5)
        {
            printf("Reg %d - Cid %d: menor: %.0f, maior: %.0f, mediana: %.2f, média: %.2f e DP: %.2f\n", j, k, CITIES[i], CITIES[i+1], CITIES[i+2], CITIES[i+3], CITIES[i+4]);
            k++;
        }
        printf("\n"); 
        printf("\n");
    }

    return 0;
}

void mergeSort(int arr[], int n)
{
   int curr_size;  // For current size of subarrays to be merged
                   // curr_size varies from 1 to n/2
   int left_start; // For picking starting index of left subarray
                   // to be merged
 
   // Merge subarrays in bottom up manner.  First merge subarrays of
   // size 1 to create sorted subarrays of size 2, then merge subarrays
   // of size 2 to create sorted subarrays of size 4, and so on.
   for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
   {
       // Pick starting point of different subarrays of current size
       for (left_start=0; left_start<n-1; left_start += 2*curr_size)
       {
           // Find ending point of left subarray. mid+1 is starting
           // point of right
           int mid = ((left_start + curr_size - 1) < (n-1)) ? (left_start + curr_size - 1) : (n-1);
 
           int right_end = ((left_start + 2*curr_size - 1) < (n-1) ? (left_start + 2*curr_size - 1) : (n-1));
 
           // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
           merge(arr, left_start, mid, right_end);
       }
   }
}
 
/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
