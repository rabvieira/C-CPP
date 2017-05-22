/*-----------------------------------------------------------------------------
 Name      : pthreads_media.c
 Date      : August 29, 2016
 Class     : Programacao Concorrente e Distribuida
 Author    : Renan A. B. Vieira
 R. A.     : 70212
 Descript. : This program calculates the average value given a vector with
             random numbers through threads using PTHREADS
-----------------------------------------------------------------------------*/

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

/* Constants */
#define N 8             /*size of the array*/ //100000// 1000000// 10000000
#define MAX_THREADS 4   /*number of threads used*/ //1,2,4,8
#define NUM_MAX 100     /*number max for the random values*/ //defined by the user

/* Global */
double *array;   /*pointer to array of numbers*/
double *retorno; /*pointer to the partial media*/

//thread function called to fill the array and calculate the media
void *media(void *tid)
{
    int thid;           /*thread ID*/
    int i;              /*iterator*/
    unsigned int seed;  /*clock seed*/
    double sumloc = 0;  /*local sum*/

    //seeds the number generator
    srand(time(NULL));

    //cast thread ID
    thid = (int) tid;

    //access the specific thread position, filling and adding random values
    for(i=thid; i<N; i+=MAX_THREADS)
    {
        //starts the seed with the thread_id
        seed = i + time(NULL);
        //initialize the array with random numbers
        array[i] = (double) (rand_r(&seed)%(NUM_MAX-1))+1;
        //calculate the local sum within this thread
        sumloc += array[i];
    }

    //print useful information
    printf("\nthread=%d,\tsoma local=%.0f,\t\tmedia local=%g\n", thid, (float)sumloc, sumloc/(N/MAX_THREADS));

    //return the local sum of this thread
    retorno[thid] = sumloc;
}

//Main Function
int main(void)
{
    pthread_t t[MAX_THREADS];       /*array of threads*/
    double sumglobal=0;             /*final sum*/
    int th, tmili;                  /*thread iterator and time of execution*/
    struct timeval inicio, final2;  /*timer structures*/

    //pointers are allocated
    array = (double*) malloc(N*sizeof(double));
    retorno = (double*) malloc(MAX_THREADS*sizeof(double));

    //start the clock
    gettimeofday(&inicio, NULL);

    //create threads
    for(th=0; th<MAX_THREADS; th++)
    {
        pthread_create(&t[th], NULL, &media, (void *) th);
    }

    //join threads and get partial sums
    for(th=0; th<MAX_THREADS; th++)
    {
        pthread_join(t[th],NULL);
        sumglobal += retorno[th];
    }

    //stop the clock
    gettimeofday(&final2, NULL);

    //(optional: prints the array)
    /*
    printf("\n\narray:\n");
    for(th=0; th<N; th++)
        printf("%.0f ", array[th]);
    */

    //calculate the time
    tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

    //print useful information
    printf("\n\ntempo decorrido: %d milisegundos\n", tmili);
    printf("\n\nmedia global=%g\n\n",sumglobal/N);

    return 0;
}


