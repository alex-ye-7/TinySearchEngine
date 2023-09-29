/*
 * word.h - header file for word.c
 * 
 * Alexander Ye, Oct 2022
 * Lab 5, CS50 Fall 2022
 */
 
#ifndef __WORD_H
#define __WORD_H

#include <stdlib.h>

/********* normalizeWord *********
* This method is used to normalize words, making them lowercase
*/
void normalizeWord(char* word);

/********* trim *********
* Eliminates the leading spaces and trailing spaces of a string
*/
char* trim(char* s);

#endif // __WORD_H