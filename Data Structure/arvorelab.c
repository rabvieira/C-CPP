#include <stdio.h>
#include <stdlib.h>

/*Faça um programa completo que implemente árvores binárias cpm funções recursivas que
 a)busca o menor elemento da árvore;
 b)conta a quantidade de nós da árvore;
 c)soma o conteúdo de todos os nós da árvore;
 d)verifica se a árvore é completa;*/

typedef struct arvore{ //typedef
    int info; //info no lugar de val
    struct arvore *esq;
    struct arvore *dir;
 }TArvore;

 typedef TArvore *Parv; // * e tirei o struct

Parv cria (int val, Parv sae, Parv sad){
    Parv p;
    p = (Parv)malloc(sizeof(TArvore));
    p->info = val;
    p->esq = sae;
    p->dir = sad;
    return (p);
}

void imprime (Parv arv){
    if (arv!=NULL){
        printf("%d ", arv->info);
        imprime(arv->esq);
        imprime(arv->dir);
    }
}

Parv libera (Parv arv){
    if (arv!=NULL){
        arv->esq = libera(arv->esq); //-
        arv->dir = libera(arv->dir);
        free(arv);
    }
    else
        return (NULL);
}

int quant_nos(Parv arv){
    if(arv==NULL)
        return 0;
    else
        return(1+quant_nos(arv->esq)+quant_nos(arv->dir));
}

int soma (Parv arv){
    if (arv==NULL)
        return (0);
    else
        return (arv->info + soma(arv->esq)+soma(arv->dir)); // -
}

int estrit_bin(Parv arv){
    if(arv==NULL || (arv->esq==NULL && arv->dir==NULL))
        return 1;
    else if (arv->dir == NULL || arv->esq == NULL)
        return 0;
    else if (estrit_bin(arv->esq))
        return (estrit_bin(arv->dir));
    else
        return 0;
}

Parv busca_menor (Parv arv){
    Parv menor1, menor2;
    if (arv == NULL)
        return(NULL);
    else if (arv->esq == NULL && arv->dir==NULL)
            return(arv);
    else {
        menor1 = busca_menor(arv->esq);
        menor2 = busca_menor(arv->dir);
        if (menor1==NULL){ //menor2 é diferente de NULL
            if(arv->info < menor2->info)
                return (arv);
            else return (menor2);
        }
        else if(menor2==NULL){ //menor1 é diferente de NULL
            if(arv->info < menor1->info)
                return(arv);
            else return (menor1);
        } else{ //menor1 e menor2 são diferentes de NULL
        if(arv->info < menor1->info && arv->info < menor2->info)
            return (arv);
        else if(menor1->info < menor2->info)
            return (menor1);
        else return (menor2);
        }
    }
}

int main(){
    Parv arv,aux;
    int i,j;
    arv = cria(10, cria(20, cria(40, cria(70, NULL, NULL), NULL), cria(50, NULL, cria(80, NULL, NULL))), cria(30, cria(60, NULL, NULL), NULL));
    imprime(arv);
    i = soma(arv);
    printf("\nsoma: %d", i);
    aux = busca_menor(arv);
    printf("\nmenor elemento: %d",aux->info );
    j=quant_nos(arv);
    printf("\nquantidade de nos: %d",j );
    if(estrit_bin)
        printf("\nesta arvore e' estritamente binaria\n\n");
    else
        printf("\nesta arvore nao e' estritamente binaria\n\n");
    libera(arv);
}
