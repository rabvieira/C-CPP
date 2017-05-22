/*
-----------------------------------------------------------------------------
 Nome      : NeuroniosVizinhos.c
 Autores   : Danilo Augusto, Eduard Eric e Renan Arantes
 Descriçao : Simulacao de rede localmente acoplada
-----------------------------------------------------------------------------
*/
#include<stdio.h>
#include <math.h>
#define passo 0.01 //delta_t
#define epsilon 0.4 //parametro da equacao do oscilador de relaxamento
#define alfa 6.0 //parametro da sigmoide
#define beta 0.1 //parametro da sigmoide
#define tempo_final 10000 //tempo final de simulacao
#define theta 1.5 //limiar da funcao heaviside
#define neuronios 101 //quantidade de neuronios
#define ajuste_min 0.99 //range minimo
#define ajuste_max 1.01 //range maximo
#define neta 0.1 //forca de acoplamento
#define porcentagem 10 //% de neuronios inativos
//Estrutura do tipo neuronio
typedef struct neuronio{
    float x,y,I;
    struct neuronio *prox;
    struct neuronio *ant;
}Tlista_circ;

typedef Tlista_circ *Plista_circ;
//Alocacao dinamica de memoria para lista duplamente encadeada circular
Plista_circ insere(Plista_circ l,float x,float y,float I){
    Plista_circ novo = (Plista_circ)malloc(sizeof(Tlista_circ));
    novo->x = x;
    novo->y = y;
    novo->I = I;
    Plista_circ p;
    p = l;
    if(l == NULL){
        novo->prox = novo;
        novo->ant = novo;
        return novo;
    }
    while(p->prox != l)
        p = p->prox;
    p->prox = novo;
    novo->ant = p;
    novo->prox = l;
    l->ant = novo;
    return l;
}
//funcao de liberacao de memoria da lista
void libera(Plista_circ l){
    Plista_circ p, ant = l;
    if(l!=NULL){
        for(p = l->prox; p!=l; ){
            ant = p;
            p = p->prox;
            free(ant);
        }
    free(l);
    }
}
//funcao heaviside (retorna se ha ou nao interacao)
int heaviside(float a){
    if(a - theta >= 0)
        return (1);
    else
        return (0);
}
//Calculo do parametro S (interacao entre neuronios vizinhos)
int calculo_s(Plista_circ p, Plista_circ q){
    int s = 0;
    s = s + heaviside(p->x) + heaviside (q->x);
    return s;
}
//Cria um intervalo com intuito de analizar a sincronizacao entre os neuronios
int range(float p, float q){
    float p_min, p_max;
    p_min = ajuste_min*p;
    p_max = ajuste_max*p;
    if(q>=p_min && q<=p_max)
        return (1);
    else
        return (0);
}

int main(){
    Plista_circ p, q, r, lista = NULL;
    int i, log,flag=0, ocorrencia;
    float S,ro,y,x,I,tempo = 0;
    FILE *out1;
    FILE *out2;
    out1 = fopen ("matriz.txt","w");
    out2 = fopen ("tempo.txt","w");
    srand(time(0));
//Valores iniciais para x e y e alocacao destes
    for(i = 0; i<neuronios; i++){
        x = rand()%10;
        y = rand()%10;
        ocorrencia = rand()%100;
        if(ocorrencia > porcentagem)
            I = 0.2;
        else
            I = - 0.1;
        fprintf(out1,"%2.2f \t",x);
        lista = insere(lista,x,y,I);
    }
    fprintf(out1,"\n");
    p=lista;
    fprintf(out2,"%2.2f \n",tempo);
//integracao pelo metodo de Euler
    while(tempo <= tempo_final){
        for(i = 0; i<neuronios; i++){
            ro = rand()% 1000 - 0.5;
            ro = ro/10000;
            tempo = tempo +  passo;
            S = calculo_s(p->ant,p->prox);
            p->x = p->x + (3*p->x - pow(p->x,3) + 2 - p->y + p->I + ro + neta*S)*passo;
            p->y = p->y + (epsilon*(alfa*(1 + tanh(p->x/beta)) - p->y))*passo;
            p = p->prox;
        }
//Escreve os dados no arquivo texto
        q = p;
        do{
            fprintf(out1,"%2.2f \t",p->x);
            p = p->prox;
        }while(p != q);
        fprintf(out1,"\n");
        fprintf(out2,"%2.2f \n",tempo);
//Analiza o grau de sincronizacao dos neuronios
        r = p;
        q = p->prox;
        log=1;
        do{
            if(range(p->x, q->x)){
                p = p->prox;
                q = q->prox;
            }
            else
                log = 0;
        }while(q != r && log==1);
        if(log==1 && flag==0){
            printf("tempo minimo de sincronizacao: %.2f\n",tempo);
            flag = 1;
        }
    }
    printf("\t\t\t\tconcluido!\n\n");
    fclose(out1);
    fclose(out2);
    libera(lista);
    return (0);
}
