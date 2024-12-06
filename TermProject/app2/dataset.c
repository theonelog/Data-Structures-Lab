/*
* File: dataset.c
* Author: Awni AlQuraini 
* Lab Section: CSEN12L-1
* Last Updated: 12/6/2024
* Description: This is the dataset implementation for the college.c driver function. It uses direct hashing by ID to store all the elements for
* a super effecient addition, retrieval, and search.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set.h"
#include <assert.h>
#include <stdbool.h>
/*
* This is the set struct that defines all the elements required to make the dataset for the college.c driver file.
*/

typedef struct student{
    int id;
    int age;
} STUD;
/*
* This is the set struct that defines what goes into making a student (an id and age).
*/
typedef struct set{
    int count;
    int length;
    STUD **data;
    char *flags;
} SET;
/*
* Big-O: O(1)
*
* Since the size of our hash table will always be larger than the max # of students to be added, this is essentially a direct hashing function.
*/
int hash(int id, SET *sp){
    assert(sp != NULL);
    return id % sp -> length;
}
/*
* Big-O: O(1)
*
* This function creates a student and then returns a pointer to the student object. It does memory allocation so freeing it is required at some point.
*/
STUD *createStud(int id, int age){
    STUD *sp = malloc(sizeof(STUD));
    assert(sp != NULL);
    sp -> id = id;  
    sp -> age = age;
    return sp;
}
/*
* Big-O: O(n)
*
* Since this function uses a loop to pre-fill each flag with the value "E" (aka empty), the big o of the function is O(n)
* The goal of this function is allocate all the memory required for the set and also sets values for the count and length variables.
*/
SET *createDataSet(int maxElts){
    SET *sp;

    sp = malloc(sizeof(SET));
    assert(sp != NULL);
    sp -> data = malloc(sizeof(STUD) * maxElts);
    sp -> flags = malloc(sizeof(char) * maxElts);
    for(int i = 0; i < maxElts; i++){
        sp -> flags[i] = 'E';
    }
    sp -> length = maxElts;
    sp -> count = 0; 
}
/*
* Big-O: O(n)
*
* Since a student is stored in the data array, we need to free the data that was allocated in the createStud method.
* That is why we must loop through the whole array of data and free any "filled" slots.
*/
void destroyDataSet(SET *sp){
    assert(sp != NULL);
    for(int i = 0; i < sp -> length; i++){
        if(sp -> flags[i] == 'F'){
            free(sp -> data[i]);
        }
    }
    free(sp -> flags);
    free(sp -> data);
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
* Big-O: O(1)
*
* Since we are doing direct hasing by ID, adding is an O(1) operation as we made the array large enough to store the max # of students that can be inserted.
*/
void insertStud(SET *sp, STUD *elt){
    assert(sp != NULL && elt != NULL);
    int index = hash(elt -> id, sp);
    if(sp -> flags[index] == 'F'){
        printf("Student with ID: %d already in the dataset.\n", elt -> id);
        return;
    }
    sp -> data[index] = elt;
    sp -> flags[index] = 'F';
    sp -> count++;
    printf("Successfully added Student | ID: %d | Age: %d\n", elt -> id, elt -> age);
}
/*
* Big-O: O(1)
*
* Since we use direct hashing by ID, removing a student is always an O(1) operation which makes removal simple.
*/
STUD *removeStud(SET *sp, int id){
    assert(sp != NULL);
    int index = hash(id, sp);
    if(sp -> flags[index] == 'E'){
        printf("Student of ID: %d not in the dataset.\n", id);
        return NULL;
    }
    STUD *stud;
    stud = sp -> data[index];
    sp -> flags[index] = 'E';
    sp -> count--;
    printf("Successfully removed student of ID: %d\n", id);
    return stud;
}
/*
* Big-O: O(1)
*
* Since we use direct hashing by ID, searching by ID to return a student is an O(1) operation as we do not need to do any searching or collision
* handling as there SHOULD NOT be more than 1 ID's that produce the same hashed value.
*/
STUD *searchID(SET *sp, int id){
    assert(sp != NULL);
    int index = hash(id, sp);
    if(sp -> flags[index] == 'E'){
        printf("Student of ID: %d not in the dataset.\n", id);
        return NULL;
    }
    STUD *stud;
    stud = sp -> data[index];
    return stud;
}