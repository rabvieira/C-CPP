
//Desenvolvido por Renan Vieira e Danilo Dias Carvalho - BCT
#include <stdio.h>
#include <math.h>
#define tempo_inicial 0
#define passo 0.01
#define g 9.8
#define pi 3.141592654

int main(){
    float massa, v0, theta, radiano, altura_inicial;
    float velocidade_x, velocidade_y, velocidade, tempo;
    float posicao_x,posicao_y, energia_cinetica, k;
    FILE *out;
    printf("\t\t<<Movimento de Projetil com Resistencia do Ar>>\n\n");
    printf("massa:\t");
    scanf("%f",&massa);
    printf("constante de amortecimento:\t");
    scanf("%f",&k);
    printf("altura inicial:\t");
    scanf("%f",&altura_inicial);
    printf("velocidade inicial:\t");
    scanf("%f",&v0);
    printf("angulo inicial:\t");
    scanf("%f",&theta);
    radiano = (pi*theta)/180;
    velocidade = v0;
    velocidade_x = v0*cos(radiano);
    velocidade_y = v0*sin(radiano);
    tempo = tempo_inicial;
    posicao_x = 0;
    posicao_y = altura_inicial;
    energia_cinetica = ((massa*pow(v0,2))/2);
    out = fopen ("dados.txt","w");
    while(posicao_y>=0){
        fprintf(out,"%.2f   %.4f   %.4f   %.4f   %.4f\n",tempo,posicao_x,posicao_y,velocidade,energia_cinetica);
        tempo = tempo +  passo;
        posicao_x = posicao_x + velocidade_x*passo;
        posicao_y = posicao_y + velocidade_y*passo;
        velocidade_x = velocidade_x + (passo * (-(k*velocidade)/massa));
        velocidade_y = velocidade_y + (passo * ((-(k*velocidade)/massa) - g));
        velocidade = sqrt(pow(velocidade_x,2) + pow(velocidade_y,2));
        energia_cinetica = ((massa*pow(velocidade,2))/2);
    }
    fclose(out);
    printf("\n\nfoi criado 'dados.txt' como saida\n\n\n");
    return (0);
}
