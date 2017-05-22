/* Autor:
 * Disciplina:  Introducao a Computacao-I
 * Data:        09.05.2017
 * Descricao:   Este programa recebe dois numeros digitados do teclado,
 *              encontra o numero primo mais proximo destes e efetua
 *              a multiplicacao imprimindo o resultado.
*/

//bibliotecas
#include <stdio.h>
#include <stdlib.h>

//determina se o parametro number e´ um numero primo
//retorna number se primo ou zero caso contrario
int is_prime(int number)
{
    int i;
    for(i=2 ; i<number ; i++)
    {
        if(number%i == 0)
            break;
    }
    if(number==i)
        return number;
    else
        return 0;
}

//retorna o numero primo mais proximo do parametro number
int max_prime(int number)
{
    int i, aux=0, max=0;
    for(i=2 ; i<=number ; i++)
    {
        aux = is_prime(i);
        if(aux > 0)
            max = aux;
    }
    return max;
}

//funcao principal
int main(void)
{
    int N_JU = 10; //numero do Juilherme
    int M_JO = 15; //numero do Jogerio

    //recebe do usuario o valor dos numeros
    printf("type N: ");
    scanf("%d",&N_JU);
    printf("type M: ");
    scanf("%d",&M_JO);

    //efetua a multiplicacao dos primos mais proximos e imprime
    printf("answer: %d", max_prime(N_JU) * max_prime(M_JO));
    return 0;
}
