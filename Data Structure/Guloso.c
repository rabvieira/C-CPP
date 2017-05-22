/*
-----------------------------------------------------------------------------
 Nome      : Guloso.c
 Autores   : Danilo Augusto, Eduard Eric e Renan Arantes
 Descriçao : Este codigo consiste na implementacao do algoritmo guluso para
             solucao do problema da mochila.
-----------------------------------------------------------------------------
*/
#include <time.h>
#include <stdio.h>
#define peso_limite 20 //peso maximo que a mochila suporta
#define n_objetos 10 //numero de objetos disponiveis


// GULOSO

int main(){
    clock_t inicio, fim;
    float peso,valor;
    int flag = 1;
    int adicionados[n_objetos];
    float valora = 0;
    float v[n_objetos];
    float w[n_objetos];
    int melhor = 0;
    float peso_acumulado = 0;
    float pl = peso_limite;
    int i,j = 0;
    float va = 0;
    float seg = 0;
    srand(time(0));
//adiciona os objetos na mochila
    for(i = 0; i < n_objetos; i++){
        v[i] = rand()%9 + 1;
        w[i] = rand()%9 + 1;
        adicionados[i] = -1;
    }

//imprime os objetos
    printf("A mochila suporta %d temos os objetos:\n\n",peso_limite);
    printf("\tvalor/peso   v/p\n");
    for(i = 0; i < n_objetos; i++){
        printf("[%d]\t%.2f %.2f   %.2f\n",i,w[i],v[i],v[i]/w[i]);
    }
    //flag para notificar que todos objetos foram tentados ser adicionados na mochila
inicio = clock();
    while(flag){
        //laço para varrer todos os objetos, comparando a relação valor/peso
        //a variavel melhor armazena a posicao do objeto com melhor razao valor/peso
        for(i = 0; i < n_objetos; i++){
            if(w[i] !=0 && va < v[i]/w[i]){
                melhor = i;
                va = v[i]/w[i];
            }
        }
        //laço para adicionar o objeto selecionado verificando se nao foi passado o peso limite da mochila
        //caso o objeto seja adicionado ele recebe 0, para que ele nao seja adicionado de novo
        if(peso_acumulado + w[melhor] <= pl){
            peso_acumulado+=w[melhor];
            valora+=v[melhor];
            adicionados[j] = melhor;
            j++;
            v[melhor] = 0;
            w[melhor] = 0;
            melhor = 0;
            va = 0;
        }
        //caso o objeto nao seja adicionado por que excedeu o peso limite... ele é zerado, notificando que ja foi tentado
        // adicionar o objeto em questão.
        else{
            v[melhor] = 0;
            w[melhor] = 0;
            melhor = 0;
            va = 0;
        }
        flag = 0;
        //laço para verificar se ja foi tentado adicionar todos os objetos, acionando ou nao o flag
        for(i = 0; i < n_objetos && !flag; i++){
            if(w[i] == 0)
                flag = 0;
            else
                flag = 1;
        }
    }
fim = clock();
printf("\n\ntempo de execucao: %g\n\n",(double)(fim-inicio) / (double)CLOCKS_PER_SEC);

//imprime os objetos adicionados
    printf("\n\nn-esimos objetos adicionados:\n\n");
    for(i = 0; i < n_objetos; i++){
        if(adicionados[i] != -1)
            printf(" %d ",adicionados[i]);
    }
    printf("\n\npeso acumulado:  %.1f",peso_acumulado);
    printf("\nvalor adquirido: %.1f\n",valora);
    printf("\t\t\t\tconcluido!\n\n");
    return 0;
}
