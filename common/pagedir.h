/* 
 * pagedir.h - header file for TSE pagesaver module
 *
 * Alexander Ye, Oct 2022
 * CS 50, Fall 2022
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <webpage.h>

/********* pagedir_load *********
 * This function loads a webpage from a file in that directory.
*/
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

/********* pagedir_validate *********
 * This function verifies whether a passed in pageDirectory 
 * is indeed a Crawler-produced directory.
*/
bool pagedir_validate(const char* pageDirectory);

/********* pagedir_init *********
 * This function returns true or false based on if a page was written to the pageDirectory
 * and marked as a Crawler-produced pageDirectory. 
*/
bool pagedir_init(const char* pageDirectory);

/********* pagedir_save *********
 * This function writes a page to the pageDirectory with a given webpage's URL, depth away from 
 * the seedURL, and the html within the page.
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H