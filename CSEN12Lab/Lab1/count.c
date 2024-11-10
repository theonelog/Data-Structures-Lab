/*
 * File: count.c
 * 
 * Copyright: 2024, Awni AlQuraini
 * 
 * Description: Reads the file and counts the amount of words in a file fed thru the command line.
 */
#include <stdio.h> //Libraries required for the program to function
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 30  //The max length of a word

int main(int argc, char* argv[]){

    FILE *pFile; //the pointer to the file
    char word[MAX_WORD_LENGTH]; //the buffer that holds each word
    int wordCnt = 0; //the counter variable for the # of words

    /*  
    * Checks if there is a second argument
    */

    if(argc != 2){
        fprintf(stderr, "missing file path\n");
        exit(EXIT_FAILURE);
    }

    pFile = fopen(argv[1], "r"); //assigns the pFile pointer to the file directory

    /*
    * Checks if the the file pointer successfully references a file name
    */
    if (pFile == NULL){
        fprintf(stderr, "file path not valid\n");
        exit(EXIT_FAILURE);
    }
    /*
    * Counts the amount of words in the file
    */
    while(fscanf(pFile, "%s", word) == 1){
        wordCnt++;
    }

    fclose(pFile); //closes the file

    printf("Word count: %d \n", wordCnt); //prints out the final word count

    exit(0); //exits the program
}