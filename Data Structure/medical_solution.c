//libraries
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"
#include "heap.h"

//global variables
int M_doctors = 0;    //number of doctors available
Fila drNames;         //doctor's name
int T_time = 0;       //time since the last patient group
int time_[2];         //array of time
int N_patient = 0;    //number of patients in the group
int patient[2];       //array of patients
Fila patientsNames;   //patient names
Fila patientsAnswers; //patient answers
int N_questions=12;   //number of questions
int line_max = 99;    //maximum number of characters onto a line
int N_group = 0;      //number of groups
int const size_array_max = 20;

int get_min(int counter, int frequencies[])
{
    int i, pos=0;
    int min=1000;//frequencies[0];
    for (i=0 ; i<counter ; i++)
    {
        if(frequencies[i]<min)
        {
             min = frequencies[i];
             pos = i;
        }
    }
    return pos;
}

int attending_time(int group_time, int rand_t)
{
    if(group_time > rand_t)
        return group_time;
    else
        return rand_t;
}

int remaning(int counter, int frequencies[])
{
    int i;
    for(i=0 ; i<counter ; i++)
    {
        if(frequencies[i]<1000)
            return 1;
    }
    return 0;
}

//main algorithm
void algorithm()
{
    int i,j,k,x,y,counter=0, awns=0;
    int frequency, pos,min=0,flag=1;
    int frequencies[5];
    int rand_times[5] = {18,54,7,19,22};
    int expected_times[5] = {0,10,20,60,180};
    int expected_times_real[5];
    int delayed_time[M_doctors];
    int available[M_doctors];
    float perc=0.;
    printf("\n\nAlgorithm:\n\n");
    for(i=0 ; i<M_doctors ; i++)
    {
        available[i]=0;
        delayed_time[i] = 0;
    }
    for(i=0 ; i<5 ; i++)
    {
        //rand_times[i] = 0;
        expected_times_real[i]=0;
        frequencies[i] = 0;
    }

    for(i=0 ; i<N_group ; i++)
    {
        for(j=0 ; j<patient[i] ; j++)
        {
            Node *aux = patientsAnswers.begin_queue;
            for(x=0 ; x<N_questions*counter ; x++)
                aux = aux->next;
            for(k=0 ; k<N_questions ; k++)
            {
                //printf("%s ",aux->name);
                if(!strcmp(aux->name, "S"))
                {
                    if(awns<3)
                    {
                        //printf(" -> red0");
                        frequency = 0;
                        break;
                    }
                    if(awns<6)
                    {
                        //printf(" -> orange1");
                        frequency = 1;
                        break;
                    }
                    if(awns<9)
                    {
                        //printf(" -> yellow2");
                        frequency = 2;
                        break;
                    }
                    if(awns<12)
                    {
                        //printf(" -> green3");
                        frequency = 3;
                        break;
                    }
                }
                if(awns==N_questions-1)
                {
                    //printf(" -> blue4");
                    frequency = 4;
                }
                aux = aux->next;
                awns++;
            }
            //printf("\n");
            expected_times_real[counter] = expected_times[frequency];
            frequencies[counter] = frequency;
            //rand_times[counter] = rand()%50;
            counter ++;
            awns=0;
        }

        //decides higher priority
        int cycle_c=0;
        while(flag)
        {
            for(y=0; y<M_doctors ; y++)
            {
                min=get_min(counter,frequencies);
            }
            Node *aux = patientsNames.begin_queue;
            char pName[50];
            for(x=0 ; x<min ; x++)
                aux = aux->next;
            strcpy(pName, aux->name);
            //frequencies[min] = 1000;
            cycle_c++;
            if(cycle_c>=patient[i])
                flag=0;
            //frequencies[min] = 1000;
            int it;
            for(it=0 ; it<M_doctors ; it++)
            {
                if(available[it]==0)
                {
                    Node *aux = drNames.begin_queue;
                    for(x=0 ; x<it ; x++)
                        aux = aux->next;
                    available[it]=1;
                    frequencies[min] = 1000;
                    printf("%s ",aux->name);
                    printf("%s ",pName);
                    printf("%d ",expected_times_real[min]);
                    printf("%d\n",delayed_time[it]);
                    if((expected_times_real[min] - delayed_time[it]) < 0)
                        perc++;
                    if(i+1 < N_group)
                        delayed_time[it] = attending_time(time_[i],rand_times[min]);
                    else
                        delayed_time[it] = rand_times[min];
                    break;
                }
            }
        }
        flag=1;
        int it;
        if(i+1 < N_group)
        {

            for(it=0 ; it<M_doctors ; it++)
            {
                if(time_[i+1]>delayed_time[it])// || i+1>=N_group)
                {
                    available[it]=0;
                    //printf("del time: %d\n",delayed_time[it]);
                    delayed_time[it] = 0;
                }
                else
                {
                    delayed_time[it] -= time_[i+1];
                }
            }
        }
    }
    //pos-processing
    flag=1;

    while(remaning(5,frequencies))
    {
        min=get_min(5,frequencies);
        Node *aux = patientsNames.begin_queue;
        char pName[50];
        for(x=0 ; x<min ; x++)
            aux = aux->next;
        strcpy(pName, aux->name);
        int it=0;
        int min_ = get_min(2,delayed_time);
        available[min_]=0;
        for(it=0 ; it<M_doctors ; it++)
        {
            if(available[it]==0)
            {
                Node *aux = drNames.begin_queue;
                for(x=0 ; x<it ; x++)
                    aux = aux->next;
                available[it]=1;
                frequencies[min] = 1000;
                printf("%s ",aux->name);
                printf("%s ",pName);
                printf("%d ",expected_times_real[min]);
                printf("%d\n",delayed_time[it]);
                if((expected_times_real[min] - delayed_time[it]) < 0)
                    perc++;
                delayed_time[it] = 1000;
                //    delayed_time[it] = attending_time(time_[i],rand_times[min]);
                //    delayed_time[it] = rand_times[min];
                break;
            }
        }
    }
    printf("%.1f ",(perc/5.)*100);
    printf("porcento\n");
/*
    printf("\n");
    for(y=0; y<2 ; y++)
    {
        printf("del time: %d\n",delayed_time[y]);
    }
    for(y=0; y<5 ; y++)
    {
        printf("rand: %d\n",rand_times[y]);
    }
    printf("\n");
    for(y=0; y<5 ; y++)
    {
        printf("freq: %d\n",frequencies[y]);
    }
*/
}

