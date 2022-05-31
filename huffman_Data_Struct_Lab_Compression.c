/* 

	Project 5: Huffman Code 

	Nikhil Trivedi 


	Step1: Read the file and count the frequency of each character 
	Step2: 
	Step3: 

    aadolfsson@scu.edu
*/

/* Library Declaration */ 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "pack.h"
#include "pqueue.h"



/* Global Variables */
struct node *nodeList[257];

/* Function Declaration */ 
struct node *makeNode(int,struct node*, struct node*);
int getDepth(struct node*, int);
static int compare(struct node*,struct node*);


/* Functions */ 
int main(int argc, char *argv[])
{

	// file pointer 
	FILE *fp; 
	//check if file is successfully open 
	if((fp = fopen(argv[1],"r")) == NULL)
		return 0; 
	int counts[257]; // Upper case and lower case alphabet values plus a space 
	//initialize each value as zero 
	int i; 
	for(i = 0; i<257; i++)
	{
		counts[i] = 0; 
		nodeList[i] = NULL; 
	}
	int  ch; 
	// read the character 
	// get the frequency of each character 
	while ((ch = getc(fp)) != EOF)
		counts[ch]++;
    fclose(fp);
//    //allocate memory for the node list
    //create the priority queue
    PQ *pq = createQueue(compare);
    //traverse through the counts array to add the entry
    for( i = 0; i < 257; i++)
    {
        //make one line
        if(counts[i] > 0)
        {
            nodeList[i] = makeNode(counts[i],NULL,NULL);
            addEntry(pq,nodeList[i]);
        }

	if(i == 256)
	{
		nodeList[i] = makeNode(0,NULL,NULL);
		addEntry(pq,nodeList[i]); 
   	}
    }
    //create a blank node with data as zero
    addEntry(pq,makeNode(0,NULL,NULL));
    //create the Huffman tree
    while(numEntries(pq) >1)
    {
	struct node *node1 = removeEntry(pq); 
	struct node *node2 = removeEntry(pq); 
	int parentData = node1->count + node2->count; 
     	addEntry(pq,makeNode(parentData,node1,node2));    
    }
    for(i =0; i< 257; i++)
    {
	if(nodeList[i] != NULL)
	{
        	//traverse up the list first
        	int depth = getDepth(nodeList[i],0);
       	 	//print out in the format designated
        	char c = i;
        	int prod = depth * nodeList[i]->count;
		(isprint(c) != 0)? printf("'%c' : %d x %d bits = %d\n",c,nodeList[i]->count,depth,prod):printf("%d : %d x %d bits = %d\n",i,nodeList[i]->count,depth,prod); 
   	}
     }
    pack(argv[1],argv[2],nodeList);
    return 0;
}


/* Makes a parent node from the data value */
struct node
*makeNode (int data, struct node *n1, struct node *n2)
{
    struct node *node = malloc(sizeof(struct node));
    node->count = data;
    if(n1 != NULL && n2 != NULL){
        n1->parent = node;
        n2->parent = node;
    }
    return node;
}

/* gets the depth of the given leaf*/
int
getDepth(struct node *leaf,int depth)
{
    //create the bit encoder from top down
    if(leaf == NULL)
        return depth;
    //traverse the list from bottom to top because only parent pointer is known
    return getDepth(leaf->parent,++depth);
    
}

/* local compare function */
static int compare(struct node *num1, struct node *num2)
{
    //function used when creating the priority queue
    if(num1->count > num2->count)
        return 1;
    if(num1->count < num2->count)
        return -1;
    return 0;
}

