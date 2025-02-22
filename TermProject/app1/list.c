/*
* File: list.c - doubly linked circular linked list  with a dummy node. 
* Author: theonelog
* Lab Section: CSEN12L-1
* Last Updated: 11/7/2024
* Description: The implementation of the doubly linked circular linked list with a dummy node. Contains all the required
interfacing functions for radix.c, maze.c, and table.c
*/
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include <assert.h>
/*
* This is the struct for a node in the linked list that the list struct would use.
* It contains the data that is stored in the node and a prev and next pointer as this is a doubly linked list.
*/
struct node{
    void *data;
    struct node *next;
    struct node *prev;
}; 
/*
* This is the list struct that defines all the data required for a list to be a list.
*
* It has a head pointer that will always point to the dummy node and also has a compare as this is a generic linked list 
that works with any data type that is comparable.
*/
typedef struct list{
    int count;
    struct node *head;
    int (*compare)();
} LIST;
/*
* Big-O: O(1)
*
* This function creates and returns a linked list pointer
* It first mallocs for the list pointer and then defines all the variables after asserting that it is not null.
* When defining head, we ensure to set the data to null as it is a dummy pointer and also set the next and prev to itself to prevent any NULL
errors which is the purpose of the dummy node.
*/
LIST *createList(int (*compare)()){
    struct list *lp = malloc(sizeof(LIST));
    assert(lp != NULL);
    lp -> count = 0;
    lp -> head = malloc(sizeof(struct node));
    assert(lp -> head != NULL);
    lp -> head -> data = NULL;
    lp -> head -> next = lp -> head;
    lp -> head -> prev = lp -> head;
    lp -> compare = compare;
    return lp;
}
/*
* Big-O: O(n)
*
* This function goes through the entire list and frees each node and then ultimately frees the list pointer.
*/
void destroyList(LIST *lp){
    assert(lp != NULL);
    struct node *tbd = lp -> head -> next;
    while(tbd != lp -> head){
        struct node *del = tbd;
        tbd = del -> next;
        free(del);
    }
    free(lp -> head);
    free(lp);
}
/*
* Big-O: O(1)
*
* This function returns an integer which is the count of the amount of elements in the linked list.
*/
int numItems(LIST *lp){
    assert(lp != NULL);
    return lp -> count;
}
/*
* Big-O: O(1)
*
* This function adds an element to the front of the linked list.
* It allocates memory for the node and then sets all its variables.
* prev will always point to the dummy node and next will point to the element to the right of the dummy node (aka the first real node).
*/
void addFirst(LIST *lp, void *item){
    assert(lp != NULL);
    struct node *np = malloc(sizeof(struct node));
    assert(np != NULL && item != NULL);
    np -> data = item;
    np -> next = lp -> head -> next;
    np -> prev = lp -> head;
    lp -> head -> next -> prev = np;
    lp -> head -> next = np;
    lp -> count++;
}
/*
* Big-O: O(1)
*
* This function adds an element to the end of the linked list.
* It does this even without a tail pointer as this is a circular doubly linked list so we are able to access the last item.
* Once we know what the last item is a create the new node, we connect the new node to the list then we reconnect the other nodes already
in the list that are modified. 
*/
void addLast(LIST *lp, void *item){
    assert(lp != NULL);
    struct node *np = malloc(sizeof(struct node));
    assert(np != NULL && item != NULL);
    np -> data = item;
    np -> next = lp -> head;
    np -> prev = lp -> head -> prev;
    lp -> head -> prev -> next = np;
    lp -> head -> prev = np;
    lp -> count++;
}
/*
* Big-O: O(1)
*
* This function removes the first element in the list and reconnects the other elements to one another.
* The deleted node is then freed as we allocated memory to it when we added node in any of the addNode functions.
*/
void *removeFirst(LIST *lp){
    assert(lp != NULL);
    if(lp -> count <= 0){return NULL;}
    struct node *del = lp -> head -> next;
    assert(del != NULL);
    del -> next -> prev = lp -> head;
    lp -> head -> next = del -> next;
    lp -> count--;
    void *data = del -> data;
    free(del);
    return data;
}
/*
* Big-O: O(1)
*
* This function removes the last element in the list and reconnects the other elments to one another when the node is removed.
* The deleted node is then freed as we allocated memory to it when we added node in any of the addNode functions.
*/
void *removeLast(LIST *lp){
    assert(lp != NULL);
    if(lp -> count <= 0){return NULL;}
    struct node *del = lp -> head -> prev;
    assert(del != NULL);
    del -> prev -> next = lp -> head;
    lp -> head -> prev = del -> prev;
    lp -> count--;
    void *data = del -> data;
    free(del);
    return data;
}
/*
* Big-O: O(1)
*
* This function returns the data in the first element of the list.
*/
void *getFirst(LIST *lp){
    assert(lp != NULL);
    return lp -> head -> next -> data;
}
/*
* Big-O: O(1)
*
* This function returs the last element in the list's data.
*/
void *getLast(LIST *lp){
    assert(lp != NULL);
    return lp -> head -> prev -> data;
}
/*
* Big-O: O(n)
*
* Since we are only given the item (which is the data in a node), we need to search for it.
* If the node is not found (aka when the del node pointer points to the head), you will not do any deletion.
* If the node is found, it then goes through with the deletion process and frees the node to be deleted. 
*/
void removeItem(LIST *lp, void *item){
    assert(lp != NULL);
    struct node *del = lp -> head -> next;
    while(del != lp -> head){
        if(lp -> compare(del -> data, item) == 0){
            break;
        }
        del = del -> next;
    }
    if(del != lp -> head){
        del -> prev -> next = del -> next;
        del -> next -> prev = del -> prev;
        free(del);
    }
    lp -> count--;
}
/*
* Big-O: O(n)
*
* Since we do not know which node the item is, we need to search for it. if it is found, the data in that node is immediately returned.
* If the item is not found, NULL is returned.
*/
void *findItem(LIST *lp, void *item){
    assert(lp != NULL);
    struct node *np = lp -> head -> next;
    while(np != lp -> head){
        if(lp -> compare(np -> data, item) == 0){
            return np -> data;
        }
        np = np -> next;
    }
    return NULL;
}
/*
* Big-O: O(n)
*
* This function gets all the elements in the list and places it in the items array.
* First, we allocate memory to the array as we will be returning it to the user
* Once it fills the items array with all the elements, it then returs the array.
* Since memory is allocated for the array and then it is returned to the user, the user must free the memory manually.
*/
void *getItems(LIST *lp){
    assert(lp != NULL);
    void **items = malloc(sizeof(void *) * lp -> count);
    assert(items != NULL);
    struct node *temp = lp -> head -> next;
    int i = 0;
    while(temp != lp -> head){
        items[i++] = temp -> data;
        temp = temp -> next;
    }
    return items;
}