//loads the file to global variables
int load_data( char file[])
{
    int i,j;
    char line[line_max];
    FILE *instream;
    instream = fopen(file,"r");
    if (instream == NULL)
    {
        printf("\nerror opening file\n");
        return (-1);
    }
    printf("Data received:\n\n");
    fscanf(instream, "%d" ,&M_doctors);
    printf("%d\n", M_doctors);
    for(i=0 ; i<M_doctors ; i++)
    {
        fscanf(instream, "%s" ,&line);
        fila_insert(&drNames, line);
    }
    print_queue(drNames);
    printf("\n");
    do{
        fscanf(instream, "%d" ,&T_time);
        if(T_time==-1)
            break;
        printf("\n%d\n", T_time);
        time_[N_group] = T_time;
        fscanf(instream, "%d" ,&N_patient);
        printf("%d\n", N_patient);
        patient[N_group] = N_patient;
        for(i=0 ; i<N_patient ; i++)
        {
            fscanf(instream, "%s" ,&line);
            fila_insert(&patientsNames, line);
            printf("%s\t", line);
            for(j=0 ; j<N_questions ; j++)
            {
                fscanf(instream, "%s" ,&line);
                fila_insert(&patientsAnswers, line);
                printf("%s  ", line);
            }
            printf("\n");
        }
        N_group++;
    }while(T_time!=-1);
    fclose(instream);
    return 0;
}

//initializes structures
void init()
{
    fila_init(&drNames);
    fila_init(&patientsNames);
    fila_init(&patientsAnswers);
}

//main program
int main(void)
{
    srand(time(0));
    init();
    load_data("input.txt");
    algorithm();

    return 0;
}
