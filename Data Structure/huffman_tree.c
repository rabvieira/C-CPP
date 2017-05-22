/*

-----------------------------------------------------------------------------------

 Name      : huffman.c

 Date	  : November 6, 2014

 Class     : CS315, Data Structures and the Analysis of Algorithms

 Author    : Renan A. B. Vieira

 Descript. : In this program is implemented a Huffman Tree from a "little end up"

             heap inside an array. It is asked to provide data, i.e., character and

             frequency until the user types the integer 0.

             As a output, is printed the heap inside the array and the Huffman Tree

             in pre-order and in-order.

-----------------------------------------------------------------------------------

*/



/*libraries*/

#include<stdio.h>

#include<stdlib.h>

#include <string.h>

int const size_array_max = 10; /* Type here the size maximum of the data*/



/*----------------------------------- tree ----------------------------------------

|  attributes:

|           (int)   ch: stores a integer which identify the node

|           (int)   fr: stores a integer which represents the frequency of the node

|           (tree)  left: stores the address of left child of this node

|           (tree)  right: stores the address of right child of this node

*---------------------------------------------------------------------------------*/

typedef struct tree

{

    int ch, fr;

    struct tree *left, *right;

} TTree;

typedef TTree *PTree;



/*----------------------------------- print_array ----------------------------------

|  Purpose: this function travels the heap and print its contents

|   Inputs:

|       heap        The heap inside an array which will be printed (character,frequency)

|       size_array  The size of the heap

*---------------------------------------------------------------------------------*/

void print_array(PTree heap[], int size_array)

{

    int it;

    printf("\nHeap inside an array [ch,fr]:\n");

    for(it=1 ; it<size_array ; it++)

        printf("[%d,%d] ",heap[it]->ch, heap[it]->fr);

}



/*----------------------------------- insert_heap ----------------------------------

|  Purpose: this function adds a new node inside a heap implemented in an array

|           the parent of the new node, which in the beginning is the last position,

|           is the floor(child/2). If the insertion of the new node violates the heap

|           property, the new node is swapped up until satisfied it.

|           Heap Property: The father is smaller of its both children.

|   Inputs:

|       heap   The heap inside an array which will add a new node

|       node   The node that will be add inside the heap preserving its properties

|       pos    Current size of the heap

*---------------------------------------------------------------------------------*/

void insert_heap(PTree heap[], PTree *node, int pos)

{

    int parent = pos/2;

    PTree temp;

    while(parent>0 && (*node)->fr < heap[parent]->fr)

    {

        temp = heap[parent];

        heap[parent] = (*node);

        heap[pos] = temp;

        pos = parent;

        parent = parent/2;

    }

}



/*----------------------------------- younger -------------------------------------

|  Purpose: Since the left child of a parent p is 2p, and right child is 2p+1, this

|           function return the position of the smaller child (right/left)

|   Inputs:

|       heap   The heap inside an array

|       parent integer which represents the position of the parent in analysis

|       size_array  The size of the heap

|   Return:

|       An integer which represents the smaller child of the parent (right/left)

*---------------------------------------------------------------------------------*/

int younger(PTree heap[], int parent, int size_array)

{

    int child=0, right, left;

    right = 2*parent+1;

    left  = 2*parent;

    if((right < size_array) && (heap[right]->fr < heap[left]->fr))

        child = right;

    else if(left < size_array)

        child = left;

    return child;

}



/*----------------------------------- remove_heap ----------------------------------

|  Purpose: Since the root of a heap is the smallest frequency, this function return

|           it. However, it is necessary fixed the heap after doing that, so that

|           the last node becomes the main root and it is swapped down until the heap

|           satisfy its own property.

|           Heap Property: The father is smaller of its both children.

|   Inputs:

|       heap   The heap inside an array

|       size_array  The size of the heap

|   Return:

|       The root of the heap since it is the smallest frequency

*---------------------------------------------------------------------------------*/

PTree remove_heap(PTree heap[], int size_array)

