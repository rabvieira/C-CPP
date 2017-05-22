/**********************************************************
** PROGRAMA EM CPP IMPLEMENTANDO ALGORITMO GENETICO PARA **
** PROBLEMA DE OTIMIZACAO DE BAR                         **
**                                                       **
** AUTOR: RENAN VIEIRA (UNIFESP), MODIFICADO DE          **
**        ANTONIO AUGUSTO CHAVES (FEG/UNESP)             **
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX(x,y) ((x)<(y) ? (y) : (x))

#define n 50                 //numero de objetos
#define capacidade 100       //capacidade da mochila

//Tipo Solução
struct TSol
{
  int obj[n];
  int fo;
};

//Variáveis Globais
int B[n],                   //vetor com os benefícios dos objetos
    P[n];                   //vetor com os pesos dos objetos

//Declaração de Funções
void LerArquivos();
void GA();
int CalcularFO(TSol s);
TSol GerarSolucaoInicial();
TSol TrocarBit(TSol s, int i);
void ImprimirMelhorSol(TSol s);

double randomico(double min, double max);
int irandomico(int min, int max);

/************************************************************************************
*** Método: Main()                                                                ***
*** Função: Programa principal                                                    ***
*************************************************************************************/
int main()
{
  LerArquivos();
  system("cls");
  printf("\nAlgoritmo Genetico\n");

  GA();

  fflush(stdin);
  getchar();
  return 0;
}


/************************************************************************************
*** Método: LerArquivos()                                                         ***
*** Função: Ler os arquivos de beneficios e pesos dos objetos                     ***
*************************************************************************************/
void LerArquivos()
{
  int j, valor;
  FILE *arquivo;

  char nomearqB[50] = "beneficio50.txt";
  char nomearqP[50] = "peso50.txt";

  //Ler os benefícios dos objetos
  arquivo = fopen(nomearqB,"r");
	if (!arquivo) {
     printf("O Arquivo %s nao pode ser aberto.\n", nomearqB);
     getchar();
     exit(1);
  }
  j=0;
  while (!feof(arquivo)){
    fscanf(arquivo, "%d", &valor);
    B[j] = valor;
    j++;
  }
  fclose(arquivo);

  //Ler os pesos
  arquivo = fopen(nomearqP,"r");
  if (!arquivo) {
     printf("O Arquivo %s nao pode ser aberto.\n", nomearqP);
     getchar();
     exit(1);
  }
  j=0;
  while (!feof(arquivo)){
    fscanf(arquivo, "%d", &valor);
    P[j] = valor;
    j++;
  }
  fclose(arquivo);
}

/************************************************************************************
*** Método: limpar_solucao()                                                      ***
*** Função: Inicializa todos os indivíduos de uma população como "nulos"          ***
*************************************************************************************/
void limpar_populacao(TSol Pop[], int tamPop)
{
     for (int i=0; i<tamPop; i++)
     {
         for (int j=0; j<n; j++)
            Pop[i].obj[j]=0;
         Pop[i].fo = CalcularFO(Pop[i]);
         }
     return;
}

/************************************************************************************
*** Método: CalcularFO(TSol s)                                                    ***
*** Função: Calcula o valor da função objetivo da solução s                       ***
*************************************************************************************/
int CalcularFO(TSol s)
{
  int fo,
      beneficio = 0,
      peso = 0,
      inviabilidade = 0,
      penalidade = 400;

  for (int i=0; i<n; i++)
  {
    if (s.obj[i] == 1)
    {
      beneficio += B[i];
      peso += P[i];
    }
  }

  inviabilidade = MAX(0, peso - capacidade);
  fo = beneficio - (penalidade * inviabilidade);

  return fo;
}

/************************************************************************************
*** Método: GerarSolucaoInicial()                                                 ***
*** Função: Gerar Solucao Inicial Aleatorimante                                   ***
*************************************************************************************/
TSol GerarSolucaoInicial()
{
  TSol s;
  for (int j=0; j<n; j++)
    s.obj[j] = irandomico(0,1);

  s.fo = CalcularFO(s);

  return s;
}

/************************************************************************************
*** Método: randomico(double min, double max)                                     ***
*** Função: Gera um numero aleatorio entre min e max                              ***
*************************************************************************************/
double randomico(double min, double max)
{
	return ((double)(rand()%10000)/10000.0)*(max-min)+min;
}

/************************************************************************************
*** Método: irandomico(int min, int max)                                          ***
*** Função: Gera um numero inteiro aleatorio entre min e max                      ***
*************************************************************************************/
int irandomico(int min, int max)
{
	return (int)randomico(0,max-min+1.0) + min;
}

/************************************************************************************
*** Método: TrocarBit(TSol s, int i)                                              ***
*** Função: Troca um bit da solução s                                             ***
*************************************************************************************/
TSol TrocarBit(TSol s, int i)
{
  if (s.obj[i] == 1)
    s.obj[i] = 0;
  else
    s.obj[i] = 1;

  return s;
}

