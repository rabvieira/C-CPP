/*
-----------------------------------------------------------------------------
 Nome      : ForcaBrutaRandomica.c
 Autores   : Danilo Augusto, Eduard Eric e Renan Arantes
 Descriçao : Este codigo consiste na implementacao do algoritmo de forca
             bruta com possibilidade de repeticoes.
-----------------------------------------------------------------------------
*/

#include<stdio.h>
#include <math.h>
#include <time.h>
#define max_p 9 //peso maximo de um objeto qualquer
#define max_v 9 //valor maximo de um objeto qualquer
#define tam_max_vetor 20 //tamanho maximo dos vetores
#define W 20 //peso maximo que a mochila suporta
#define objetos 10 //numero de objetos


//FORÇA BRUTA RANDOMICA

//Calcula o fatorial do numero de objetos
int fatorial (int n){
    if(n==1)
        return (1);
    else
        return n*fatorial(n-1);
}

//Retorna 1 se nao existe determinado valor no vetor de posicoes
int nao_existe(int posicoes[], int valor, int obj){
    int i;
    for(i=0 ; i<obj ; i++){
        if(posicoes[i]==valor)
            return (0);
    }
    return(1);
}

int main(){
    clock_t inicio, fim;
    float peso[tam_max_vetor],valor[tam_max_vetor];
    int i,j,k,fat=0,contador=0, posicao=0, flag=0, posicoes[tam_max_vetor];
    float peso_parcial=0, peso_maximo=0, valor_parcial=0, valor_maximo=0;
    int teste=0;
    for(i=0 ; i<objetos ; i++){
        valor[i] = 0;
        peso[i] = 0;
        posicoes[i] = -1;
    }
    srand(time(0));
//Valores iniciais randomicos para peso e valor
    printf("\n");
    printf("A mochila suporta %d, temos os objetos:\n\n",W);
    printf("\tvalor/peso\n");
    for(i=0 ; i<objetos ; i++){
        peso_parcial = 1 + rand()%max_p;
        valor_parcial = 1 + rand()%max_v;
        printf("[%d]\t%.2f %.2f\n",i+1,valor_parcial,peso_parcial);
        peso[i] = peso_parcial;
        valor[i] = valor_parcial;
    }
    fat = fatorial(objetos);
//    printf("\ncombinacoes randomicas:\n");
    valor_maximo=0;
    peso_maximo=0;
    inicio = clock();
    for(k=0 ; k<fat ; k++){
//Gera o vetor de posicoes aleatorias
        for(i=0 ; i<objetos ; i++){
            flag = 0;
            while(flag==0){
                posicao = rand()%objetos;
                if(posicoes[i]==-1 && nao_existe(posicoes,posicao,objetos)){
                    posicoes[i] = posicao;
                    flag=1;
                }
            }
        }
//para visualizar as combinacoes geradas, descomente
/*
        for(teste=0 ; teste<objetos ; teste++)
            printf("%d",1+posicoes[teste]);
        printf("\n");
*/
        peso_parcial=0;
        valor_parcial=0;
        for(j=0 ; j<objetos ; j++){
            if(peso[posicoes[j]] + peso_parcial<=W){
                peso_parcial += peso[posicoes[j]];
                valor_parcial += valor[posicoes[j]];
            }
        }
        if(peso_parcial>peso_maximo){
            peso_maximo = peso_parcial;
            valor_maximo = valor_parcial;
        }
        for(i=0 ; i<objetos ; i++)
            posicoes[i] = -1;
        contador++;
    }
    fim = clock();
    printf("\ntempo de execucao:  %g segundos\n",(double)(fim-inicio) / (double)CLOCKS_PER_SEC);
    printf("\n\nA mochila estara otimizada com:\n");
    printf("\n\tvalor maximo: %.2f\n\tpeso maximo:  %.2f\n\n",valor_maximo,peso_maximo);
    printf("chamadas da funcao:\n");
    printf("\t%d\n\n\n\t\t\tconcluido!\n\n",contador);
    return (0);
}
