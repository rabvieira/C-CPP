/*
-----------------------------------------------------------------------------
 Nome      : t_llgc.c
 Autor     : Renan A. B. Vieira
 Descriçao : Learning with Local and Global Consistency Algorithm
-----------------------------------------------------------------------------
*/

#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define l 3       //linhas do dataset
#define c 3       //colunas do dataset
#define caract 2  //quantidade de caracteres de uma informacao do dataset
#define sigma 0.1 //variancia
#define alfa 0.5  //alfa (0,1)

//Imprime matriz
void print_matrix(igraph_matrix_t *m, FILE *f) {
    long int i, j;
    for (i=0; i<igraph_matrix_nrow(m); i++) {
        for (j=0; j<igraph_matrix_ncol(m); j++) {
            //fprintf(f, "%li\t", (long int)MATRIX(*m, i, j));
            fprintf(f, "%.5f\t", (float)MATRIX(*m, i, j));
        }
        fprintf(f, "\n");
    }
}

//Imprime vetor
void print_vector(igraph_vector_t *v, FILE *f) {
    long int i;
    for (i=0; i<l; i++) {
        //fprintf(f, "%li\t", (long int)MATRIX(*m, i, j));
        fprintf(f, "%.5f\t", (float)VECTOR(*v)[i]);
    }
}

//Multiplicacao de matrizes
int mul_matrix(igraph_matrix_t *A, igraph_matrix_t *B, igraph_matrix_t *res){
    int i,j,k;
    if(igraph_matrix_ncol(A)!=igraph_matrix_nrow(B))
        return (-1);
    igraph_matrix_init(res, igraph_matrix_nrow(A), igraph_matrix_ncol(B));
    igraph_matrix_null(res);
    for(i=0 ; i<igraph_matrix_nrow(A) ; i++)
        for(j=0 ; j<igraph_matrix_ncol(B) ; j++)
            for(k=0 ; k<igraph_matrix_ncol(A) ; k++)
                MATRIX(*res, i, j) += MATRIX(*A, i, k) * MATRIX(*B, k, j);
    return (0);
}

//Cholesky Decomposition
int cholesky(igraph_matrix_t *m, igraph_matrix_t *res){
    int i,j,k;
    float s=0, d=0;
    if(igraph_matrix_nrow(m)!=igraph_matrix_ncol(m))
        return (-1);
    igraph_matrix_init(res, igraph_matrix_nrow(m), igraph_matrix_nrow(m));
    igraph_matrix_null(res);
    for(i=0 ; i<igraph_matrix_nrow(m) ; i++){
        s = 0;
        for(k=0 ; k<i ; k++)
            s += pow(MATRIX(*res, k, i),2);
        d = MATRIX(*m, i, i) - s;
        if(fabs(d)<0.00001)
            MATRIX(*res, i, i) = 0;
        else{
            if(d<0){
                printf("Matrix not positive-definite");
                return (-1);
            }
            MATRIX(*res, i, i) = sqrt(d);
        }
        for(j=i+1 ; j<igraph_matrix_nrow(m) ; j++){
            s = 0;
            for(k=0 ; k<igraph_matrix_nrow(m) ; k++)
                s += MATRIX(*res, k, i) * MATRIX(*res, k, j);
                if(fabs(s)<0.00001)
                    s = 0;
                MATRIX(*res, i, j) = (MATRIX(*m, i, j) - s) / MATRIX(*res, i, i);
        }
    }
    return (0);
}

void cholesky_inverse(igraph_matrix_t *aux, igraph_matrix_t *inv){
    int i,j,k,t;
    float s=0, tjj=0, sum=0;
    igraph_matrix_init(inv, igraph_matrix_nrow(aux), igraph_matrix_nrow(aux));
    igraph_matrix_null(inv);
    for(j=igraph_matrix_nrow(aux)-1 ; j>=0 ; j--){
        s=0;
        tjj = MATRIX(*aux, j, j);
        for(k=j+1 ; k<igraph_matrix_nrow(aux) ; k++)
            s +=  MATRIX(*aux, j, k) * MATRIX(*inv, j, k);
        MATRIX(*inv, j, j) = ((1.0/pow(tjj,2)) - (s/tjj));
        for(i=j-1 ; i>=0 ; i--){
            sum=0;
            for(t=i+1 ; t<igraph_matrix_nrow(aux) ; t++)
                sum += MATRIX(*aux, i, t) * MATRIX(*inv, t, j);
            MATRIX(*inv, i, j) = -sum/MATRIX(*aux, i, i);
            MATRIX(*inv, j, i) = MATRIX(*inv, i, j);
        }
    }
}

void inverse_matrix(igraph_matrix_t *m, igraph_matrix_t *inv){
    igraph_matrix_t aux;
    cholesky(m,&aux);
    cholesky_inverse(&aux,inv);
//    print_matrix(&aux, stdout);
//    print_matrix(&inv, stdout);
    igraph_matrix_destroy(&aux);
}

int max_row(igraph_matrix_t *m, int lin){
    int j,index=0;
    float max;
    max = MATRIX(*m,lin,0);
    for(j=0 ; j<igraph_matrix_ncol(m) ; j++){
        if(MATRIX(*m,lin,j)>max){
            max = MATRIX(*m,lin,j);
            index = j;
        }
    }
    return (index);
}

