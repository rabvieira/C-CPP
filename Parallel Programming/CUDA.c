#include <stdio.h>
#include <cuda.h>
#include <time.h>
#include <sys/time.h>

/* constantes numericas para calculo do metodo ftcs */
#define tam 1.0
#define dx 0.00001
#define dt 0.000001
#define T  0.01
#define kappa 0.000045

/* quantidade de threads por bloco */
#define THREADS_PER_BLOCK 64

typedef long long int lli;

__global__ void calcula(double *u, double *prev, double k, double t, double xx, int sz){
    lli i =  blockDim.x * blockIdx.x + threadIdx.x;
    if(i>0 && i<(sz-1))
        u[i] = prev[i] + k*t/(xx*xx)*(prev[i-1]-2*prev[i]+prev[i+1]);
}

__global__ void contorno(double *u, int n){
    lli i =  blockDim.x * blockIdx.x + threadIdx.x;
    if(i==1)
        u[0] = u[n] = 0.0;
}

int main(){
    /* variaveis do host */
    double  *u, *u_prev, *temp;
    double x = dx, t = 0.0, tmiliCuda;
    lli i, n, maxloc, nblocks, size;
    struct timeval inicioCuda, fimCuda;

    /* variaveis do device */
    double *u_u, *u_prev_prev;;

    /* calcula quantidade de pontos */
    n = tam/dx;

    /* calcula quantidade de blocos */
    nblocks = ceil((n+1)/THREADS_PER_BLOCK);

    /* aloca vetores no host */
    size = (n+1)*sizeof(double);
    u = (double *) malloc(size);
    u_prev = (double *) malloc(size);

    /* preenche vetor u_prev no host */
    for(i=1; i<n; i++){
        if(x<=0.5)
            u_prev[i] = 200.0*x;
        else       
            u_prev[i] = 200.0*(1.0-x);
        x += dx;
    }

    /* aloca vetores no device */
    cudaMalloc((void **)&u_u, size);
    cudaMalloc((void **)&u_prev_prev, size);

    /* copia valores do vetor u_prev do host para vetor u_prev_prev do device */
    cudaMemcpy(u_prev_prev, u_prev, size, cudaMemcpyHostToDevice);

    /* marca tempo inicial do processamento para medida de desempenho */   
    gettimeofday(&inicioCuda,NULL);

    while(t<T){
        /* processamento paralelo no device (equivalente ao for do programa serial) */
        calcula<<<nblocks,THREADS_PER_BLOCK>>>(u_u, u_prev_prev, kappa, dt, dx, n+1);

        /* sincroniza processamento no device para realizar troca de ponteiros
           antes da proxima iteracao do programa */       
        cudaDeviceSynchronize();

        /* condicao de contorno do metodo numerico ftcs */
        contorno<<<nblocks,THREADS_PER_BLOCK>>>(u_u, n);

        /* troca de ponteiros que atualiza informacoes para proxima iteracao */
        temp = u_u;
        u_u = u_prev_prev;
        u_prev_prev = temp;

        /* atualiza variavel de controle da estrutura de repeticao */
        t += dt;
    }

    /* marca tempo final do processamento para medida de desempenho */
    gettimeofday(&fimCuda,NULL);

    /* copia valores do vetor u_u do device para vetor u do host */
    cudaMemcpy(u, u_u, size, cudaMemcpyDeviceToHost);

    /* calcula maior valor do vetor e sua posicao */
    maxloc = 0;
    for(i=1; i<n+1; ++i)
        if(u[i] > u[maxloc])
            maxloc = i;
   
    /* calcula tempo de processamento para medida de desempenho */
    tmiliCuda = (double)(1000.0*(fimCuda.tv_sec-inicioCuda.tv_sec)+(fimCuda.tv_usec-inicioCuda.tv_usec)/1000.0);

    printf("FTCS :: CUDA(CPU+GPU)\n");
    printf("Maior valor u[%lld] = %g\n", maxloc, u[maxloc]);
    printf("%lld Pontos e %lld Iteracoes\n", (n+1), (lli)(T/dt));
    printf("%lld Blocos e %lld Threads por bloco\n", nblocks, (lli)THREADS_PER_BLOCK);
    printf("Tempo decorrido: %.3lf milissegundos\n", tmiliCuda);

    /* liberando memoria no host */
    free(u);
    free(u_prev);
    u = NULL;
    u_prev = NULL;

    /* liberando memoria no device */
    cudaFree(u_u);
    cudaFree(u_prev_prev);

    return 0;
}