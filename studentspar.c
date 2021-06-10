#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv){

    if(argc != 5)
        return 0;
    int R = atoi(argv[1]);  // Regiões
    int C = atoi(argv[2]);  // Cidades 
    int A = atoi(argv[3]);  // Alunos
    int seed = atoi(argv[4]);   // Semente do rand()
    srand(seed);

    int*** GRADES = (int***)malloc(R*sizeof(int**));

    for(int i = 0; i < C; i++){
        GRADES[i] = (int**)malloc(C*sizeof(int*));
        for(int j = 0; j < A; j++)
            GRADES[i][j] = (int*)malloc(A*sizeof(int));
    }

    for(int i = 0; i < R; i++)
        for(int j = 0; j < C; j++)
            for(int k = 0; k < A; k++)
                GRADES[i][j][k] = rand()%101;

    double t1 = omp_get_wtime();

    for(int i = 0; i < R; i++)
        for(int j = 0; j < C; j++){
            
        }
                

    double t2 = omp_get_wtime();

    printf("time: ", t2 - t1);
    return 0;
}





void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
  
  
int partition(int arr[], int l, int h)
{
    int x = arr[h];
    int i = (l - 1);
  
    for (int j = l; j <= h - 1; j++) {
        if (arr[j] <= x) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[h]);
    return (i + 1);
}
  

void quickSortIterative(int arr[], int l, int h)
{
    // Create an auxiliary stack
    int stack[h - l + 1];
  
    // initialize top of stack
    int top = -1;
  
    // push initial values of l and h to stack
    stack[++top] = l;
    stack[++top] = h;
  
    // Keep popping from stack while is not empty
    while (top >= 0) {
        // Pop h and l
        h = stack[top--];
        l = stack[top--];
  
        // Set pivot element at its correct position
        // in sorted array
        int p = partition(arr, l, h);
  
        // If there are elements on left side of pivot,
        // then push left side to stack
        if (p - 1 > l) {
            stack[++top] = l;
            stack[++top] = p - 1;
        }
  
        // If there are elements on right side of pivot,
        // then push right side to stack
        if (p + 1 < h) {
            stack[++top] = p + 1;
            stack[++top] = h;
        }
    }
}
