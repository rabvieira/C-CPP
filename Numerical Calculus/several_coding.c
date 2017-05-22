/*
Autores:
    Eduardo Chaves do Nascimento
    Renan Arantes Bernardes Vieira
    BCT - Calculo Numerico
*/

#include <iostream>     /* cin, cout */
#include <vector>       /* class vector */
#include <math.h>       /* pow */
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */
#include <iomanip>      /* setprecision */
#include <stdio.h>      /* printf */
const unsigned int PRECISION = 8; //algarismos significativos
const unsigned int rand_min = 1;  //piso para geracao dos numeros aleatorios
const unsigned int rand_max = 10; //teto para geracao dos numeros aleatorios
const unsigned int erro = 0.0001; //limiar de zero
using namespace std;    /* standard */

//_______________________________________________________//
//_________________Funcoes_Auxiliares____________________//
//_______________________________________________________//
void print_matrix(const vector<vector<double> > &A)
{
    for(int i=0 ; i<A.size() ; i++)
    {
        for(int j=0 ; j<A.size() ; j++)
        {
            cout << setprecision(PRECISION) << A[i][j] << "\t";
            //printf("%.3f\t",A[i][j]);
        }
        cout << endl;
    }
    cout << endl;
}
//_______________________________________________________//
void print_vector(const vector<double> &v)
{
    for(int i=0 ; i<v.size() ; i++)
        cout << setprecision(PRECISION) << v[i] << "\t";
    cout << "\n" << endl;
}
//_______________________________________________________//
void generate_TI(vector<vector<double> > &L, bool one)
{
    for(int i=0 ; i<L.size() ; i++)
    {
        for(int j=0 ; j<L.size() ; j++)
        {
            if(i<j)
                L[i].push_back(0);
            else
            {
                if(one)
                    L[i].push_back(1);
                else
                    L[i].push_back(rand_min + rand()%(rand_max-rand_min));
            }
        }
    }
}
//_______________________________________________________//
void generate_resposta(vector<double> &b)
{
     for(int i=0 ; i<b.size() ; i++)
     {
        b[i] = rand_min + rand()%(rand_max-rand_min);
     }
}
//_______________________________________________________//
void generate_TS(vector<vector<double> > &U, bool one)
{
    for(int i=0 ; i<U.size() ; i++)
    {
        for(int j=0 ; j<U.size() ; j++)
        {
            if(i>j)
                U[i].push_back(0);
            else
            {
                if(one)
                    U[i].push_back(1);
                else
                    U[i].push_back(rand_min + rand()%(rand_max-rand_min));
            }
        }
    }
}
//_______________________________________________________//
void generate_matriz(vector<vector<double> > &A)
{
    for(int i=0 ; i<A.size() ; i++)
    {
        for(int j=0 ; j<A.size() ; j++)
        {
            A[i].push_back(rand_min + rand()%(rand_max-rand_min));
        }
    }
}
//_______________________________________________________//
void generate_matriz_gabarito(vector<vector<double> > &A)
{
    if(A.size()==3)
    {
        A[0].push_back(5);
        A[0].push_back(2);
        A[0].push_back(1);
        A[1].push_back(3);
        A[1].push_back(1);
        A[1].push_back(4);
        A[2].push_back(1);
        A[2].push_back(1);
        A[2].push_back(3);
    }
    else
        cout << "n deve ser igual a tres..." << endl;
}
//_______________________________________________________//
void generate_vector_gabarito(vector<double> &v)
{
    if(v.size()==3)
    {
        v[0] = 5.;
        v[1] = 10.;
        v[2] = 6.;
    }
    else
        cout << "n deve ser igual a tres..." << endl;
}
//_______________________________________________________//
//_______________________________________________________//
//_______________________________________________________//

//__________________Exercicio_1__________________________//
void matriz_ij(vector<vector<double> > &A)
{
    for(int i=0 ; i<A.size() ; i++)
    {
        for(int j=0 ; j<A.size() ; j++)
        {
            A[i].push_back(pow(i+1,j));
        }
    }
}

void exerc1(int n)
{
    vector<vector<double> > A;
    A.resize(n);
    matriz_ij(A);
    cout << "Matriz n^(n-1):\n\n";
    print_matrix(A);
}
//_______________________________________________________//

//__________________Exercicio_2__________________________//
void vetor_n1(vector<double> &v)
{
    int n = v.size();
    v[0] = n-1;
    for(int i=1 ; i<n ; i++)
    {
        v[i] = (pow(i+1,n)-1)/(i);
    }
}

void exerc2(int n)
{
    vector<double> v;
    v.resize(n);
    vetor_n1(v);
    cout << "Vetor (n^(n)-1)/(n-1):\n\n";
    print_vector(v);
}
//_______________________________________________________//

