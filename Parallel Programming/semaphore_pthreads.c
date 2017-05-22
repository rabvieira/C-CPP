/*-----------------------------------------------------------------------------
 Name      : semaphore_pthreads.c
 Date      : September 19, 2016
 Class     : Programacao Concorrente e Distribuida
 Author    : Renan A. B. Vieira
 R. A.     : 70212
 Descript. : This program calculates the dot product given two vectors with
             random numbers through threads using SEMAPHORES and PTHREADS
-----------------------------------------------------------------------------*/

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

/* Constants */
#define N 10000000        /*size of the array*/ //100000 //10000000
#define MAX_THREADS 1   /*number of threads used*/ //1,2,4,8
#define NUM_MAX 10      /*number max for the random values*/ //defined by the user

/* Global */
double *arrayA;         /*pointer to array of numbers*/
double *arrayB;         /*pointer to array of numbers*/
double sumResult = 0;   /*global sum*/
pthread_mutex_t sb;     /*semaphore*/

//thread function called to calculate the dot ptoduct
void *dot_product(void *tid)
{
    long int thid;           /*thread ID*/
    int i;              /*iterator*/
    double sumloc = 0;  /*local sum*/

    //cast thread ID
    thid = (long int) tid;

    //access the specific thread position, calculating dot product
    for(i=thid; i<N; i+=MAX_THREADS)
    {
        //calculate the dot ptoduct within this thread
        sumloc += arrayA[i]*arrayB[i];
    }

    //print useful information
    printf("\nthread=%ld,\tsoma local=%.0f\n", thid, (float)sumloc);

    pthread_mutex_lock(&sb);
    //starts critical section
    sumResult += sumloc;
    //finishes critical section
    pthread_mutex_unlock(&sb);

}

//Main Function
int main(void)
{
    pthread_t t[MAX_THREADS];       /*array of threads*/
    int i, tmili;                   /*thread iterators and time of execution*/
    long int th;                    /*thread iterators*/
    struct timeval inicio, final2;  /*timer structures*/

    //seeds the number generator
    srand(time(NULL));

    //pointers are allocated
    arrayA =  (double*) malloc(N*sizeof(double));
    arrayB =  (double*) malloc(N*sizeof(double));

    //filling vectors with random values
    for(i=0; i<N; i++)
    {
        //initialize arrays with random numbers
        arrayA[i] = (double) (rand()%(NUM_MAX-1))+1;
        arrayB[i] = (double) (rand()%(NUM_MAX-1))+1;
    }

    //start the clock
    gettimeofday(&inicio, NULL);

    //create threads
    for(th=0; th<MAX_THREADS; th++)
    {
        pthread_create(&t[th], NULL, &dot_product, (void *) th);
    }

    //join threads
    for(th=0; th<MAX_THREADS; th++)
    {
        pthread_join(t[th],NULL);
    }

    //stop the clock
    gettimeofday(&final2, NULL);

//(optional: prints the array)
/*
    printf("\n\narray A:\n");
    for(th=0; th<N; th++)
    {
        printf("%.0f ", arrayA[th]);
    }
    printf("\n\narray B:\n");
    for(th=0; th<N; th++)
    {
        printf("%.0f ", arrayB[th]);
    }
*/

    //calculate the time
    tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

    //print useful information
    printf("\n\ntempo decorrido: %d milisegundos\n", tmili);
    printf("\nresultado do produto escalar=%g\n\n",sumResult);

    return 0;
}