/************************************************************************************
*** Método: ImprimirMelhorSol()                                                   ***
*** Função: Imprimir a melhor solucao encontrada                                  ***
*************************************************************************************/
void ImprimirMelhorSol(TSol s)
{
  printf("\n\n\nMELHOR SOLUCAO ENCONTRADA");
  printf("\n*************************");
  printf("\n\nFO: %d", s.fo);
  printf("\n\nObjetos selecionados: \n");
  printf("--------------------\n");
  for (int i=0; i<n; i++)
    if (s.obj[i])
      printf(" %d ",i);

  printf("\n\nObjetos nao selecionados: \n");
  printf("------------------------\n");
  for (int i=0; i<n; i++)
    if (!s.obj[i])
      printf(" %d ",i);

  printf("\n\n\nPressione alguma tecla para retornar ao menu...");
  fflush(stdin);
  getchar();
}

/************************************************************************************
*** Método: GA()                                                                  ***
*** Função: Aplica a metaheuristica Algoritmo Genetico no Problema da Mochila     ***
*************************************************************************************/
void GA()
{
  int tamPop = 100;
  float probCros = 0.95,
        probMut = 0.005;

  printf("\n\nDigite o tamanho da populacao: ");
  scanf("%d", &tamPop);

  printf("\n\nDigite a probabilidade de crossover: ");
  scanf("%f", &probCros);

  printf("\n\nDigite a probabilidade de mutacao: ");
  scanf("%f", &probMut);

  TSol Pop[tamPop];
  TSol PopInter[tamPop];
  TSol PopFilhos[tamPop];

  TSol melhorInd;
  TSol melhorFilho;
  melhorInd.fo = -9999999;

  limpar_populacao(Pop, tamPop);
  limpar_populacao(PopInter, tamPop);
  limpar_populacao(PopFilhos, tamPop);

  //Gerar a população inicial
  for (int i=0; i<tamPop; i++)
  {
    TSol ind = GerarSolucaoInicial();
    Pop[tamPop-i-1]=ind;
  }


  int numGeracoes = 0;
  while (numGeracoes < 1000)
  {
    numGeracoes++;

    //Selecionar os pais por meio do método de seleção por TORNEIO
    for (int j=0; j<tamPop; j++)
    {
      int p1, p2, p3, melhor;

      p1 = irandomico(0,tamPop-1);
      p2 = irandomico(0,tamPop-1);
      p3 = irandomico(0,tamPop-1);

      if ((Pop[p1].fo > Pop[p2].fo) && (Pop[p1].fo > Pop[p3].fo))
				melhor = p1;
      else
      if (Pop[p2].fo > Pop[p3].fo)
        melhor = p2;
			else
        melhor = p3;

			//inserir o melhor indivíduo na população intermediaria
			PopInter[tamPop-j-1]=Pop[melhor];
		}


    //***********************************************************************
    //  realizar crossover de 2 pontos entre os pares de pais selecionados
    //***********************************************************************
    int pcorte1 = 0;
    int pcorte2 = 0;

    //realizar o crossover até o número de filhor ser igual a tamPop
    for (int i=0; i<tamPop; i=i+2)
    {
      PopFilhos[tamPop-i-2] = PopInter[i];
      PopFilhos[tamPop-i-1] = PopInter[i+1];

      //verificar a probalidade de crossover
      if (randomico(0,1) <= probCros)
      {
        pcorte1 = irandomico(1,n-1);
        pcorte2 = irandomico(1,n-1);

        if (pcorte1 > pcorte2)
				{
          int temp = pcorte1;
          pcorte1 = pcorte2;
          pcorte2 = temp;
        }

        for (int j=pcorte1; j<pcorte2; j++)
        {
          PopFilhos[i].obj[j] = PopInter[i+1].obj[j];
          PopFilhos[i+1].obj[j] = PopInter[i].obj[j];
        }
      }
    }

    //***********************************************************************
	//       realizar mutação bit a bit nos filhos gerados - trocar bit
	//***********************************************************************
	for (int i=0; i<tamPop; i++)
	{
   	 //verificar a probabilidade de realizar mutacao em cada bit do filho
      for (int j=0; j<n; j++)
      {
        if (randomico(0,1) <= probMut)
          PopFilhos[i] = TrocarBit(PopFilhos[i], j);
      }
    }


    //Avaliar a populaçao de filhos
    melhorFilho.fo = -99999999;
    int mediaFilhos = 0;
    for (int i=0; i<tamPop; i++)
    {
      PopFilhos[i].fo = CalcularFO(PopFilhos[i]);

			mediaFilhos += PopFilhos[i].fo;

      //encontrar o melhor filho desta geracao
      if (PopFilhos[i].fo > melhorFilho.fo)
				melhorFilho = PopFilhos[i];

      //encontrar o melhor filho do GA
      if (PopFilhos[i].fo > melhorInd.fo)
        melhorInd = PopFilhos[i];
    }

	printf("Geracao: %d  media = %d   melhor individuo = %d  \n", numGeracoes, (mediaFilhos/tamPop), melhorInd.fo);

    //Limpar Pop e PopInter
    limpar_populacao(Pop, tamPop);
    limpar_populacao(PopInter, tamPop);


    //substituir a população pelos filhos gerados
    for (int i=0; i<tamPop; i++)
       Pop[i] = PopFilhos[i];

    //Elitismo - atribuir o melhor individuo na Pop
    int pos1 = irandomico(0,tamPop-1);
		Pop[pos1] = melhorInd;

    // PopFilhos.clear();
    limpar_populacao(PopFilhos, tamPop);

  }

  ImprimirMelhorSol(melhorInd);
}