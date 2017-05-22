/* ------------------------------------------------------------------------
 * FILE: least-squares.c
 * This program computes a linear model for a set of given data.
 *
 * PROBLEM DESCRIPTION:
 *  The method of least squares is a standard technique used to find
 *  the equation of a straight line from a set of data. Equation for a
 *  straight line is given by
 *    y = mx + b
 *  where m is the slope of the line and b is the y-intercept.
 *
 *  Given a set of n points {(x1,y1), x2,y2),...,xn,yn)}, let
 *      SUMx = x1 + x2 + ... + xn
 *      SUMy = y1 + y2 + ... + yn
 *      SUMxy = x1*y1 + x2*y2 + ... + xn*yn
 *      SUMxx = x1*x1 + x2*x2 + ... + xn*xn
 *
 *  The slope and y-intercept for the least-squares line can be
 *  calculated using the following equations:
 *        slope (m) = ( SUMx*SUMy - n*SUMxy ) / ( SUMx*SUMx - n*SUMxx )
 *  y-intercept (b) = ( SUMy - slope*SUMx ) / n
 *
 * AUTHOR: Dora Abdullah (Fortran version, 11/96)
 * REVISED: RYL (converted to C, 12/11/96)
 * ---------------------------------------------------------------------- */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define MAX_THREADS 1   /*number of threads used*/ //1,2,4

int main(int argc, char **argv) {
  double *x, *y;
  double SUMx, SUMy, SUMxy, SUMxx, SUMres, res, slope,
         y_intercept, y_estimate, xini, temp, sinal ;
  long int i,n;
  unsigned int seed;
  int aleat, tmili;
  pthread_t t[MAX_THREADS];
  long int th;
  struct timeval inicio, final2;  /*timer structures parallel*/

  omp_set_num_threads(MAX_THREADS);

  n = 100000000;
  x = (double *) malloc (n*sizeof(double));
  y = (double *) malloc (n*sizeof(double));

  seed = time(NULL);

  aleat =  rand_r(&seed);
  if (aleat > RAND_MAX/3) sinal = 1; else sinal = -1;

  //start the clock serial
  gettimeofday(&inicio, NULL);

  //parallel program
  #pragma omp parallel private(i,temp,aleat,th) shared(x, y, n, sinal) reduction (+:SUMx, SUMy, SUMxy, SUMxx)
  {
      th = omp_get_thread_num();

      //input
      #pragma omp for
      for(i=0; i<n; i++) {
          x[i] = (float)i*10./(float)n;
          temp = (float)rand_r(&seed)/(float)RAND_MAX;
          aleat = rand_r(&seed)/10000;
          y[i] = temp*temp*sinal*(float)aleat;
          //x[i] = i; y[i] = i*2;
      }

      //Calculate
      #pragma omp for
      for(i=0; i<n; i++) {
        SUMx = SUMx + x[i];
        SUMy = SUMy + y[i];
        SUMxy = SUMxy + x[i]*y[i];
        SUMxx = SUMxx + x[i]*x[i];
      }
  }

  //stop the clock
  gettimeofday(&final2, NULL);

  slope = ( SUMx*SUMy - n*SUMxy ) / ( SUMx*SUMx - n*SUMxx );
  y_intercept = ( SUMy - slope*SUMx ) / n;
  tmili =     (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

  printf ("\nThe linear equation that best fits the given data:\n");
  printf ("       y = %6.2lfx + %6.2lf\n", slope, y_intercept);
  printf("\n\ntempo paralelo: %d milisegundos\n\n\n", tmili);
}