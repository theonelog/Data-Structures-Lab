/*
* File: dataset.c
* Author: theonelog 
* Lab Section: CSEN12L-1
* Last Updated: 12/6/2024
* Description: The implementation of a set with a hash table using linked lists to handle collisions. 
* Made for use with the college.c driver file.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.c"
#include "set.h"
#include <assert.h>
#include <stdbool.h>

#define MIN_AGE 18
#define MAX_AGE 30
#define SET_SIZE 13
/*
* This is the set struct that defines all the elements that make up a set for a generics implementation.
* The data includes a hvoidash function pointer and a compare function pointer.
*/
typedef struct set{
    int count;
    int length;
    int min;
    int max;
    LIST **data;
} SET;
/*
* This is the set struct that defines what goes into making a student (an id and age).
*/
typedef struct student{
    int id;
    int age;
} STUD;
/*
* Big-O: O(1)
*
* This function soley exists to make the list data structure happy. But it can be used to "compare" two students.
*/
static int compare(STUD *s1, STUD *s2){
    assert(s1 != NULL && s2 != NULL);
    return (s1 -> age) - (s2 -> age);
}
/*
* Big-O: O(1)
*
* This function returns the index where a student is/should be placed in the array of lists.
*/
static int hash(int age){
    return (age % MIN_AGE);
}
/*
* Big-O: O(1)
*
* This function creates a student and returns a pointer to said student.
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
* This function creates a set and returns the set pointer. It pre-fills each element in the data array with a list to prevent any NULL eror issues.
* If it did not pre-fill each index of the data array with a linked list, it would be O(1) but it would create many special cases in other methods. 
*/
SET *createDataSet(){
    SET *sp;
    sp = malloc(sizeof(SET));
    assert(sp != NULL);
    sp -> length = SET_SIZE;
    sp -> data = malloc(sizeof(LIST) * sp -> length);
    for(int i = 0; i < sp -> length; i++){
        sp -> data[i] = createList((*compare));
    }
    sp -> count = 0;
    sp -> min = 0;
    sp -> max = 0;
    return sp;   
}  
/*
* Big-O: O(n^2)
*
* In this function, we go through each element of the array and call the destroyList method for each list in the data array.
* Since destroyList is an O(n) method, the Big-O of the function is O(n^2) as it is nested in another loop.
* Since we allocated memory to each element in the list (they are a student created with the create Student function),
* we must remove each element in the list before we destroy it to prevent a memory leak. This also causes it to be an O(n^2) function.
* After it destroys every list in data, the data array is then freed and the set pointer is freed.
*/
void destroyDataSet(SET *sp){
    assert(sp != NULL);
    for(int i = 0; i < sp -> length; i++){
        while(numItems(sp -> data[i]) > 0){
            free(removeLast(sp -> data[i]));
        }
        destroyList(sp -> data[i]);
    }
    free(sp -> data);
    free(sp);
}
/*
* Big-O: O(1)
*
* This function exists to display the # of students in the dataset for testing purposes.
*/
int numElements(SET *sp){    
    assert(sp != NULL);
    return sp -> count;
}
/*
* Big-O: O(1) 
*
* This function adds a student to the dataset. It first has to check if the age range is valid. 
* After doing so, the element is added to its respective list in the list array (based on its age).
* Chosing which list to place an element in based on age allows for quick retrieval of every student who is of the same age.
* After that, a min and max variable are checked if they need to be updated.
* If so, the updates take place.
*/
void insertStud(SET *sp, STUD *elt){
    assert(sp != NULL && elt != NULL);
    int location = hash(elt -> age);

    if(elt -> age > MAX_AGE || elt -> age < MIN_AGE){printf("Student is outside of the valid age range."); return;}

    addLast(sp -> data[location], elt);
    printf("Successfully added Student of ID: %d and Age: %d\n", elt -> id, elt -> age);
    sp -> count++;
    if(sp -> max != sp -> min){
        if(location < sp -> min){
            sp -> min = location;
        }
        else if(location > sp -> max){
            sp -> max = location;
        }
    }
    else{
        if(sp -> count <= 1){
            sp -> max = location;
            sp -> min = location;
        }
        if(location < sp -> min){
                sp -> min = location;
            }
        if(location > sp -> max){
            sp -> max = location;
        }
    }
}
/*
* Big-O: O(n) 
*
* This function removes all students of a given age. 
* First, it gets the index of where all students of an age should be.
* Once it figures that out, it then checks to see if there are event students of said age in that list. 
* If there are, it then sets the size parameter called in (for use in college.c) to the size of the list that is going to be nuked. 
* It also calls getItems to store all the students in an array to be returned. 
* After doing so, each element is removed from the list and count is decremented each time.
* After an element is removed, the function then checks if the elements removed were the max or min value. 
* If so, it then searches for the next element closest to the removed value to be chosen as the max/min.
*/
STUD **removeStud(SET *sp, int age, int *size){
    assert(sp != NULL);
    int location = hash(age);
    if(age >= MIN_AGE && age <= MAX_AGE){
        assert(sp -> data[location] != NULL);
        if(numItems(sp -> data[location]) == 0){printf("There is no student with that age in the database.\n"); return NULL;}
        *size = numItems(sp -> data[location]);
        STUD **arr = getItems(sp -> data[location]);
        while(numItems(sp -> data[location]) > 0){
            removeLast(sp -> data[location]);
            sp -> count--;
        }
        if(sp -> count != 0){
            if(location == sp -> min){
                location++;
                while(numItems(sp -> data[location]) == 0 && location < sp -> length){
                    location++;
                }
                sp -> min = location;
            }
            else if(location == sp -> max){
                location--;
                while(numItems(sp -> data[location]) == 0 && location >= 0){
                    location--;
                }
                sp -> max = location;
            }      
        }
        printf("Succesfully deleted students who are %d years old.\n", age);
        return arr;
    }
    else{
        printf("That is not a valid age to input into the student database.\n");
        return NULL;
    }
}
/*
* Big-O: O(n)
*
* This function gets every student of a certain age and returns them all in an array. 
* Since this function calls getItems, it makes the runtime O(n).
*/
STUD **searchAge(SET *sp, int age, int* size){
    assert(sp != NULL);
    int index = hash(age);
    if(index > SET_SIZE || age < 18 || age > 30){
       printf("Student age is out of bounds.\n");
       return NULL;
    }
    assert(sp -> data[index] != NULL);
    if(numItems(sp -> data[index]) == 0){
        printf("No Students found with this age.\n");
        return NULL;
    }
    *size = numItems(sp -> data[index]);
    STUD **arr = getItems(sp -> data[index]);
    return arr;
}
/*
* Big-O: O(1)
*
* This function returns the max age gap in the student dataset.
*/
int maxAgeGap(SET *sp){
    return sp -> max - sp -> min;
}