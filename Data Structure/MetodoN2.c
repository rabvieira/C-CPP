/*
-----------------------------------------------------------------------------
 Nome      : MetodoN2.c
 Autores   : Danilo Augusto, Eduard Eric e Renan Arantes
 Descriçao : Este codigo consiste na implementacao do metodo N quadrado que
             consiste em n*n combinacoes entre os objetos.
-----------------------------------------------------------------------------
*/

#include<stdio.h>
#include <time.h>
#include <math.h>
#define W 20.00 //peso maximo que a mochila suporta
#define objetos 10 //numero de objetos disponiveis
#define max_p 9 //peso maximo de um objeto qualquer
#define max_v 9 //valor maximo de um objeto qualquer


//METODO ALTERNATIVO N2

//Estrutura do tipo objeto
typedef struct objeto{
    float peso,valor;
    int flag; // se 0 (esta fora da mochila), se 1 (esta dentro da mochila)
    struct objeto *prox;
}Tlista_circ;
typedef Tlista_circ *Plista_circ;

//Alocacao dinamica de memoria e inicializacao dos objetos
Plista_circ insere(Plista_circ l,float peso, float valor){
    Plista_circ novo = (Plista_circ)malloc(sizeof(Tlista_circ));
    novo->peso = peso;
    novo->valor = valor;
    novo->flag = 0;
    if(l == NULL){
        novo->prox = novo;
        return novo;
    }
    novo->prox = l->prox;
    l->prox = novo;
    return novo;
}

//funcao de liberacao de memoria da lista
void libera(Plista_circ l){
    Plista_circ p, ant = l;
    if(l!=NULL){
        for(p = l->prox; p != l; ){
            ant = p;
            p = p->prox;
            free(ant);
        }
    free(l);
    }
}
//calcula e retorna o peso atual da mochila
float peso_mochila(Plista_circ l){
    float peso_parcial = 0;
    int i;
    Plista_circ p=l;
    do{
        if(p->flag!=0){
            peso_parcial += p->peso;
        }
        p = p->prox;
    }while(p!=l);
    return peso_parcial;
}
//calcula e retorna o valor atual da mochila
float obtem_valor(Plista_circ l){
    float valor_parcial = 0;
    int i;
    Plista_circ p=l;
    do{
        if(p->flag!=0){
            valor_parcial += p->valor;
        }
        p = p->prox;
    }while(p!=l);
    return valor_parcial;
}
//Esvazia a mochila (desativa os flags)
void esvaziar_mochila(Plista_circ l){
    int i;
    Plista_circ p=l;
    do{
        if(p->flag!=0){
            p->flag = 0;
        }
        p = p->prox;
    }while(p!=l);
}

int main(){
    clock_t inicio, fim;
    Plista_circ p, q, lista = NULL;
    int i;
    float peso=0, peso_parcial=0, peso_maximo = 0;
    float valor=0, valor_parcial = 0, valor_maximo=0;
    srand(time(0));
//Valores iniciais randomicos para peso e valor e alocacao destes
    printf("A mochila suporta %.2f temos os objetos:\n\n",W);
    printf("\tvalor/peso\n");
    for(i=0 ; i<objetos ; i++){
        peso = 1+ rand()%max_p;
        valor = 1+ rand()%max_v;
        printf("[%d]\t%.2f %.2f\n",i+1,valor,peso);
        lista = insere(lista,peso,valor);
    }
    p=lista;
    q=p;
    inicio = clock();
    for(i=0 ; i<objetos ; i++){
        valor_parcial = 0;
        peso_parcial = 0;
        do{
            if(lista!=NULL)
                peso_parcial = peso_mochila(lista);
            if(peso_parcial + p->peso <= W) //se o peso que ja esta na mochila + o peso da vez <= peso suporte
                p->flag = 1; //coloca o peso na mochila
        p = p->prox;
        }while(p != q);
        if(lista!=NULL){
            peso_parcial = peso_mochila(lista);
            valor_parcial = obtem_valor(lista);
        }
        if(valor_parcial > valor_maximo){
            valor_maximo = valor_parcial;
            peso_maximo = peso_parcial;
        }
        if(lista!=NULL)
            esvaziar_mochila(lista);
        p = p->prox;
        q = p;
    }
    fim = clock();
    printf("\ntempo de execucao:  %g segundos\n",(double)(fim-inicio) / (double)CLOCKS_PER_SEC);
    printf("\n\nA mochila estara otimizada com:\n");
    printf("\n\tvalor maximo: %.2f\n\tpeso maximo:  %.2f\n\n",valor_maximo,peso_maximo);
    printf("\t\t\t\tconcluido!\n\n");
    libera(lista);
    return (0);
}