{

    int parent=1, child;

    PTree root = heap[1], temp;

    heap[1] = heap[size_array];

    child = younger(heap,parent,size_array);

    while(child>0 && heap[parent]->fr > heap[child]->fr)

    {

        temp = heap[parent];

        heap[parent] = heap[child];

        heap[child] = temp;

        parent = child;

        child = younger(heap,parent,size_array);

    }

    return root;

}



/*----------------------------------- built_tree ----------------------------------

|  Purpose: This function creates a Huffman Tree from a heap inside an array.

|           It is created a new node. Its right and left child comes from a remove

|           operation in the heap. Then, this is put back into the heap until remain

|           one node. This is the root of the Huffman Tree.

|   Inputs:

|       heap   The heap inside an array

|       size_array  The size of the heap

*---------------------------------------------------------------------------------*/

void built_tree(PTree *huff, PTree heap[], int size_array)

{

    int pos;

    if(size_array>2)

    {

        for(pos=1 ; pos<size_array-1 ; pos++)

        {

            PTree new_node = (PTree)malloc(sizeof(TTree));

            new_node->left  = remove_heap(heap, size_array-pos);

            new_node->right = remove_heap(heap, size_array-pos-1);

            new_node->ch = 0;

            new_node->fr = new_node->left->fr + new_node->right->fr;

            heap[size_array-pos-1] = new_node;

            insert_heap(heap, &new_node, size_array-pos-1);

        }

    }

    (*huff) = heap[1];

}



/*----------------------------------- print_tree_recIN -----------------------------

|  Purpose: this function travels the Huffman Tree and print it In-Order recursively

|   Inputs:

|       Hufmman  The pointer to the Huffman Tree's root

*---------------------------------------------------------------------------------*/

void print_tree_recIN(PTree *Huffman)

{

    if((*Huffman) != NULL)

    {

        print_tree_recIN(&(*Huffman)->left);

        if((*Huffman)->ch==0)

            printf("-\t%d\n", (*Huffman)->fr);

        else

            printf("%d\t%d\n", (*Huffman)->ch, (*Huffman)->fr);

        print_tree_recIN(&(*Huffman)->right);

    }

}



/*----------------------------------- print_tree_recPRE ----------------------------

|  Purpose: this function travels the Huffman Tree and print it Pre-Order recursively

|   Inputs:

|       Hufmman  The pointer to the Huffman Tree's root

*---------------------------------------------------------------------------------*/

void print_tree_recPRE(PTree *Huffman)

{

    if((*Huffman) != NULL)

    {

        if((*Huffman)->ch==0)

            printf("-\t%d\n", (*Huffman)->fr);

        else

            printf("%d\t%d\n", (*Huffman)->ch, (*Huffman)->fr);

        print_tree_recPRE(&(*Huffman)->left);

        print_tree_recPRE(&(*Huffman)->right);

    }

}



/*----------------------------------- main ----------------------------------------

|  Purpose: This function asks to the user to provide data, i.e., integer identifier

|           and frequency of this. Then a new node of a tree is created and stored

|           inside a heap.  The others function of this program is called in order

|           to implements a Huffman Tree. Finally, this Tree is printed

*---------------------------------------------------------------------------------*/

int main(void)

{

    int pos, size_array=1, character, frequency;

    PTree heap[size_array_max];

    PTree huff=NULL;

    for( pos=1 ; ; pos++)

    {

        if(size_array >= size_array_max)

            break;

        printf("Enter an integer (enter a '0' to quit entering integers): ");

        scanf("%d", &character);

        if(character == 0)

            break;

        printf("  Enter '%d's frequency: ",character);

        scanf("%d", &frequency);

        size_array ++;

        PTree new_node = (PTree)malloc(sizeof(TTree));

        new_node->left = NULL;

        new_node->right = NULL;

        new_node->ch = character;

        new_node->fr = frequency;

        heap[pos] = new_node;

        insert_heap(heap, &new_node, pos);

    }

    if(size_array>1)

    {

        print_array(heap, size_array);

        built_tree(&huff, heap, size_array);

        printf("\n\nThe nodes of the Huffman tree in In-order are: [ch,fr]\n");

        print_tree_recIN(&huff);

        printf("\n\nand in Pre-order are: [ch,fr]\n");

        print_tree_recPRE(&huff);

    }

    return 0;

}