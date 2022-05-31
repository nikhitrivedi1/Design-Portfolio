/*
 * File:        table.c
 *
 * Copyright:	2017, Darren C. Atkinson
 *
 * Description: This file contains the public and private function and type
 *              definitions for a set abstract data type for generic
 *              pointer types.  A set is an unordered collection of unique
 *              elements.
 *
 *              This implementation uses a hash table to store the
 *              elements, with linear probing to resolve collisions.
 *              Insertion, deletion, and membership checks are all average
 *              case constant time.
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <stdbool.h>
# include "set.h"

# define EMPTY   0
# define FILLED  1
# define DELETED 2

struct set {
    int count;                  /* number of elements in array */
    int length;                 /* length of allocated array   */
    void **data;                /* array of allocated elements */
    char *flags;                /* state of each slot in array */
    int (*compare)();		/* comparison function         */
    unsigned (*hash)();		/* hash function               */
};

void **quickSort(SET*,void**,int,int);
int partition(SET*,void**,int,int);
/*
 * Function:    search
 *
 * Complexity:  O(1) average case, O(n) worst case
 *
 * Description: Return the location of ELT in the set pointed to by SP.  If
 *		the element is present, then *FOUND is true.  If not
 *		present, then *FOUND is false.  The element is first hashed
 *		to its correct location.  Linear probing is used to examine
 *		subsequent locations.
 */

static int search(SET *sp, void *elt, bool *found)
{
    int available, i, locn, start;


    available = -1;
    start = (*sp->hash)(elt) % sp->length;

    for (i = 0; i < sp->length; i ++) {
        locn = (start + i) % sp->length;

        if (sp->flags[locn] == EMPTY) {
            *found = false;
            return available != -1 ? available : locn;

        } else if (sp->flags[locn] == DELETED) {
            if (available == -1)
		available = locn;

        } else if ((*sp->compare)(sp->data[locn], elt) == 0) {
            *found = true;
            return locn;
        }
    }

    *found = false;
    return available;
}


/*
 * Function:    createSet
 *
 * Complexity:  O(m)
 *
 * Description: Return a pointer to a new set with a maximum capacity of
 *		MAXELTS.
 */

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
    int i;
    SET *sp;


    assert(compare != NULL && hash != NULL);

    sp = malloc(sizeof(SET));
    assert(sp != NULL);

    sp->data = malloc(sizeof(char *) * maxElts);
    assert(sp->data != NULL);

    sp->flags = malloc(sizeof(char) * maxElts);
    assert(sp->flags != NULL);

    sp->compare = compare;
    sp->hash = hash;
    sp->length = maxElts;
    sp->count = 0;

    for (i = 0; i < maxElts; i ++)
        sp->flags[i] = EMPTY;

    return sp;
}


/*
 * Function:    destroySet
 *
 * Complexity:  O(m)
 *
 * Description: Deallocate memory associated with the set pointed to by SP.
 *		The elements themselves are not deallocated since we did not
 *		allocate them in the first place.  That's the rule: if you
 *		didn't allocate it, then you don't deallocate it.
 */

void destroySet(SET *sp)
{
    assert(sp != NULL);

    free(sp->flags);
    free(sp->data);
    free(sp);
}


/*
 * Function:    numElements
 *
 * Complexity:  O(1)
 *
 * Description: Return the number of elements in the set pointed to by SP.
 */

int numElements(SET *sp)
{
    assert(sp != NULL);
    return sp->count;
}


/*
 * Function:    addElement
 *
 * Complexity:  O(1) average case, O(n) worst case
 *
 * Description: Add ELT to the set pointed to by SP.
 */

void addElement(SET *sp, void *elt)
{
    int locn;
    bool found;


    assert(sp != NULL && elt != NULL);
    locn = search(sp, elt, &found);

    if (!found) {
	assert(sp->count < sp->length);

	sp->data[locn] = elt;
	sp->flags[locn] = FILLED;
	sp->count ++;
    }
}


/*
 * Function:    removeElement
 *
 * Complexity:  O(1) average case, O(n) worst case
 *
 * Description: Remove ELT from the set pointed to by SP.  A element is
 *		deleted by changing the state of its slot.
 */

void removeElement(SET *sp, void *elt)
{
    int locn;
    bool found;


    assert(sp != NULL && elt != NULL);
    locn = search(sp, elt, &found);

    if (found) {
	sp->flags[locn] = DELETED;
	sp->count --;
    }
}


/*
 * Function:    findElement
 *
 * Complexity:  O(1) average case, O(n) worst case
 *
 * Description: If ELT is present in the set pointed to by SP then return
 *		it, otherwise return NULL.
 */

void *findElement(SET *sp, void *elt)
{
    int locn;
    bool found;


    assert(sp != NULL && elt != NULL);

    locn = search(sp, elt, &found);
    return found ? sp->data[locn] : NULL;
}

/* allocate and return an array of elements in the set */
/* O(n) -> traversing through the array to find all non null elements */
void
*getElements(SET *sp)
{
    // malloc the void ** array
    void **array = malloc(sizeof(void*) * sp->count);
    //traverse through the set and copy elements to this array
    int i;
    int arrayCount = 0;
    for(i = 0; i < sp->length; i++)
    {
        if(sp->flags[i] == FILLED)
        {
            array[arrayCount] = sp->data[i];
            arrayCount++;
        }
        
    }
    
    return quickSort(sp,array,0,arrayCount-1);
    
}


/* Quick Sort Function */
void
**quickSort(SET *sp,void **array, int low, int high)
{
    if(low < high) //stopping condition -> low index is higher than high index
    {
        printf("CHECK1\n");
        // then you want to continue the recursive sequence
        // get the partition index
        int part = partition(sp,array,low,high);
        
        quickSort(sp,array,low,part-1) ;
        quickSort(sp,array,part+1,high);
    }
    return array;
};

int
partition(SET *sp,void **array, int low, int high)
{
    printf("CHECK2\n");
    //designate the pivot as the high element
    void *pivot = array[high];
    //get the index of the smaller element
    int i = (low);
    int j;
    void *temp;
    // traverse through line until values are less than pivot
    for(j = low; j < high; j++)
    {
        //check if value is less than pivot
        printf("CHECK3 %d\n",j);
        if(sp->compare(array[j] , pivot) <= 0)
        {
            printf("CHECK4\n");
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
        }
    }
    temp = array[i];
    array[i] = array[high];
    array[high] = temp;
    return i;
}



