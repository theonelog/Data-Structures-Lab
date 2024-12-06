/*
* File: college.c - main driver file
* Author: Awni AlQuraini
* Lab Section: CSEN12L-1
* Last Updated: 12/6/2024
* Description: This is the driver file that does all tests required for the 2nd application of the term project. 
* Made to work with the dataset.c file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dataset.c"
#define MAX_STUDENTS 1000
#define MAX_IDVAL 3000
/*
* Big-O: O(1)
*
* Generates a random number between an upper and lower bound.
*/
int genRandom(int lower, int upper){
    return rand() % (upper - lower + 1) + lower;
}

int main(){
    srand(time(NULL));
    int currId = 0;
    SET *ds = createDataSet(MAX_IDVAL);
    for(int i = 0; i < MAX_STUDENTS; i++){
        currId = currId + genRandom(1, 2);
        STUD *stud = createStud((currId), genRandom(18, 30));
        insertStud(ds, stud);
    }
    printf("Size of Student Body: %d\n", numElements(ds));
    STUD *stud = removeStud(ds, genRandom(1, 2000));
    if(stud != NULL){
        printf("Deleted Student Information | ID: %d | Age: %d \n", stud -> id, stud -> age);
    }
    printf("Size of Student Body: %d\n", numElements(ds));
    free(stud);
    destroyDataSet(ds);
    exit(EXIT_SUCCESS);
}