#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#define SIZE 1001        // Tamanho máximo da linha
#define NUM_MAX_CHAR 128 // Intervalo ASCII
#define NUM_MIN_CHAR 32  //  Intervalo ASCII
#define BLOCKSIZE 100
typedef struct car_s
{
    char caractere;
    int contagem;
} char_count;

int main(int argc, char **argv)
{
    // Leitura dos dados
    int numLines = 0;
    char *text = (char *)calloc(SIZE, sizeof(char));

    while (1)
    {
        char *result = fgets(&text[numLines * SIZE], SIZE, stdin);
        if (result == NULL)
            break;
        numLines += 1;
        text = (char *)realloc(text, SIZE * (numLines + 1) * sizeof(char)); // Realocacao
    }
    // Final da leitura

    int my_rank, num_procs;
    int tag, src, dest, test_flag, i;
    int buffersize, sizemsgsnd, nrmsgs;
    int provided;

    char *buffersnd;
    char msgsnd[30], msgrcv[30], msgsync[30];

    MPI_Status status;
    MPI_Request mpirequest_mr, mpirequest_mr_ready;

    MPI_Init_thread(&argc, &argv, /*MPI_THREAD_MULTIPLE*/ MPI_THREAD_SINGLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    /*Criação do tipo MPI char_count que é uma struct contendo qual caracter e numero de vezes que ele aparece*/
    const int nitems = 2;
    int blocklengths[2] = {1, 1};
    MPI_Datatype types[2] = {MPI_CHAR, MPI_INT};
    MPI_Datatype mpi_char_count_type;
    MPI_Aint offsets[2];

    offsets[0] = offsetof(char_count, caractere);
    offsets[1] = offsetof(char_count, contagem);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_char_count_type);
    MPI_Type_commit(&mpi_char_count_type);

    tag = 0;
    if (my_rank == 0)
    {
        printf("There are %d processes.\n", num_procs);
        printf("I am process %d \n", my_rank);
        int linhas_por_thread = numLines / (num_procs - 1);
        int linhas_ultima_thread = numLines % (num_procs - 1);

        for (dest = 1; dest < npes - 1; dest++)
            MPI_Send((void *)&text[(dest - 1) * SIZE * linhas_por_thread], linhas_por_thread * SIZE, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
        MPI_Send((void *)&text[(dest - 1) * SIZE * linhas_ultima_thread], linhas_ultima_thread * SIZE, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);

        printf("todas as mensagens enviadas")
    }    // fim do if (my_rank == 0)
    else // my_rank == 1
    {
        printf("There are %d processes.\n", num_procs);
        printf("I am process %d \n", my_rank);
    } // fim do else my_rank == 1

    MPI_Finalize();

    exit(0);
}