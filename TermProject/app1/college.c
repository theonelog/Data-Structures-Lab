/*
* File: college.c - main driver file
* Author: theonelog
* Lab Section: CSEN12L-1
* Last Updated: 12/6/2024
* Description: This is the driver file that does all tests required for the 1st application of the term project. 
* Made to work with the dataset.c file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dataset.c"
#define MAX_STUDENTS 1000
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
    SET *ds = createDataSet();
    for(int i = 0; i < MAX_STUDENTS; i++){
        currId = currId + genRandom(1, 2);
        STUD *stud = createStud((currId), genRandom(18, 30));
        //printf("Iteration %d | Inserted Student Age: %d\n", i, stud -> age);
        insertStud(ds, stud);
        //printf("Iteration %d | Current Age Gap: %d\n", i, maxAgeGap(ds));
    }
    printf("Size of Student Body: %d\n", numElements(ds));
    printf("Max age gap: %d\n", maxAgeGap(ds));
    int *arrSize = malloc(sizeof(int));
    STUD **studs = removeStud(ds, genRandom(18, 30), arrSize);
    for(int i = 0; i < *arrSize; i++){
        printf("Deleted Student Information | ID: %d | Age: %d \n", studs[i] -> id, studs[i] -> age);
    }
    printf("Size of Student Body: %d\n", numElements(ds));
    free(studs);
    printf("Max age gap: %d\n", maxAgeGap(ds));
    free(arrSize);
    destroyDataSet(ds);
    exit(EXIT_SUCCESS);
}