#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>



void mergeSort(int arr[], int n);
void merge(int arr[], int l, int m, int r);

float media(int *GRADES, int A)
{
    int i; 
    float MEDIA = 0;
    
    for(i = 0; i < A; i++)
    {
        MEDIA = MEDIA + GRADES[i];
    }
    MEDIA = MEDIA/A;

    return MEDIA;
}

float desvPad(float mean, int arr[], int n){
    float sum = 0;
    for(int i = 0; i < n; i++){
        sum += (arr[i] - mean)*(arr[i] - mean);
    }
    sum /= n-1;
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
    float *RESULTS = (float*)malloc(5*C*sizeof(float));     // Vetor com os dados resultantes [menor, maior, mediana, média, DP]
    
    int i;
    for(i = 0; i < T; i++)          // Gera as notas dos alunos
        GRADES[i] = rand()%101;

    double t1 = omp_get_wtime(); 
    for(i = 0; i < C*A; i = i + A)
    {
        mergeSort(&GRADES[i], i + A);
        RESULTS[i] = GRADES[i]; // Menor nota daquela cidade
        RESULTS[i+1] = GRADES[i+A-1]; // Maior nota daquela cidade
        
        if((A%2)==0)
            RESULTS[i+2] = (GRADES[i+(A/2)-1] + GRADES[i+(A/2)])/2; // Mediana daquela cidade, caso A seja par
        else
            RESULTS[i+2] = GRADES[i+(A/2)]; // Mediana daquela cidade, caso A seja ímpar

        RESULTS[i+3] = media(GRADES, A); // Media daquela cidade
        RESULTS[i+4] = desvPad(RESULTS[i+3], &GRADES[i], i+A); // Desvio padrão daquela cidade
    }
    double t2 = omp_get_wtime();

    printf("Time: %lf\n", t2 - t1);

    printf("\n");
   for(i = 0; i < T; i = i + A)
   {
       printf("%d %d %d %d %d %d\n", GRADES[i], GRADES[i+1], GRADES[i+2], GRADES[i+3], GRADES[i+4], GRADES[i+5]);
   }
   printf("\n");

   int k;
    for(int j = 0; j < R; j++)
    {
        k = 0;
       for(i = 5*j*C ; i < (5*C)*(j+1); i = i + 5)
        {
            printf("Reg %d - Cid %d: menor: %.0f, maior: %.0f, mediana: %.2f, média: %.2f e DP: %.2f\n", j, k, RESULTS[i], RESULTS[i+1], RESULTS[i+2], RESULTS[i+3], RESULTS[i+4]);
            k++;
        }
        printf("\n"); 
        printf("\n");
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
