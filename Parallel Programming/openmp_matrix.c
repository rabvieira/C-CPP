/*-----------------------------------------------------------------------------
 Name      : openmp_matrix.c
 Date      : August 29, 2016
 Class     : Programacao Concorrente e Distribuida
 Author    : Renan A. B. Vieira
 R. A.     : 70212
 Descript. : This program calculates the matrix sum given two others matrices
             with random numbers through threads using OPENMP
-----------------------------------------------------------------------------*/

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>

/* Constants */
#define N 4             /*size of the squared matrix*/ //100// 1000
#define MAX_THREADS 4   /*number of threads used*/ //1,2,4,8
#define NUM_MAX 100     /*number max for the random values*/ //defined by the user

/* Global */
double **matrixA;       /*pointer to input matrix A*/
double **matrixB;       /*pointer to input matrix B*/
double **matrixS;       /*pointer to output matrix S = A+B*/

//prints all three matrices
void print_matrix()
{
    int i,j;

    printf("\nmatrix A:\n");
    for(i=0; i<N; i++)
    {
        for(j=0 ; j<N ; j++)
        {
            printf("%.0f\t", matrixA[i][j]);
        }
        printf("\n");
    }

    printf("\n\nmatrix B:\n");
    for(i=0; i<N; i++)
    {
        for(j=0 ; j<N ; j++)
        {
            printf("%.0f\t", matrixB[i][j]);
        }
        printf("\n");
    }

    printf("\n\nmatrix SUM=A+B:\n");
    for(i=0; i<N; i++)
    {
        for(j=0 ; j<N ; j++)
        {
            printf("%.0f\t", matrixS[i][j]);
        }
        printf("\n");
    }
}

//Main Function
int main(void)
{
    pthread_t t[MAX_THREADS];       /*array of threads*/
    int i,j,thid, tmili;            /*thread iterators and time of execution*/
    struct timeval inicio, final2;  /*timer structures*/

    //pointers are allocated
    matrixA = (double**) malloc(N*sizeof(double*));
    matrixB = (double**) malloc(N*sizeof(double*));
    matrixS = (double**) malloc(N*sizeof(double*));

    //seeds the number generator
    srand(time(NULL));

    //filling random values into the input matrices
    for(i=0; i<N; i++)
    {
        matrixA[i] = (double*)malloc(N*sizeof(double));
        matrixB[i] = (double*)malloc(N*sizeof(double));
        matrixS[i] = (double*)malloc(N*sizeof(double));

        for(j=0 ; j<N ; j++)
        {
            matrixA[i][j] = (double) (rand()%(NUM_MAX-1))+1;
            matrixB[i][j] = (double) (rand()%(NUM_MAX-1))+1;
        }
    }

    //start the clock
    gettimeofday(&inicio, NULL);

    //create threads
    #pragma omp parallel private(i, thid)
    {
        //gets the thread id
        thid = omp_get_thread_num();

        //access the specific thread position, filling and adding random values
        for(i=thid ; i<N ; i+=MAX_THREADS)
        {
            for(j=0 ; j<N ; j++)
            {
                matrixS[i][j] = matrixA[i][j] + matrixB[i][j];
            }
        }
        printf("Hello from thread %d\n",thid);
    }

    //stop the clock
    gettimeofday(&final2, NULL);

    //calculate the time
    tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

    //(optional: prints matrices)
    print_matrix();

    //print useful information
    printf("\n\ntempo decorrido: %d milisegundos\n", tmili);

    return 0;
}

