#include <stdio.h>

#include <stdlib.h>

#define max_tam 10

typedef struct{

    int vet[max_tam];

    int pos;

}TPilha;

 

TPilha *nova(){

    TPilha* np = (TPilha*) malloc (sizeof (TPilha));

    np->pos = -1;

return np;}

 

int push(TPilha *p, int valor){

    if(p->pos == max_tam-1) return (-1); //pilha cheia

    else {

        p->pos+=1;

        p->vet[p->pos]=valor;

        return (1);

    }

}

 

int pop(TPilha *p, int *valor){

    if(p->pos==-1) return (-1);

    else {

    *valor = p->vet[p->pos];

    p->pos-=1;

    return (1);

    }

}

 

void libera(TPilha *p){

    free (p);

}

int main(){

    TPilha *pilha;

    int n;

    pilha = NULL;

    pilha = nova();

    if(push(pilha,10)==-1)

        printf("Estouro de pilha!\n");

        else if (push(pilha,20)==-1)

            printf("Estouro de pilha\n");

    if(pop(pilha,&n)==-1)

    //lista_pilha(pilha);

        printf("Underflow de pilha!\n");

    else {

        printf("n=%i\n",n);

        if(pop(pilha,&n==-1))

            printf("Underflow!\n");

        else printf("n=%i\n",n);

    }

    libera(pilha);

}

 

 

PILHA

 

#include<stdio.h>

 

typedef struct pilha{

    int info;

    struct pilha *prox;

}Tpilha;

 

typedef Tpilha *Ppilha;

 

Ppilha push(Ppilha p,int v){

    Ppilha novo = (Ppilha)malloc(sizeof(Tpilha));

    novo->info = v;

    novo->prox = p;

    return novo;

}

 

Ppilha pop(Ppilha p){

    if(p != NULL){

        Ppilha k = p->prox;

        free(p);

        return k;

    }

    else{

        return NULL;

    }

}

 

void imprime(Ppilha p){

    printf("\n");

    if(p != NULL){

    do{

        printf("%5d",p->info);

        p = p->prox;

    }while(p != NULL);

    printf("\n");

    }

    else printf("\nlista vazia\n");

}

 

void libera(Ppilha p){

    Ppilha k;

    if( p != NULL ){

    do{

        k = p->prox;

        free(p);

        p = k;

    }while(p != NULL);

    }

}

 

int main(){

    Ppilha pilha = NULL;

    int val;

    char ver = 's';

 

    while(ver != 'n'){

        printf("digite o valor:");

        scanf("%d",&val);

        pilha = push(pilha,val);

        printf("deseja empilhar mais alguma coisa??:");

        fflush(stdin);

        scanf("%c",&ver);

    }

 

    imprime(pilha);

 

    printf("aperte enter para desempilhar:");

        getch();

        pilha = pop(pilha);

        imprime(pilha);

    printf("aperte enter para desempilhar:");

        getch();

        pilha = pop(pilha);

        imprime(pilha);

    libera(pilha);

 

    return 0;

 

}

 

FILA

 

#include<stdio.h>

 

typedef struct no{

    int info;

    struct no* prox;

} TNo;

typedef TNo *PNo;

 

typedef struct Fila{

    PNo ini,fim;

} TFila;

typedef TFila *PFila;

 

PFila cria(){

    PFila f = (PFila) malloc(sizeof(TFila));

    f->ini = f->fim = NULL;

    return (f);

}

 

void insere(PFila f){

    PNo novo;

    novo = (PNo) malloc(sizeof (TNo));

    novo->prox = NULL;

    printf("Digite um valor:\t");

    scanf("%i", &novo->info);

    if(f->fim == NULL)

        f->ini=novo;

    else f->fim->prox=novo;

    f->fim=novo;

}

 

void imprimir(PFila f){

    PNo q;

    q=f->ini;

    while(q!=NULL){

        printf("%d\n", q->info);

        q=q->prox;

    }

}

 

