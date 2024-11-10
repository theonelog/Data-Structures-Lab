/*
* File: unsorted.c
* Author: Awni AlQuraini
* Last Updated: 10/9/2024
* Description: This file an implementation of the Set ADT using an unsorted array. This can be used with the parity.c and unique.c files to run
a variety of comparissons. The search algorithm used in this implementation is a sequential/linear search.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set.h"
#include <assert.h>

struct set{
    int count;
    int length;
    char **data;
};
/*
* Big-O: O(1)
* 
* This fuction creates a set with a parameter of max elements. First, it assigns a set pointer named "sp". It then checks to see if the
pointer was successfully created and points to a valid chunk of memory. After doing so, it uses the sp pointer to allocate memory and/or define
the variables of a given set. The only significant assignment is the length being assigned to max elements.
*/
SET *createSet(int maxElts){
    SET *sp;

    sp = malloc(sizeof(SET));
    assert(sp != NULL);
    sp -> data = malloc(sizeof(char *) * maxElts);
    sp -> length = maxElts;
    sp -> count = 0;
}
/*
* Big-O: O(n)
*
* This is the search function that finds the index of a given item elt and returns said index. If it fails to find it, it returns a -1. 
* The searching algorithm implemented in the search function is sequential search. The reason why it is O(n) is because it has a loop.
* This searching algorithm works by going through every element in the array until it finds the element elt. 
*/
static int search(SET *sp, char *elt){
    for(int i = 0; i < sp -> count; i++){
        if(strcmp(sp -> data[i], elt) == 0){
            return i;
        }
    }
    return -1;
}
/*
* Big-O: O(n)
*
* This function loops through the entire data array and deallocates any memory allocated to the elements of the data array and then nukes the data array pointer
and the pointer to the entire set. The reason why it is O(n) is because it has a loop.
*/
void destroySet(SET *sp){
    for(int i = 0; i < sp -> count; i++){
        free(sp -> data[i]);
    }
    free(sp -> data);
    free(sp);
}
/*
* Big-O: O(1)
*
* This returns the current # of elements in the set. This is stored in the variable count so all this function does is return the count variable in a given set 
thru the pointer sp.
*/
int numElements(SET *sp){
    return sp -> count;
}
/*
* Big-O: O(1)
*
* This function adds an element to the end of the array and increments count. 
* Since the implementation is with an unsorted array, adding elements to the set will take O(1) as there is no neccessity to have a loop in this function. 
8 Eeven this function contains the search method which is o(n), it is only counted as 1 operation as it is just called.
*/
void addElement(SET *sp, char *elt){
    assert(sp -> count < sp -> length);
    if(search(sp,elt) == -1){
        sp -> data[sp -> count++] = strdup(elt);
    }
}
/*
* Big-O: O(1)
* 
* This function removes an element from the array and replaces the last element in the array into the element that was removed's position. It then decrements count.
*/
void removeElement(SET *sp, char *elt){
    int i = search(sp, elt);
    if(i != -1){
        sp -> data[i] = sp -> data[sp -> count - 1];
        sp -> data[sp -> count - 1] = NULL;
        sp -> count--;
    }
}
/*
* Big-O: O(1)
*
* This function finds an element in the array and then returns the pointer to it. If it fails to find it, it then returns NULL.
*/
char *findElement(SET *sp, char *elt){
    int index = search(sp, elt);
    if(index != -1){
        return sp -> data[index];
    }
    return NULL;
}
/*
* Big-O: O(n)
*
* This function collets every element from the array and duplicates it into a new array of strings that it returns at the end. Since it has a loop, it is Big-O of n.
*/
char **getElements(SET *sp){
    char **elts = malloc(8 * sp -> count);
    for(int i = 0; i < sp -> count; i++){
        elts[i] = strdup(sp -> data[i]);
    }

    return elts;
}