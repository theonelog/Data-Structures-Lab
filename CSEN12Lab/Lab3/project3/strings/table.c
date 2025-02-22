/*
* File: table.c - string implementation
* Author: theonelog 
* Lab Section: CSEN12L-1
* Last Updated: 10/24/2024
* Description: This is the string implementation for a hash table implementation of a Set ADT. This can be compiled with the parity.c and unique.c
files to allow for each other file to work as intended.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set.h"
#include <assert.h>
#include <stdbool.h>
/*
* This is the set struct that defines all the elements that make up a set for a string implementation.
*/
struct set{
    int count;
    int length;
    char **data;
    char *flags;
};
/*
* Big-O: O(n)
*
* Since this function uses a loop to pre-fill each flag with the value "E" (aka empty), the big o of the function is O(n)
* The goal of this function is allocate all the memory required for the set and also sets values for the count and length variables.
*/
SET *createSet(int maxElts){
    SET *sp;

    sp = malloc(sizeof(SET));
    assert(sp != NULL);
    sp -> data = malloc(sizeof(char *) * maxElts);
    sp -> flags = malloc(maxElts);
    for(int i = 0; i < maxElts; i++){
        sp -> flags[i] = 'E';
    }
    sp -> length = maxElts;
    sp -> count = 0;    
}
/*
* Big-O: O(n)
*
* Since this function has a loop, the big O is O(n).
* This function creates a hash for a given string.
*/
unsigned strhash(char *s){
    assert(s != NULL);
    unsigned hash = 0;
    while (*s != '\0'){
        hash = 31 * hash + *s ++;
    }
    return hash;
}

/*
* Big-O: O(n)
*
* This is the search function that finds the index of a given item elt and returns said index if found. When the location is not found
* initially, it will then probe to the next location that the given element can be. 
* The location i is calculated by hashing the element then adding the probe value and then applying the modulo operation
* on the hashed value.
* If the location that is found has a flag of 'D' and the first available slot variable is equal to its default value of -1, 
* availabe is then set to the current i value, which is then recaclculated with the new probe value. 
*/

static int search(SET *sp, char *elt, bool *found){
    assert(sp != NULL && elt != NULL);
    int available, i, probe;
    probe = 0;
    available = -1;
    i = (strhash(elt)) % sp -> length;
    while(i < sp -> length){
        assert(i < sp -> length);
        if(sp -> flags[i] == 'F'){
            if(strcmp(elt, sp -> data[i]) == 0){
                *found = true;
                available = i; 
                i = sp ->length;
            }   
            probe++;
            i = (strhash(elt) + probe) % sp -> length;
        }
        else if(sp -> flags[i] == 'D' && available == -1){
            available = i;
            probe++;
            i = (strhash(elt) + probe) % sp -> length;
        }
        else if(sp -> flags[i] == 'D'){
            probe++;
            i = (strhash(elt) + probe) % sp -> length;
        }
        else if(available == -1){
            available = i;
            i = sp->length;
        }
        else{
            i = sp->length;
        }
    }
    return available;
}   
/*
* Big-O: O(n)
*
* This function goes and frees all memory that we manually allocated. 
* Since we manually allocated the memory by using strdup for each element, we need to free all the data first then we can free the array pointer.
* Since we only manually allocated memory for the flags array pointer, we can just free the entire flags pointer without going in and manually doing so.
*/
void destroySet(SET *sp){
    assert(sp != NULL);
    for(int i = 0; i < sp -> length; i++){
        if(sp -> flags[i] == 'F'){
            free(sp -> data[i]);
        }
    }
    free(sp -> data);
    free(sp -> flags);
    free(sp);
}
/*
* Big-O: O(1)
*
* This function makes sure that the set pointer isn't null and then returns the count value of the set pointer.
*/
int numElements(SET *sp){    
    assert(sp != NULL);
    return sp -> count;
}
/*
* Big-O: O(n)
*
* The Big-O is O(n) as the search function is a O(n) operation and I made the method, therefore I must account for its Big-O runtime
* Add element makes sure the element exists and then if it does not, it will then add the elt at the location by duplicating elt
* It will then define the flag at the location as "F", representing filled.
* Count is then incremented by 1.
*/
void addElement(SET *sp, char *elt){
    bool exists = false;
    assert(sp != NULL && elt != NULL);
    int location = search(sp, elt, &exists);
    if(!exists){
        sp -> data[location] = strdup(elt);
        sp -> flags[location] = 'F';
        sp -> count++;
    }
}
/*
* Big-O: O(n)
*
* Since we must search to find the element first, the Big-O is O(n) even though every other operation is O(1)
* If the element elt is found, we then free the data at the found location as we manually allocated it and set the flag at the location to "D" to indicate there was once data there.
*/
void removeElement(SET *sp, char *elt){
    assert(sp != NULL && elt != NULL);
    bool exists = false;
    int location = search(sp, elt, &exists);
    if(exists){
        free(sp -> data[location]);
        sp -> flags[location] = 'D';
        sp -> count--;
    }
}
/*
* Big-O: O(n)
*
* Since we must use the search function to figure out if the element elt is in the hash table, it is an O(n) function
* Once we do find the location, we then check if the exists variable is true. If it is, we then return the data at the location.
* If it is not found, we return NULL
*/
char *findElement(SET *sp, char *elt){
    assert(sp != NULL && elt != NULL);
    bool exists = false;
    int location = search(sp, elt, &exists);

    return exists ? sp -> data[location] : NULL;
}
/*
* Big-O: O(n)
*
* Since we have a loop, the Big-O of the function is O(n).
* In this function, we create a new string array that will hold a copy of all the data.
* We then iterate through the entire data array as hashing places each element in a random location.
* If the location we are at has a flag of "F", we duplicate the data at data[i] to elts[j]
* the reason we use a seperate j incrementor is to reduce the memory impact of the elts array as we only need it to be as big as the count variable.
* If we used i, we would get a segmentation fault as we would be trying to access data we aren't supposed to.
* the function then returns the elts array with all the elements. Since we can't free the data in the function, the user must do it themselves in their implementation.
*/
char **getElements(SET *sp){
    assert(sp != NULL);
    char **elts = malloc(sizeof(char *) * sp -> count);
    int j = 0;
    for(int i = 0; i < sp -> length; i++){
        if(sp -> flags[i] == 'F'){
            elts[j++] = strdup(sp -> data[i]);
        }
    }   
    return elts;
}