void retirar(PFila f){

    PNo q;

    q=f->ini;

    f->ini=f->ini->prox;

    printf("Removendo %d\n",q->info);

    free(q);

}

 

void liberar(PFila f){

    PNo q;

    for (q=f->ini ; q!=NULL ; q=f->ini){

        f->ini = q->prox;

        free(q);

    }

    free(f);

}

 

int main(){

    PFila fila;

    int op;

    fila = cria();

    do{

        printf("(1) Inserir\n");

        printf("(2) Imprimir a fila\n");

        printf("(3) Retirar\n");

        printf("(4) Sair do programa\n");

        scanf("%i",&op);

    switch(op){

        case 1 : insere(fila);

            break;

        case 2 : imprimir(fila);

            break;

        case 3 : retirar(fila);

            break;

        case 4 : liberar(fila);

            break;

        }

    }while(op!=4);

return 0;

}

 

LISTA

 

#include <stdio.h>

 

typedef struct lista{

    int info;

    struct lista* prox;

}Tlista;

 

typedef Tlista *Plista;

 

 

Plista insere(Plista l,int v){

    Plista novo = (Plista)malloc(sizeof(Tlista));

    novo->info = v;

    novo->prox = l;

    return novo;

}

 

void imprime(Plista l){

    Plista p = l;

    if(p->prox == NULL){

        printf("%5d",p->info);

        getch();

        return;

    }

    else{

        printf("%5d",p->info);

        imprime(p->prox);

    }

}

 

Plista retira(Plista l){

    Plista ant = NULL;

    Plista p;

    int v;

    printf("digite a remocao:");

    scanf("%d",&v);

    for(p = l;p != NULL && p->info != v;p = p->prox)

        ant = p;

 

    if(p == NULL) return l;

    if(ant == NULL)

        l = p->prox;

    else

        ant->prox = p->prox;

        free(p);

    return l;

    }

 

 

void busca(Plista l){

    Plista p = l;

    int v = 1;

    int val;

    printf("digite a busca:");

    scanf("%d",&val);

 

    while(p!=NULL && v){

        if(p->info == val)

            v = 0;

        else p = p->prox;

    }

    if(v) printf("nao encontrado");

    else  printf("encontrado");

 

    }

 

void libera(Plista l){

    Plista p;

    for(p = l; l != NULL; p = l){

    l = p -> prox;

    free(p);

    }

}

 

int main(){

    Plista lista = NULL;

    int op,v,k;

    do{

        printf("1)inserir\n2)imprime\n3)busca\n4)remover\n5)sair\n");

        printf("opcao:");

        scanf("%d",&op);

        switch(op){

            case 1:

                printf("digite info:");

                scanf("%d",&v);

                lista = insere(lista,v);

                break;

            case 2:

                imprime(lista);

                break;

            case 3:

                busca(lista);

                getch();

                break;

            case 4:

                lista = retira(lista);

                break;

            case 5:

                libera(lista);

        }

        system("cls");

    }while (op != 5);

return 0;

    }

 

 

CIRCULAR

 

#include<stdio.h>

#include<string.h>

 

typedef struct soldados{

    int info;

    struct soldados *prox;

}Tlista_circ;

 

typedef Tlista_circ *Plista_circ;

 

Plista_circ insere(Plista_circ l,int info){

    Plista_circ novo = (Plista_circ)malloc(sizeof(Tlista_circ));

    novo->info = info;

    if(l == NULL){

        novo->prox = novo;

        return novo;

    }

    novo->prox = l->prox;

    l->prox    = novo;

    return novo;

}

 

void imprime(Plista_circ l){

    Plista_circ p = l;

    if(l != NULL){

    do{ p = p->prox;

        printf("%5d",p->info);

    }while(p != l);

  }

}

 

