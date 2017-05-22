/*
-----------------------------------------------------------------------------
 Name      : ordered_list.c
 Date	   : September 1, 2014
 Class     : CS315, Data Structures and the Analysis of Algorithms
 Author    : Renan A. B. Vieira
 Op. Syst. : Developed in Code::Blocks / Windows 8
 Descript. : This program ask the user some integers and store them in a ordered list
-----------------------------------------------------------------------------
*/

/*library*/
#include<stdio.h>
#include<stdlib.h>

/*----------------------------------- list ----------------------------------------
|  attributes:
|           (int)   data: stores a integer
|           (list)  next: stores a the address of the next node of the list
*---------------------------------------------------------------------------------*/
typedef struct list
{
    int data;
    struct list *next;
} Tlist;
typedef Tlist *Plist;

/*----------------------------------- print_node ----------------------------------
|  Purpose: this function travels the list and print its content
|
|  Inputs:  the list which will be printed
|
|  Return:  void
*---------------------------------------------------------------------------------*/
void print_node(Plist list)
{
    Plist temp;
    for(temp=list ; temp->next!=NULL ; temp=temp->next)
        printf("%d  ->  ", temp->data);
        printf("%d", temp->data);
}

/*----------------------------------- print_list ------------------------------------
|  Purpose: this function checks if the list is empty, if not it will call print_node()
|
|  Inputs:  the list which will be printed
|
|  Return:  void
*-----------------------------------------------------------------------------------*/
void print_list(Plist list)
{
    if(list==NULL)
    {
        printf("\nyour list is empty\n");
        return;
    }
    printf("\n\nOK, here's your list in numeric order:\n\n");
    print_node(list);
    printf("\n\n");
}

/*----------------------------------- insert_node ----------------------------------
|  Purpose: this function receives a integer and store this in a new node;
|           then, it checks the right place in the list to store this node.
|
|  Inputs:
|         list:     the whole list in order to insert new nodes
|         value:    int value which will add in the new node of the list
|
|  Return:  it returns the whole list sorted
*---------------------------------------------------------------------------------*/
Plist insert_node(Plist list, int value)
{
    Plist new_node = (Plist)malloc(sizeof(Tlist));
    Plist temp;
    new_node->data = value;
    if(list!=NULL && value > list->data)
    {
        for(temp=list ; temp->next!=NULL && value > temp->next->data ; temp=temp->next);
        new_node->next = temp->next;
        temp->next = new_node;
        return list;
    }
    else
        new_node->next = list;
    return new_node;
}

/*----------------------------------- create_list ----------------------------------
|  Purpose: this function obtains integers typed from the user and call other function
|           in order to store that in an order way.
|
|  Return:  it returns the address of the start node of the ordered list
*---------------------------------------------------------------------------------*/
Plist create_list()
{
    Plist list=NULL;
    int new_data=-1;
    printf("type integer numbers or 0 to stop\n\n");
    for( ; ; )
    {
        printf("Enter an integer:\t");
        scanf("%d", &new_data);
        if(!new_data)
            break;
        list = insert_node(list, new_data);
    }
    print_list(list);
    return list;
}

/*----------------------------------- free_list -----------------------------------
|  Purpose: this function destroy the list
|
|  Inputs:  the list which will be deleted
|
|  Return:  void
*---------------------------------------------------------------------------------*/
void free_list(Plist list)
{
    Plist temp;
    for(temp=list ; list!=NULL ; temp=list)
    {
        list = temp->next;
        free(temp);
    }
}

/*----------------------------------- main ----------------------------------------
|  Purpose: the main function calls the others functions of the program.
*---------------------------------------------------------------------------------*/
int main(void)
{
    Plist list;
    list = create_list();
    free_list(list);
    return 0;
}

