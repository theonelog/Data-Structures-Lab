/*
 * File:        set.h
 *
 * Copyright:	2021, Darren C. Atkinson
 *
 * Description: This file contains the public function and type
 *              declarations for a set abstract data type for generic
 *              pointer types.  A set is an unordered collection of
 *              distinct elements.
 */

# ifndef SET_H
# define SET_H

typedef struct set SET;
typedef struct student STUD;

STUD *createStud(int id, int age);
SET *createDataSet();
void destroyDataSet(SET *sp);
int numElements(SET *sp);
void insertStud(SET *sp, STUD *elt);
STUD *removeStud(SET *sp, int id);
STUD *searchID(SET *sp, int id);

# endif /* SET_H */