void libera(Plista_circ l){

    Plista_circ p,ant = l;

    for(p = l->prox;p != l; ){

        ant = p;

        p = p->prox;

        free(ant);

    }

    free(l);

}

 

int main(){

    Plista_circ lista = NULL;

    int op,v,k;

    do{

        printf("1)inserir\n2)imprime\n3)busca\n4)remover\n5)sair\n");

        printf("opcao:");

        scanf("%d",&op);

        switch(op){

            case 1:

                printf("digite info:");

                scanf("%d",&v);

                lista = insere(lista,v);

                break;

            case 2:

                imprime(lista);

                break;

            case 4:

                break;

            case 5:

                libera(lista);

        }

        getch();

        system("cls");

    }while (op != 5);

return 0;

    }

 

 

 

DUPLA CIR

 

#include<stdio.h>

#include<stdlib.h>

 

 

typedef struct lista_dupla{

    int info;

    struct lista_dupla *ant;

    struct lista_dupla *prox;

}TLista, *PLista;

 

PLista insere(PLista lista,int val){

    PLista novo, p;

    novo=(PLista)malloc(sizeof(TLista));

    novo->info=val;

    if(lista==NULL){

        novo->prox=novo;

        novo->ant=novo;

        return(novo);

    }

p=lista;

do{

    if(p->info<val)

        p=p->prox;

    }while(p->info<val&& p!=lista->ant);

    if (p->info<val){

        novo->ant=p;

        novo->prox=lista;

        p->prox=novo;

        lista->ant=novo;

        return (lista);

    }

    else{

        novo->ant=p->ant;

        novo->prox=p;

        p->ant->prox=novo;

        p->ant=novo;

        if(p==lista)

            return(novo);

        else{

            return(lista);

        }

    }

}

void imprime(PLista lista){

    PLista p=lista;

    if(p!=NULL)

        do{

            printf("%d",p->info);

            p=p->prox;

        }while(p!=lista);

}

void busca(PLista lista, int par){

    PLista p=lista;

    if(lista==NULL)

        printf("Erro detectado, digite a lista\n");

    else{

    int encontrou=0;

    p=p->prox;

    do{

        if(p->info==par){

            encontrou=encontrou+1;

        }

        p=p->prox;

    }while (p!=lista);

    if (encontrou!=0){

            printf("Posição encontrada com sucesso\n");

        if (encontrou==1)

            printf("O valor foi obtido %d vez\n",encontrou);

        else

            printf("O valor foi obtido %d vezes\n",encontrou);

 

 

}

    else{

        printf("Valor não encontrado\n");

    }

}

}

PLista busca_retira(PLista lista, int val){

    PLista p;

    p=lista;

    if (lista==NULL){

        printf("Problema ocorrido, lista vazia");

    }

    else{

    p=p->prox;

    do{

        if(p->info==val){

            return (p);

        }

        p=p->prox;

    }while(p!=lista);

}

return(NULL);

}

PLista retira(PLista lista, int val){

    PLista p,q;

    p=busca_retira(lista,val);

    if(p!=NULL){

        q=p->ant;

        while(p->info==val){

            if(p==p->prox){

                lista=NULL;

                free(p);

                return(lista);

            }

            if(p==lista){

                lista=p->prox;

                lista->ant=p->ant;

                p->ant->ant=lista;

            }

            else{

                p->ant->prox=p->prox;

                p->prox->ant=p->ant;

                }

            free(p);

            p=q->prox;

            }

        }

    return (lista);

    }

 

void libera(PLista lista){

    PLista p,q;

    for(p=lista->prox;p!=lista;p=q){

        q=p->prox;

        free(p);

    }

    free(lista);

}

