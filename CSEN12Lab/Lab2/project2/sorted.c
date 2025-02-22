/*
* File: sorted.c
* Author: theonelog
* Last Updated: 10/9/2024
* Description: This file an implementation of the Set ADT using a sorted array. This can be used with the parity.c and unique.c files to run
a variety of comparissons. The search algorithm used in this implementation is a binary search.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set.h"
#include <assert.h>
#include <stdbool.h>

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
* Big-O: O(log n)
*
* The search function for the sorted array implementation of the set is a binary search. As a result, the search algorithm big-o is O(log n).
* Since the array is already sorted, binary search can work as intended.
* In binary search, there is a hi, low, and mid variable that are all used to deduce where the location of a given element (elt) is. 
* If the value at index mid is greater than the value of element, it means that the hi is to hi, therefore it is set to mid -1.
* If the value at the index mid is less than the value of the element, it means that the lo is too low, therefore it is set to mid + 1.
* This repeats until the value at index mid is equal to the element or when the lo value is higher than or equal to the hi value.'
* If the value is found, the found boolean variable parameter is set to true and the index mid is returned.
* If the value can't be found, then the lo variable is returned with the found boolean parameter being set to false.
*/
static int search(SET *sp, char *elt, bool *found){
    int lo = 0;
    int hi = (sp->count - 1);
    int mid; 
    while(lo <= hi){
        mid = (hi + lo)/2;
        if(strcmp(sp->data[mid], elt) == 0){
            *found = true;
            return mid;
        }
        else if(strcmp(sp->data[mid], elt) < 0){
            lo = mid + 1;
        }
        else{
            hi = mid - 1;
        }
    }
    *found = false;
    return lo;
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
    return sp->count;
}
/*
* Big-O: O(n)
*
* This function takes in an element elt to add to the sorted array. First it checks if the element ever existed in the array at a point in time by using the search function.
* It then assigns the return of the search function to location. If the exists boolean is false, then the function will continue to actually add the element.
* Next, the function asserts that the count variable must be less than the length to prevent any seg faults. The function then shifts the array starting from the end of the array
moving every element one to the right leaving an empty hole at the location variable index. 
* The function then sets the element at index location to the element elt passed in as a parameter by duplicating its value and increments count by 1.
*/
void addElement(SET *sp, char *elt){    
    bool exists;
    int location = search(sp, elt, &exists);
    assert(sp != NULL && elt != NULL);

    if(!exists){
        assert(sp -> count < sp -> length);
        for(int i = (sp -> count - 1); i >= location; i--){
            sp -> data[i + 1] = strdup(sp -> data[i]);
        }
        sp -> data[location] = strdup(elt);
        sp -> count++;
    }
}
/*
* Big-O: O(n)
*
* This funciton takes in the parameter elt which is the element the user desires to be removed from the set. 
* The function then checks if the element even exists in the sorted array. It returns the value returned by search into the loc variable.
* Then the function checks if the exist boolean is true, meaning that the element is located in the sorted array. 
* Once it confirms that, the function then iterates from the index stored in the loc variable to count - 1 and shiftes every element back 1, overriding the element at loc. 
* Once it does that, it then sets the element at count - 1 to null and decrements count by 1 as well.
*/
void removeElement(SET *sp, char *elt){
    bool exists;
    int loc = search(sp, elt, &exists);

    if(exists){
        for(int i = loc; i < sp->count-1; i++){
            sp->data[i] = strdup(sp->data[i + 1]);
        }
        sp -> data[sp -> count - 1] = NULL;
        sp->count--;
    }
}
/*
* Big-O: O(1)
* 
* This function takes in the parameter elt which is an element that the user wants to check exists in the set. 
* The function uses the search function to find the location and check if the element does exist by storing a ture or fales value in the exists boolean variable.
* In the return statement, a ternary statement is used which will return the string at the index loc in data or NULL if the value of exists is true and NULL if the value is false. 
*/
char *findElement(SET *sp, char *elt){
    bool exists;
    int loc = search(sp, elt, &exists);
    return exists ? sp->data[loc] : NULL;
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