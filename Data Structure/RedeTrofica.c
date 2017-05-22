/*
-----------------------------------------------------------------------------
 Nome      : RedeTrofica.c
 Autores   : Danilo Augusto, Eduard Eric e Renan Arantes
 Descriçao : Este codigo foi utilizado para gerar todas as simulacoes, em que
             basicamente ha integracoes numericas das equacoes de cada especie
             da rede trofica. As perturbacoes estao comentadas.
-----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <math.h>
#define passo 0.01
#define alfa 0.1 //taxa de crescimento da vegetacao (modelo logistico)
#define k 5 //espaco limitante para vegetacao
#define tempo_final 2000 //tempo de simulacao

//parametros vegetacao
#define v1 0.2 //fator de predacao do coelho para com a vegetacao
#define v2 0.4 //fator de predacao da vaca para com a vegetacao
#define v3 0.4 //fator de predacao do humano para com a vegetacao

//parametros coelho
#define c1 0.5 //quanto o coelho se beneficia da vegetacao
#define c2 0.35 //fator de predacao da raposa para com a coelho
#define c3 0.1 //fator de predacao do leao para com a coelho
#define c4 0.25 //fator de cooperacao do humano para com o coelho
#define c5 0.15 //fator limitante coelho

//parametros vaca
#define va1 0.2 //quanto a vaca se beneficia da vegetacao
#define va2 0.4 //fator de predacao do leao para com a vaca
#define va3 0.3 //fator de predacao do homem para com a vaca
#define va4 0.5 //espaco limitante para a vaca

//parametros raposa
#define r1 0.3 //quanto a raposa se beneficia do coelho
#define r2 0.15 //fator de predacao do leao para com a raposa
#define r3 0.05 //fator de predacao do homem para com a raposa
#define r4 0.3 //espaco limitante para a raposa

//parametros leao
#define l1 0.05 //quanto o leao se beneficia do coelho
#define l2 0.5 //quanto o leao se beneficia da vaca
#define l3 0.1 //quanto o leao se beneficia da raposa
#define l4 0.7//fator de competicao por espaco entre homem e leao
#define l5 0.7 //espaco limitante para o leao

//parametros humanos
#define h1 0.4 //quanto o homem se beneficia da vegetacao
#define h2 0.6 //quanto o homem se beneficia da vaca
#define h3 0.02 //quanto o homem se beneficia da raposa
#define h4 0.7 //espaco limitante para o humano
#define h5 0.1 //quanto o homem se beneficia do coelho
#define h6 0.25 //quanto o humano se beneficia da industria


//parametros industria
#define i1 0.35 //quanto a industria se beneficia do humano
#define i2 0.7 //fator limitante da industria

//Esta funcao é usada para informar que a vegetacao esta estavel e que especies podem ser adicionadas
int range(float vegetacao){
    float lim_i;
    lim_i = k*0.99;
    if(vegetacao > lim_i)
        return 1;
    else return 0;
}

int main(){
    float vegetacao = 2;
    float coelho = 0;
    float vaca = 0;
    float raposa = 0;
    float leao = 0;
    float humano = 0;
    float industria = 0;
    float tempo = 0;
    int flag = 0,flag_a = 0, flag_b = 0,cont = 0;
    FILE *out;
    out = fopen ("dados.txt","w");
    while(tempo<=tempo_final){
        if(cont%10 == 0) //metodo para minimizar as informacoes que sao escritas no .txt
            fprintf(out,"%.2f   %.2f  %.2f  %.2f  %.2f %.2f %.2f  %2f\n",tempo,vegetacao,coelho,vaca,leao,raposa,humano,industria);
        tempo = tempo +  passo;
        vegetacao = vegetacao + (alfa*vegetacao*(1 - v1*coelho - v2*vaca - v3*humano - (vegetacao/k)))*passo;
        if(!flag){ //adiciona as especies com populacao inicial = 2 a todas, apenas uma vez e apos a vegetacao estar estavel
            if(range(vegetacao)){
                coelho = 2;
                vaca= 2;
                raposa = 2;
                leao = 2;
                humano = 2;
                flag = 1;
            }
        }
        else { //integracao numerica das equacoes
           if(tempo < 1000){
            coelho  = coelho + (coelho*(c1*vegetacao - c2*raposa - c3*leao + c4*humano - c5))*passo;
            vaca    = vaca   + (vaca*(va1*vegetacao  - va2*leao - va3*humano - va4))*passo;
            leao    = leao   + (leao*(l1*coelho + l2*vaca + l3*raposa - l4*humano - l5))*passo;
            raposa  = raposa + (raposa*(r1*coelho - r2*leao - r3*humano - r4))*passo;
            humano  = humano + (humano*(h1*vegetacao + h2*vaca + h3*raposa - h4))*passo;
        }
            else{//Pertubacao de quando o homem comeca a comer o coelho ao inves de cria-lo
                /*//variavel flag para adicionar populacao de raposas, extinguir a populacao de vegetacao e adionar o fator industrial
                 if(tempo>2000 && flag_a==0){
                    vegetacao = 0.0 //incendio
                    industria = 1.0;
                    raposa += 10;   //raposas migrantes
                    flag_a = 1;
                }*/

                //variavel flag responsavel pela extincao da populacao humana
                /*if(tempo>2012 && flag_b==0){
                    humano = 0.0;
                    flag_b = 1;
                }*/

                industria = industria +(industria*(i1*humano - i2))*passo;
                coelho  = coelho + (coelho*(c1*vegetacao - c2*raposa - c3*leao - c4*humano - c5))*passo;
                vaca    = vaca   + (vaca*(va1*vegetacao  - va2*leao - va3*humano - va4))*passo;
                leao    = leao   + (leao*(l1*coelho + l2*vaca + l3*raposa - l4*humano - l5))*passo;
                raposa  = raposa + (raposa*(r1*coelho - r2*leao - r3*humano - r4))*passo;
                humano  = humano + (humano*(h1*vegetacao + h2*vaca + h3*raposa - h4 + h5*coelho + h6*industria))*passo;
            }
        }
        cont ++;
    }
    fclose(out);
    printf("\n\t\t\t\tconcluido\n\n");
    return (0);
}

/*
O trecho de codigo que representa a perturbacao do tipo incendio é:
                if(tempo>2000 && flag_a==0){
                    vegetacao = 0;
                    flag_a = 1;
                }
O trecho de codigo que representa a perturbacao do tipo raposas migrantes é:
                if(tempo>2000 && flag_a==0){
                    raposa += 10;
                    flag_a = 1;
                }
Em que ambos sao incorporados na main
*/