int main(){

printf("\t\t/////////////////////////////////////////\n");

printf("\t\t///////////  LISTA-DUPLA.C  /////////////\n");

printf("\t\t/////////////////////////////////////////\n");

printf("\n\n\n");

 

PLista P1=NULL;

int val, escolha=0;

while(escolha!=5){

    printf("\n\t\tME NU DE OPCOES DO PROGRAMA\n");

    printf("\tOperacoes suportadas:\n");

    printf("Digite 1 - Inserir Lista\n");

    printf("Digite 2 - Buscar Elementos\n");

    printf("Digite 3 - Retirar Elementos\n");

    printf("Digite 4 - Imprime Elemento\n");

    printf("Digite 5 - Sair\n");

    printf("\t\tQual opcao deseja seguir?\n");

    scanf("%d",&escolha);

    switch(escolha){

        case 1:

            printf("Iniciando insercao\n");

            printf("Digite o valor que deseja passar com o parametro\n");

            scanf("%d",&val);

            P1=insere(P1,val);

            break;

 

 

        case 2:

            if (P1==NULL){

                printf("\t\tOcorreu um erro, entre com os valores para prosseguir\n\n");

                printf("--------------------------------------------------------------------------------\n\n");

            }

            else{

                printf("Qual valor deseja buscar?\n");

                scanf("%d",&val);

                busca(P1,val);

            }

            break;

 

        case 3:

            if(P1==NULL){

                printf("\t\tOcorreu um erro, entre com os valores para prosseguir\n\n");

                printf("--------------------------------------------------------------------------------\n\n");

            }

            else{

                printf("Iniciando Retirada de Elemento\n");

                printf("Qual valor deseja retirar da lista?");

                scanf("%d",&val);

                P1=retira(P1,val);

            break;

 

        case 4:

            if(P1==NULL){

                printf("\t\tOcorreu um erro, entre com os valores para prosseguir\n\n");

                printf("--------------------------------------------------------------------------------\n\n");

            }

            else{

                printf("Iniciando impressão\n");

                imprime(P1);

            }

            break;

 

        case 5:

            break;

        }

 

    }

}

    printf("\n\n\nEncerrando programa...\n\n");

    if (P1!=NULL)

       libera(P1);

    return 0;

}

 

DUP CIRC LINEAR

 

#include <stdio.h>

 

typedef struct lista_dupla{

    int info;

    struct lista_dupla* prox;

    struct lista_dupla* ant;

}Tlista_dupla;

 

typedef Tlista_dupla *Plista_dupla;

 

Plista_dupla insere(Plista_dupla l,int val){

    Plista_dupla p = l;

    Plista_dupla novo = (Plista_dupla)malloc(sizeof(Tlista_dupla));

    novo->info = val;

 

    if(l == NULL){

        novo -> prox = NULL;

        novo -> ant  = NULL;

        return novo;

    }

 

    while(p -> prox != NULL && p->info < val)

        p = p->prox;

        if(p -> info < val){

            novo -> ant = p;

            novo -> prox = NULL;

            p->prox = novo;

            return l;

        }

        novo -> prox = p;

        novo->ant = p->ant;

 

        if(p == l){

            p->ant = novo;

            return novo;

        }

        p->ant->prox = novo;

        p->ant = novo;

        return l;

        }

 

void imprime(Plista_dupla l){

    Plista_dupla p = l;

 

    while(p != NULL){

        printf("%5d",p->info);

        p = p->prox;

    }

}

 

void libera(Plista_dupla l){

    Plista_dupla p = l;

    while(l->prox != NULL){

        l = l->prox;

        p = l->ant;

        free(p);

    }

    free(l);

}

 

int main(){

    Plista_dupla lista = NULL;

    int op,v;

    do{

        printf("1)inserir\n2)imprime\n5)sair\n");

        printf("opcao:");

        scanf("%d",&op);

        switch(op){

            case 1:

                printf("digite info:");

                scanf("%d",&v);

                lista = insere(lista,v);

                break;

            case 2:

                imprime(lista);

                getch();

                break;

            case 5:

                libera(lista);

                break;

        }

        system("cls");

    }while (op != 5);

return 0;

    }