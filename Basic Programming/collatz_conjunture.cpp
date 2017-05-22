/*
-----------------------------------------------------------------------------
 Autores   :
 Data	   : 12.05.2017
 Descriçao : programa que descobre qual o numero pensado pelo usuario:
             jogador e´ convidado a pensar em um numero inteiro positivo
             - se o numero for par: dividir este numero por dois
             - se o numero for impar: subtrair um e dividir o numero por dois
             repetir este processo ate´ que atinja o numero um.
             o numero pensado e´ entao apresentado junto com sua base binaria.
-----------------------------------------------------------------------------
*/
/*bibliotecas utilizadas*/
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//converte um inteiro passado como parametro para binario em formato de string
string decToBinary(int numb)
{
    string aux;
    while(numb!=0)
    {
        aux = (numb%2==0 ? "0" : "1" ) + aux;
        numb/=2;
    }
    return aux;
}

//imprime uma mensagem no display alem de analisar se a entrada digitada
//pelo usuario esta entre os parametros permitidos por option
//retorna um char valido digitado pelo usuario
char readChar(string message, string option)
{
	stringstream readChar;
	string auxReadChar;
	char str;

	cout << message;
	while(true)
    {
		readChar.clear();
		auxReadChar.clear();
		fflush(stdin);
		getline(cin, auxReadChar);
		readChar.str(auxReadChar);
		readChar >> str;
		for (size_t i=0; i<option.length(); i++)
			if(option[i] == str || toupper(option[i]) == str)
				return tolower(str);
		cout << "\n<<Respostas Aceitaveis: [ ";
		for (size_t i=0; i<option.length(); i++)
        {
			cout << option[i];
			if(i<option.length()-1)
				cout << " ou ";
		}
		cout << " ]>>\n" << "\nIntroduza novamente: ";
	}
}

//coleta as respostas do usuario e reconstroi o numero pensado
int main()
{
    bool onGame = true; //controla o while loop
    char charInput, playAgain; //recebe a resposta digitada pelo usuario
    vector<int> order;  //armazena 0 para par e 1 para impar no vetor da ordem digitada
    float number = 1.;  //numero que sera reconstruido baseado nas respostas
    while(onGame)
    {
        charInput = readChar("\nDigite se o numero e' [P]ar ou [I]mpar: ", "pi");
		switch(charInput)
		{
            case 'p':
                cout << "\ndivida por 2" << endl;
                order.push_back(0);
            break;
            case 'i':
                cout << "\nsubtraia 1 e divida por 2" << endl;
                order.push_back(1);
            break;
		}
		playAgain = readChar("\nEste numero e' igual a 1? [S]im ou [N]ao: ","sn");
		if(playAgain == 's')
			onGame = false;
    }
    //percorre o vetor de respostas de forma inversa
    for(int i=order.size()-1 ; i>=0 ; i--)
    {
        if(order.at(i) == 0)
            number = number*2;
        else
            number = number*2 + 1;
    }
    //imprime as respostas
	cout << "\n\nO numero pensado foi: " << number << endl;
    cout << "\nnumero em base binaria: " << decToBinary((int)number) << endl;
	return 0;
}
