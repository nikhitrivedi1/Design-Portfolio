/* 
	Project 5 
	
	Nikhil Trivedi 

*/


/* Included Libraries */ 
#include <stdlib.h>
#include <string.h> 
#include <assert.h>
#include <stdio.h>
#include "pqueue.h"


/* Structure Declaration */ 
typedef struct pqueue
{
	int count; 
	int length; 
	void **data; 
	int (*comp)();
}PQ; 

/* Function Declarations */ 
PQ *createQueue(int (*comp)()); 
void destroyQueue(PQ*); 
int numEntries(PQ*); 
void addEntry(PQ*, void*); 
void *removeEntry(PQ *pq); 



// return a pointer to a new priority queue 
PQ *createQueue(int (*compare)())
{
	// create a PQ pointer 
	PQ *pq =  malloc(sizeof(PQ)); 
	pq->count = 0; 
	pq->length = 10; 
	pq->comp = compare; 
	pq->data = malloc(pq->length * sizeof(void*)); 
	return pq;  
}


// dellocate memory associated with the priority queue
void destroyQueue(PQ *pq)
{
	//assert 
	assert(pq != NULL);
	free(pq); 
	return; 
}
 

// return the number of entries 
int numEntries(PQ *pq)
{
	return pq->count; 
}

// add entry to the priority pointer queue pointed by PQ 
void addEntry(PQ *pq, void *entry)
{
	//assert 
	assert(pq != NULL && entry != NULL);
	// check if priority queue is full 
	// if full use realloc to reallocate a larger chunk of memory 
	if(pq->count == pq->length)
	{	
		pq->length*=2; 
		pq->data= realloc(pq->data,sizeof(void*) * (pq->length)); 
	}
    
	// place the new element at the end of the binary heap 
	pq->data[pq->count] = entry;
	// reheap up 
	int newIndex = pq->count;
	int parentIndex = (newIndex - 1) / 2 ;
    	int result; 
	int flagOut = 0; 
	while(newIndex > 0 && pq->comp(pq->data[parentIndex],entry) > 0 )
	{
		flagOut = 1; 
		// swap the parent and the new index 
		void *temp = pq->data[newIndex]; 
		pq->data[newIndex] = pq->data[parentIndex]; 
		pq->data[parentIndex] = temp;
		newIndex = parentIndex; 
        parentIndex = (newIndex - 1) /2;
	}
	if(flagOut == 0)
		pq->data[newIndex] = entry; 
	pq->count++;
	return; 	
} 

// remove and return the smallest entry from the priority queue 
void *removeEntry(PQ *pq)
{
	assert(pq != NULL); 
	// go to the smallest entry of the priority queue (the top) 
	void *root = pq->data[0];
	// swap with the last element 
    pq->data[0] = pq->data[pq->count-1];
	pq->count--; 
	//reheap down
	int currentIndex = 0;
    	int childMin;
	// stopping conditions 
	while( (2*currentIndex +1)< pq->count)
	{
		//define the right and left children 
		int leftChild = 2*currentIndex + 1; 
		int rightChild = 2*currentIndex + 2; 
		//check if the right node exists
        
		if(rightChild < pq->count)
		{
			//find the mininum of the children 
			childMin = ((pq->comp(pq->data[leftChild],pq->data[rightChild]) < 0) ? leftChild:rightChild);
            
		}else 
		{
			childMin = leftChild; 
		}
		//compare the child min with the current
        	int result = pq->comp(pq->data[currentIndex],pq->data[childMin]) ;
		if( result > 0 )
		{
			//swap the compare node to the min node 
			void *temp = pq->data[currentIndex]; 
			pq->data[currentIndex] = pq->data[childMin]; 
			pq->data[childMin] = temp; 
			currentIndex = childMin; 
		}else 
		{
			break;
		}	
	}

	return root; 
}
