/*-----------------------------------------------------------------------------
 Name      : openmp_media.c
 Date      : August 29, 2016
 Class     : Programacao Concorrente e Distribuida
 Author    : Renan A. B. Vieira
 R. A.     : 70212
 Descript. : This program calculates the average value given a vector with
             random numbers through threads using OPENMP
-----------------------------------------------------------------------------*/

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>

/* Constants */
#define N 8             /*size of the array*/ //100000// 1000000// 10000000
#define MAX_THREADS 4   /*number of threads used*/ //1,2,4,8
#define NUM_MAX 100     /*number max for the random values*/ //defined by the user

/* Global */
double *array;   /*pointer to array of numbers*/

//Main Function
int main(void)
{
    int thid,i;                     //thread id and iterator
    unsigned int seed;              //clock seed
    double sumloc = 0;              //local sum
    double array_loc[MAX_THREADS];  //array of local sums
    double sumglobal=0;             //final sum
    int tmili;                      //time of execution
    struct timeval inicio, final2;  //timer structures

    //sets the number of threads that will be used
    omp_set_num_threads(MAX_THREADS);

    //pointer to array of numbers is allocated
    array = (double*) malloc(N*sizeof(double));

    //seeds the number generator
    srand(time(NULL));

    //start the clock
    gettimeofday(&inicio, NULL);

    //create threads
    #pragma omp parallel private(i, thid, seed, sumloc)
    {
        //gets the thread id
        thid = omp_get_thread_num();

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
        array_loc[thid] = sumloc;

        //print useful information
        printf("\nthread=%d,\tsoma local=%.0f,\t\tmedia local=%g\n", thid, (float)sumloc, sumloc/(N/MAX_THREADS));
    }

    //gets the global sum
    for(i=0; i<MAX_THREADS; i++)
        sumglobal += array_loc[i];

    //stop the clock
    gettimeofday(&final2, NULL);

    //(optional: prints the array)
    /*
    printf("\n\narray:\n");
    for(i=0; i<N; i++)
        printf("%.0f ", array[i]);
    */

    //calculate the time
    tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

    //print useful information
    printf("\n\ntempo decorrido: %d milisegundos\n", tmili);
    printf("\n\nmedia global=%g\n\n",sumglobal/N);

    return 0;
}


