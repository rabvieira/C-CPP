#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define N 100000000
#define u 2.0

int main(int argc, char * argv[])
{
    int processId; /* rank dos processos */
    int noProcesses; /* Número de processos */
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    double passo, soma, x, *sum_loc, sum_;
    int i, qtde, iproc;
    int tamLocal = N/noProcesses;
    int pri = processId*tamLocal;

    sum_loc = (double *) malloc(noProcesses*sizeof(double));
    if (!sum_loc) {
        printf("Impossivel alocar sum_loc\n"); return(0);
    }

    if (processId==noProcesses-1) {
        qtde = N - pri;}
    else
        qtde = floor((float)N/noProcesses);

    printf("no %d/%d: pri=%d, qtde=%d, ultimo=%d\n", processId, noProcesses, pri, qtde, (pri+qtde));

    passo = (u-1) / (double) N;
    for (i=0; i<qtde;i++)
    {
        x=1+(i+pri)*passo;
        soma=soma+0.5*(1/x+1/(x+passo));
    }

    if (processId!=0) { /* Processos escravos */
        MPI_Send(&sum_loc[processId], 1, MPI_DOUBLE, 0, 12, MPI_COMM_WORLD);
      }
      else { /* Processo Mestre */
        for(iproc=1; iproc<processId; iproc++) {
          MPI_Recv(&sum_loc[iproc], 1, MPI_DOUBLE, iproc,
               12, MPI_COMM_WORLD, &status);
        }
    }

    if (processId==0) { /* Processo Mestre */
        sum_ = sum_loc[0];
        for(i=0; i<noProcesses; i++) {
          printf("MESTRE: i=%d, sum= %f\n", i, sum_loc[i]);
          fflush(stdout);
          sum_ += sum_loc[i];
        }
        printf("ln %f = %20.15f\n", u, sum_);
        //printf("ln %f = %20.15f\n", u, passo*soma);
    }
    MPI_Finalize();

    return 0;
}