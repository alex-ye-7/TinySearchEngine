/*
 * word.c 
 * Purpose: file that stores functions for word operations
 * 
 * Alexander Ye, Oct 2022
 * Lab 5, CS50 Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "word.h"

/********* normalizeWord *********
* This method is used to normalize words, making them lowercase
*/
void normalizeWord(char* word)
{
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
}

/********* trim *********
* Eliminates the leading spaces and trailing spaces of a string
*/
char* trim(char* s)
{
    while (isspace(*s))
        s++;

    char* end = s + (strlen(s)) - 1;

    while (end > s && isspace(*end))
        end--;

    end++;
    *end = '\0';
    return s;
}