#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define N 100000000
#define u 2.0

int main(int argc, char * argv[])
{
    int processId; /* rank dos processos */
    int noProcesses; /* Número de processos */
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);    
    
    double passo, soma,x;
    int i, qtde;
    int tamLocal = N/noProcesses;
    int pri = processId*tamLocal;
    
    if (processId==noProcesses-1) {
        qtde = N - pri;}
    else
        qtde = floor((float)N/processId);
    
    passo = (u-1) / (double) N;
    for (i=0; i<N;i++)
    {
        x=1+i*passo;
        soma=soma+0.5*(1/x+1/(x+passo));
    }
    printf("ln %f = %20.15f\n", u, passo*soma);

    MPI_Finalize();

    return 0;
}