//Rotula a matriz F
void rotula_matrix(igraph_matrix_t *F, igraph_matrix_t *prod){
    int i,j;
    igraph_matrix_init(F, igraph_matrix_nrow(prod), igraph_matrix_ncol(prod));
    igraph_matrix_null(F);
    for(i=0 ; i<igraph_matrix_nrow(prod) ; i++){
            MATRIX(*F,i,max_row(prod,i)) = 1;
    }
}

//Calcula RBF Kernel
float exp_x(igraph_matrix_t *m, int i, int j){
    float d=0;
    int k;
    for(k=0 ; k<c-1 ; k++)
        d += pow(MATRIX(*m, i, k) - MATRIX(*m, j, k),2);
    d = sqrt(d);
    d = fabs(d);
    d = pow(d,2);
    return exp(-d / 2 * pow(sigma,2));
}

//Soma da i-esima linha da matriz W
float sum_W(igraph_matrix_t *W, int i){
    int k;
    float sum=0;
    for(k=0 ; k<igraph_matrix_ncol(W) ; k++){
        sum += MATRIX(*W, i, k);
    }
    return (sum);
}

//Calcula -alfa*S/raizq(di)*raizq(dj)
float alfa_S(igraph_matrix_t *W, igraph_vector_t *D, int i, int j){
    float aw=0;
    aw = - alfa*MATRIX(*W, i, j);
    return (aw / (sqrt(VECTOR(*D)[i]) * sqrt(VECTOR(*D)[j])));
}

//Código que implementa o LGC
int LLGC(char title[]){
    igraph_matrix_t dataset;  //dataset
    igraph_matrix_t matrix_Y; //matriz de rotulos
    igraph_matrix_t matrix_W; //matriz de pesos
    igraph_vector_t D;        //vetor D' (diagonal)
    igraph_matrix_t matrix_X; //matriz X=I - alfa*S
    igraph_matrix_t matrix_inv_X; //matriz inversa de X
    igraph_matrix_t matrix_prod; //produto X(-1)Y
    igraph_matrix_t matrix_F; //matriz de rotulos final
    char buf[caract];
    int i, j;
    FILE *instream;
    instream = fopen(title,"r");
    if (instream == NULL){
        printf("Erro ao abrir o arquivo para leitura\n");
        return (1);
    }

//Armazena o dataset em uma matriz (l x c)
    igraph_matrix_init(&dataset, l, c);
    for(i=0 ; i<l ; i++){
        for(j=0 ; j<c ; j++){
            fscanf(instream,"%s",buf);
            MATRIX(dataset, i, j) = atof(buf);
        }
    }

//Gera a matriz de rotulos (l x c-1)
    igraph_matrix_init(&matrix_Y, l, c-1);
    igraph_matrix_null(&matrix_Y);
    for(i=0 ; i<l ; i++){
        switch((int)MATRIX(dataset, i, c-1)){
            case 1:
                MATRIX(matrix_Y, i, 0)=1;
            break;
            case 2:
                MATRIX(matrix_Y, i, 1)=1;
            break;
        }
    }

//Gera a matriz de pesos (l x l)
    igraph_matrix_init(&matrix_W, l, l);
    for(i=0 ; i<l ; i++){
        for(j=0 ; j<l ; j++){
            if(i==j)
                MATRIX(matrix_W, i, j) = 0;
            else
                MATRIX(matrix_W, i, j) = exp_x(&dataset,i,j);
        }
    }

//Gera o vetor D'
    igraph_vector_init(&D, l);
    for(i=0 ; i<l ; i++)
        VECTOR(D)[i] = sum_W(&matrix_W,i);

//Gera a matrix X (X = I - alfa*S), onde {S = Wij / sqrt(Dii)*sqrt(Djj)} para i diferente de j e 1 caso contrario
    igraph_matrix_init(&matrix_X, l, l);
    for(i=0 ; i<l ; i++){
        for(j=0 ; j<l ; j++){
            if(i==j)
                MATRIX(matrix_X, i, j) = 1;
            else
                MATRIX(matrix_X, i, j) = alfa_S(&matrix_W,&D,i,j);
        }
    }

//Inverte a matrix X
    inverse_matrix(&matrix_X,&matrix_inv_X);

//Multiplica X(-1)Y
    mul_matrix(&matrix_inv_X,&matrix_Y,&matrix_prod);

//Gera a matriz F
    rotula_matrix(&matrix_F,&matrix_prod);

//    printf("Dataset\n");
//    print_matrix(&dataset, stdout);
//    printf("\nMatriz de Rotulos Y\n");
//    print_matrix(&matrix_Y, stdout);
//    printf("\nMatriz de Pesos W\n");
//    print_matrix(&matrix_W, stdout);
//    printf("\nVetor D'\n");
//    print_vector(&D, stdout);
//    printf("\nMatriz X\n");
//    print_matrix(&matrix_X, stdout);
//    printf("\nMatriz Inversa de X\n");
//    print_matrix(&matrix_inv_X, stdout);
    printf("\nF = X(-1)Y\n");
    print_matrix(&matrix_prod, stdout);
    printf("\nF (dados rotulados)\n");
    print_matrix(&matrix_F, stdout);
//    printf("\n");

    fclose(instream);
    igraph_matrix_destroy(&dataset);
    igraph_matrix_destroy(&matrix_Y);
    igraph_matrix_destroy(&matrix_W);
    igraph_vector_destroy(&D);
    igraph_matrix_destroy(&matrix_X);
    igraph_matrix_destroy(&matrix_inv_X);
    igraph_matrix_destroy(&matrix_prod);
//    igraph_matrix_destroy(&matrix_F);
    return (0);
}

int main(void){
    LLGC("test.txt");
    return (0);
}