//__________________Exercicio_3__________________________//
void solucao_TI(vector<vector<double> > &A, vector<double> &x, vector<double> &b)
{
    double sum = 0;
    for(int i=0 ; i<A.size() ; i++)
    {
        for(int j=0 ; j<i ; j++)
        {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
        sum = 0;
    }
}

void exerc3(int n)
{
    vector<vector<double> > A;
    A.resize(n);
    vector<double> b,x;
    b.resize(n);
    x.resize(n);
    generate_TI(A,false);
    cout << "Matriz Triangular Inferior Generica (L):\n\n";
    print_matrix(A);
    generate_resposta(b);
    cout << "Vetor Resposta Generica (b):\n\n";
    print_vector(b);
    solucao_TI(A,x,b);
    cout << "Vetor Solucao (x):\n\n";
    print_vector(x);
}
//_______________________________________________________//

//__________________Exercicio_4__________________________//
void solucao_TS(vector<vector<double> > &A, vector<double> &x, vector<double> &b)
{
    double sum = 0;
    for(int i=A.size()-1 ; i>=0 ; i--)
    {
        for(int j=A.size()-1 ; j>i ; j--)
        {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
        sum = 0;
    }
}

void exerc4(int n)
{
    vector<vector<double> > A;
    A.resize(n);
    vector<double> b,x;
    b.resize(n);
    x.resize(n);
    generate_TS(A,false);
    cout << "Matriz Triangular Superior Generica (U):\n\n";
    print_matrix(A);
    generate_resposta(b);
    cout << "Vetor Resposta Generica (b):\n\n";
    print_vector(b);
    solucao_TS(A,x,b);
    cout << "Vetor Solucao (x):\n\n";
    print_vector(x);
}
//_______________________________________________________//

//__________________Exercicio_5__________________________//
bool check_det(const vector<vector<double> > &A)
{
    for (int i=0 ; i<A.size() ; i++){
        if (fabs(A[i][i])<erro)
            return false;
    }
    return true;
}

double sum_u(int i, int j, const vector<vector<double> > &L, const vector<vector<double> > &U)
{
    double sum=0;
    for(int k=0 ; k<=i-1 ; k++)
    {
        sum += L[i][k]*U[k][j];
    }
    return sum;
}

double sum_l(int i, int j, const vector<vector<double> > &L, const vector<vector<double> > &U)
{
    double sum=0;
    for(int k=0 ; k<=j-1 ; k++)
    {
        sum += L[i][k]*U[k][j];
    }
    return sum;
}

bool decomposicao(vector<vector<double> > &A, vector<vector<double> > &L, vector<vector<double> > &U)
{
    generate_TI(L,true);
    generate_TS(U,true);
    for(int i=0 ; i<A.size() ; i++)
    {
        for(int j=0 ; j<A.size() ; j++)
        {
            if(i<=j)
                U[i][j] = A[i][j] - sum_u(i,j,L,U);
            else
                L[i][j] = (1./(U[j][j])) * (A[i][j] - sum_l(i,j,L,U));
        }
    }
    return check_det(U);
}

void exerc5(int n)
{
    vector<vector<double> > A;
    A.resize(n);
    vector<vector<double> > L,U;
    L.resize(n);
    U.resize(n);
    generate_matriz(A);
    //generate_matriz_gabarito(A); /* desde que n=3 */
    cout << "Matriz A:\n\n";
    print_matrix(A);
    if(decomposicao(A,L,U))
    {
        cout << "Matriz L:\n\n";
        print_matrix(L);
        cout << "Matriz U:\n\n";
        print_matrix(U);
    }
    else
        cout << "Nao e' possivel decompor a Matriz A\n\n";
}
//_______________________________________________________//

//__________________Exercicio_6__________________________//
void exerc6(int n)
{
    vector<vector<double> > A, L, U;
    A.resize(n);
    L.resize(n);
    U.resize(n);
    vector<double> v, x, b;
    v.resize(n);
    x.resize(n);
    b.resize(n);
    generate_matriz(A);
    //generate_matriz_gabarito(A); /* desde que n=3 */
    cout << "Matriz A:\n\n";
    print_matrix(A);
    generate_resposta(b);
    //generate_vector_gabarito(b); /* desde que n=3 */
    cout << "Resposta (b):\n\n";
    print_vector(b);
    if(decomposicao(A,L,U))
    {
/*
        cout << "Matriz L:\n\n";
        print_matrix(L);
        cout << "Matriz U:\n\n";
        print_matrix(U);
*/
        solucao_TI(L,v,b);
        solucao_TS(U,x,v);
        cout << "Solucao:\n\n";
        print_vector(x);
    }
    else
        cout << "Nao e' possivel decompor a Matriz A\n\n";
}
//_______________________________________________________//

//__________________Exercicio_7__________________________//
void exerc7(int n)
{
    vector<vector<double> > A, L, U;
    A.resize(n);
    L.resize(n);
    U.resize(n);
    vector<double> v, x, b;
    v.resize(n);
    x.resize(n);
    b.resize(n);
    matriz_ij(A);
    cout << "Matriz n^(n-1):\n\n";
    print_matrix(A);
//---------------------------------------
    vetor_n1(b);
    cout << "Vetor (n^(n)-1)/(n-1):\n\n";
    print_vector(b);
//---------------------------------------
    if(decomposicao(A,L,U))
    {
/*
        cout << "Matriz L:\n\n";
        print_matrix(L);
        cout << "Matriz U:\n\n";
        print_matrix(U);
*/
    solucao_TI(L,v,b);
    solucao_TS(U,x,v);
    cout << "Solucao:\n\n";
    print_vector(x);
    }
    else
        cout << "Nao e' possivel decompor a Matriz A\n\n";
}
//_______________________________________________________//

//_______________________________________________________//
//_______________________________________________________//
//_______________________________________________________//

//Descomentar para verificar o funcionanado dos exercicios separadamente.
int main()
{
    srand(time(NULL));
    int n;
    cout << "digite n:\t";
    cin >> n;
    cout << endl;
    exerc1(n);
//    exerc2(n);
//    exerc3(n);
//    exerc4(n);
//    exerc5(n);
//    exerc6(n);
//    exerc7(n);
    return 0;
}