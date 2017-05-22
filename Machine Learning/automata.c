/*
-----------------------------------------------------------------------------
 Nome      : automato.c
 Autores   : Alexandre A., Eduard Eric e Renan Arantes
 Descriçao : Este codigo simula o automato celular "Jogo da Vida"
             da forma tradicional (vizinhanca de Moore - 8 vizinhos),
             em um tabuleiro retangular MxN com as bordas conectadas entre si.
-----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#define max 80 //valor maximo de linhas e colunas do tabuleiro

//Funcao que zera todos os elementos da matriz passada como parametro
void inicializa_matriz(int matriz[max][max], int l, int c){
    int i, j;
    for (i=0 ; i<l ; i++)
        for (j=0 ; j<c ; j++)
            matriz[i][j] = 0;
}

//Funcao que imprime todos os elementos da matriz passada como parametro
void imprime_matriz(int matriz[max][max],int l, int c, int geracao){
    int i, j;
    for (i=0 ; i<l ; i++){
        printf("\n");
        for (j=0 ; j<c ; j++){
            if(matriz[i][j])
                printf("#"); //vivo
            else
                printf(" "); //morto
        }
    }
    printf("\n\ngeracao: \t%d\n",geracao);
}
//Regras do jogo da vida
int regras(int vizinho,int estado){
//sobrevive /nasce
    if((estado==1 && (vizinho==2 || vizinho==3)) || (estado==0 && vizinho==3))
        return 1;
//morte por solidao ou superpopulacao
    else if(estado==1 && (vizinho==0 || vizinho==1 || vizinho==4))
        return 0;
    else
        return 0;
}

//Caso o jogo acabe (tudo morto) esta funcao termina o jogo
int verifica_fim(int matriz_A[max][max], int matriz_B[max][max],int l, int c){
    int i, j;
    for(i=0 ; i<l ; i++)
        for(j=0 ; j<c ; j++){
            if(matriz_A[i][j]!=matriz_B[i][j] && matriz_B[i][j]==1)
                return 0; //jogo continua
        }
    return 1;             //jogo termina
}

//Funcao que apaga a tela e aguarda um tempo determinado
//Caso mode esteja ativado e' necessario digitar uma tecla para mudar de tela
void apaga_tela(int mode, int tempo){
    if(mode)
        getch();
    _sleep(tempo);
    system("cls");
}

//Funcao que trata o jogo
void jogo_da_vida(int matriz_A[max][max], int l, int c, int interacoes, int mode, int t){
    int i, j, k, x, y, vizinho=0, var=0, flag=0;
    int matriz_B[max][max]; //tabuleiro auxiliar
    inicializa_matriz(matriz_B, l, c); //inicializa a matriz auxiliar
    for(k=0 ; k<interacoes ; k++){ //executa k interacoes
        for(i=0 ; i<l ; i++){      //varre as linhas da matriz
            for(j=0 ; j<c ; j++){  //varre as colunas da matriz
//Tratamento de cantos (mundo  ciclico)
                if(i==0 && j==0 || i==0 && j==c-1 || i==l-1 && j==0 || i==l-1 && j==c-1){
                    if(i==0 && j==0){ //superior esquerdo
                        if(matriz_A[l-1][0]==1) //Cim
                            vizinho++;
                        if(matriz_A[0][c-1]==1) //Esq
                            vizinho++;
                        if(matriz_A[0][1]==1)   //Dir
                            vizinho++;
                        if(matriz_A[1][0]==1)   //Bai
                            vizinho++;
                    //segundos vizinhos
                        if(matriz_A[l-1][c-1]==1) //Nw
                            vizinho++;
                        if(matriz_A[l-1][1]==1)   //Ne
                            vizinho++;
                        if(matriz_A[1][c-1]==1)   //Sw
                            vizinho++;
                        if(matriz_A[1][1]==1)     //Se
                            vizinho++;
                        matriz_B[i][j]=regras(vizinho,matriz_A[i][j]);
                    }
                    else if(i==0 && j==c-1){ //superior direito
                        if(matriz_A[l-1][c-1]==1) //Cim
                            vizinho++;
                        if(matriz_A[0][0]==1)     //Dir
                            vizinho++;
                        if(matriz_A[0][c-2]==1)   //Esq
                            vizinho++;
                        if(matriz_A[1][c-1]==1)   //Bai
                            vizinho++;
                    //segundos vizinhos
                        if(matriz_A[l-1][c-2]==1) //Nw
                            vizinho++;
                        if(matriz_A[l-1][0]==1)   //Ne
                            vizinho++;
                        if(matriz_A[1][c-2]==1)   //Sw
                            vizinho++;
                        if(matriz_A[1][0]==1)     //Se
                            vizinho++;
                        matriz_B[i][j]=regras(vizinho,matriz_A[i][j]);
                    }
                    else if(i==l-1 && j==0){ //inferior esquerdo
                        if(matriz_A[0][0]==1)     //Bai
                            vizinho++;
                        if(matriz_A[l-1][c-1]==1) //Esq
                            vizinho++;
                        if(matriz_A[l-1][1]==1)   //Dir
                            vizinho++;
                        if(matriz_A[l-2][0]==1)   //Cim
                            vizinho++;
                    //segundos vizinhos
                        if(matriz_A[l-2][c-1]==1) //Nw
                            vizinho++;
                        if(matriz_A[l-2][1]==1)   //Ne
                            vizinho++;
                        if(matriz_A[0][c-1]==1)   //Sw
                            vizinho++;
                        if(matriz_A[0][1]==1)     //Se
                            vizinho++;
                        matriz_B[i][j]=regras(vizinho,matriz_A[i][j]);
                    }
                    else{//if(i==l-1 && j==c-1){ //inferior direito
                        if(matriz_A[0][c-1]==1)   //Bai
                            vizinho++;
                        if(matriz_A[l-1][0]==1)   //Dir
                            vizinho++;
                        if(matriz_A[l-1][c-2]==1) //Esq
                            vizinho++;
                        if(matriz_A[l-2][c-1]==1) //Cim
                            vizinho++;
                    //segundos vizinhos
                        if(matriz_A[l-2][c-2]==1) //Nw
                            vizinho++;
                        if(matriz_A[l-2][0]==1)   //Ne
                            vizinho++;
                        if(matriz_A[0][c-2]==1)   //Sw
                            vizinho++;
                        if(matriz_A[0][0]==1)     //Se
                            vizinho++;
                        matriz_B[i][j]=regras(vizinho,matriz_A[i][j]);
                    }
                }
//Bordas do tabuleiro
                else{
                    if(i==0){ //superior
                        var=1;
                        if(matriz_A[l-1][j]==1)
                            vizinho++;
                        if(matriz_A[l-1][j-1]==1) //Nw
                            vizinho++;
                        if(matriz_A[l-1][j+1]==1) //Ne
                            vizinho++;
                    }
                    if(j==c-1){ //direito
                        var=2;
                        if(matriz_A[i][0]==1)
                            vizinho++;
                        if(matriz_A[i-1][0]==1) //Ne
                            vizinho++;
                        if(matriz_A[l+1][0]==1) //Se
                            vizinho++;
                    }
                    if(i==l-1){ //inferior
                        var=3;
                        if(matriz_A[0][j]==1)
                            vizinho++;
                        if(matriz_A[0][j-1]==1) //Sw
                            vizinho++;
                        if(matriz_A[0][j+1]==1) //Se
                            vizinho++;
                    }
                    if(j==0){ //esquerdo
                        var=4;
                        if(matriz_A[i][c-1]==1)
                            vizinho++;
                        if(matriz_A[i-1][c-1]==1) //Nw
                            vizinho++;
                        if(matriz_A[i+1][c-1]==1) //Sw
                            vizinho++;
                    }
                    switch(var){
                        case 1: //superior
                            if(matriz_A[i][j+1]==1) //Dir
                                vizinho++;
                            if(matriz_A[i][j-1]==1) //Esq
                                vizinho++;
                            if(matriz_A[i+1][j]==1) //Bai
                                vizinho++;
                            if(matriz_A[i+1][j-1]==1) //Sw
                                vizinho++;
                            if(matriz_A[i+1][j+1]==1) //Se
                                vizinho++;
                            break;
                        case 2: //direito
                            if(matriz_A[i][j-1]==1) //Esq
                                vizinho++;
                            if(matriz_A[i-1][j]==1) //Cim
                                vizinho++;
                            if(matriz_A[i+1][j]==1) //Bai
                                vizinho++;
                            if(matriz_A[i-1][j-1]==1) //Nw
                                vizinho++;
                            if(matriz_A[i+1][j-1]==1) //Sw
                                vizinho++;
                            break;
                        case 3: //inferior
                            if(matriz_A[i-1][j]==1) //Cim
                                vizinho++;
                            if(matriz_A[i][j-1]==1) //Esq
                                vizinho++;
                            if(matriz_A[i][j+1]==1) //Dir
                                vizinho++;
                            if(matriz_A[i-1][j-1]==1) //Nw
                                vizinho++;
                            if(matriz_A[i-1][j+1]==1) //Ne
                                vizinho++;
                            break;
                        case 4: //esquerdo
                            if(matriz_A[i][j+1]==1) //Dir
                                vizinho++;
                            if(matriz_A[i+1][j]==1) //Bai
                                vizinho++;
                            if(matriz_A[i-1][j]==1) //Cim
                                vizinho++;
                            if(matriz_A[i-1][j+1]==1) //Ne
                                vizinho++;
                            if(matriz_A[i+1][j+1]==1) //Se
                                vizinho++;
                            break;
                        default: //Elemento Central
                            if(matriz_A[i-1][j]==1) //Cim
                                vizinho++;
                            if(matriz_A[i][j+1]==1) //Dir
                                vizinho++;
                            if(matriz_A[i+1][j]==1) //Bai
                                vizinho++;
                            if(matriz_A[i][j-1]==1) //Esq
                                vizinho++;
                            if(matriz_A[i-1][j-1]==1) //Nw
                                vizinho++;
                            if(matriz_A[i-1][j+1]==1) //Ne
                                vizinho++;
                            if(matriz_A[i+1][j-1]==1) //Sw
                                vizinho++;
                            if(matriz_A[i+1][j+1]==1) //Se
                                vizinho++;
                            break;
                    }
                }
            matriz_B[i][j]=regras(vizinho,matriz_A[i][j]);
            vizinho=0;
            var=0;
            } //fim for j
        }//fim for i
        vizinho=0;
        var=0;
        imprime_matriz(matriz_B,l,c,k+1);
        if(k!=interacoes-1 && flag==0)
            apaga_tela(mode,t);
        else{
            getch(); //fim das interacoes
            apaga_tela(mode,t);
        }
        if(verifica_fim(matriz_A,matriz_B,l,c)){
            flag=1;
            imprime_matriz(matriz_B,l,c,k+1);
            getch();
            apaga_tela(mode,t);
            break;
        }
        else{
            for(x=0 ; x<l ; x++) //copia matriz_B em matriz_A
                for(y=0 ; y<c ; y++)
                    matriz_A[x][y]=matriz_B[x][y];
        }
    }//fim for k
}

int main(void){
    int matriz_A[max][max];
    int i, j, k;
    int interacoes = 50;
    int linhas = 40;
    int colunas = 75;
    int mode = 0; //se 1 ativa o getch
    int tempo = 150; // tempo em milisegundos
    srand(time(0));
    inicializa_matriz(matriz_A, linhas, colunas); //zera a matriz
//              INICIO DO JOGO
///*
//Cria elementos vivos no tabuleiro de forma randomica
    for(k=0 ; k<(linhas + colunas)*4 ; k++){
        i = rand()%linhas-20;
        j = rand()%colunas-30;
        matriz_A[i][j] = 1;
    }
//*/
/*
//Inserindo Glider
    matriz_A[30][30]=1;
    matriz_A[30][31]=1;
    matriz_A[30][32]=1;
    matriz_A[29][32]=1;
    matriz_A[28][31]=1;
*/
 /*
//Inserindo Linha
    for(k=30 ; k<40 ; k++)
        matriz_A[20][k]=1;

*/
    printf("maximize a tela e tecle para iniciar...");
    getch();
    imprime_matriz(matriz_A, linhas, colunas,0); //tabuleiro inicial
    apaga_tela(mode,tempo);
    jogo_da_vida(matriz_A,linhas,colunas,interacoes,mode,tempo);
    printf("tecle para finalizar...");
    getch();
    printf("\n\n\n");
    return 0;
}