/*
* File: table.c - chained hash table implementation
* Author: Awni AlQuraini 
* Lab Section: CSEN12L-1
* Last Updated: 11/7/2024
* Description: The implementation of a set with a hash table using linked lists to handle collisions. Works with parity.c and unique.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set.h"
#include "list.c"
#include <assert.h>
#include <stdbool.h>
#define MAX_CHAIN_LENGTH 20 //The max length each linked list in the array can be
/*
* This is the set struct that defines all the elements that make up a set for a generics implementation.
* The data includes a hash function pointer and a compare function pointer.
*/
struct set{
    int count;
    int length;
    LIST **data;
    int (*compare)();
    unsigned (*hash)();
};
/*
* Big-O: O(n)
*
* This function creates a set and returns the set pointer. It pre-fills each element in the data array with a list to prevent any NULL eror issues.
* If it did not pre-fill each index of the data array with a linked list, it would be O(1) but it would create many special cases in other methods. 
*/
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()){
    SET *sp;
    sp = malloc(sizeof(SET));
    assert(sp != NULL && *compare != NULL && *hash != NULL);
    sp -> length = maxElts/MAX_CHAIN_LENGTH;
    sp -> data = malloc(sizeof(LIST) * sp -> length);
    for(int i = 0; i < sp -> length; i++){
        sp -> data[i] = createList(compare);
    }
    sp -> count = 0; 
    sp -> hash = hash;
    sp -> compare = compare;
    return sp;   
}  
/*
* Big-O: O(n^2)
*
* In this function, we go through each element of the array and call the destroyList method for each list in the data array.
* Since destroyList is an O(n) method, the Big-O of the function is O(n^2) as it is nested in another loo[.
* After it destroys every list in data, the data array is then freed and the set pointer is freed.
*/
void destroySet(SET *sp){
    assert(sp != NULL);
    for(int i = 0; i < sp -> length; i++){
        destroyList(sp -> data[i]);
    }
    free(sp -> data);
    free(sp);
}
/*
* Big-O: O(1)
*
* This function has 1 operation which is returning sp -> count.
*/
int numElements(SET *sp){    
    assert(sp != NULL);
    return sp -> count;
}
/*
* Big-O: O(n)
*
* Since this function calls the findItem method, the big-o of the method must be O(n) as findItem is O(n).
* The function by default adds an element to the end of the respective linked list at the index location which is obtained using a hash function.
*/
void addElement(SET *sp, void *elt){
    assert(sp != NULL && elt != NULL);
    int location = sp -> hash(elt) % sp -> length;
    void *item = findItem(sp -> data[location], elt);
    if(item == NULL){
        addLast(sp -> data[location], elt);
        sp -> count++;
    }
}
/*
* Big-O: O(n)
*
* Since this function calls the findItem method, the big-o of the method must be O(n) as findItem is O(n).
* The item that gets returned by findItem is then checked to be NULL. if it is found, it is then checked to be NULL to see if the item even existed in the set.
* The function then calls removeItem which itself is also an O(n) operation. 
* Count is then decremented.
*/
void removeElement(SET *sp, void *elt){
    assert(sp != NULL && elt != NULL);
    int location = sp -> hash(elt) % sp -> length;
    void *item = findItem(sp -> data[location], elt);
    if(item != NULL){
        removeItem(sp -> data[location], item);
        sp -> count--;  
    }
}
/*
* Big-O: O(n)
*
* This function calls findItem to get the element that the user is searching for.
* If the variable element is not NULL, it then returns the element. 
* If not it returns NULL as the element was not found.
*/
void *findElement(SET *sp, void *elt){
    assert(sp != NULL && elt != NULL);
    int location = sp -> hash(elt) % sp -> length;
    void *element = findItem(sp -> data[location], elt);
    return element != NULL ? element : NULL;
}
/*
* Big-O: O(n^2)
*
* Since this function calls getItems within the loop for each linked list, the big o must be O(n^2) as getItems is an O(n) operation.
* If the # of items of the linked list at index i is not 0, the function the goes through and puts all the elements in array form using getItems.
* Since getItems allocates memory that it does not free, we must free the temp array after using memcpy.
* Then, by using memcpy and an offset variable, the data in temp is compied to an elts array which has every element in the entire set. 
* After doing the memcpy operation, the offset is increased by the number of items in the linked list at index i. 
* Once every element is put into the elts array, it is then returned by the function for use by the user. 
* Since we allocate memory for the array in this method and then return the array, the user of the set interface must manually free memory in their program.
*/
void *getElements(SET *sp){
    assert(sp != NULL);
    void **elts = malloc(sizeof(void *) * sp -> count);
    assert(elts != NULL);
    void **temp;
    int offset = 0;
    for(int i = 0; i < sp -> length; i++){
        if(numItems(sp -> data[i]) > 0){
            temp = getItems(sp -> data[i]);
            memcpy(elts + offset, temp, numItems(sp -> data[i]) * sizeof(void *));
            offset+=numItems(sp -> data[i]);
            free(temp);
        }
    }
    return elts;
}