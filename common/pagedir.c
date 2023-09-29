/* 
 * pagedir.c
 *
 * Purpose: a collection of page directory functions for the TSE
 * 
 * Alexander Ye, Oct 2022
 * CS 50, Fall 2022
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <webpage.h>
#include <string.h>
#include "mem.h"
#include "file.h"

/********* pagedir_load *********
 * This function loads a webpage from a file in that directory.
*/
webpage_t* pagedir_load(const char* pageDirectory, const int docID)
{
    char* fileName = mem_malloc_assert(strlen(pageDirectory) + sizeof(int), "Error allocating filename");
    sprintf(fileName, "%s/%d", pageDirectory, docID);

    FILE* fp = fopen(fileName, "r");

    if (fp == NULL) {
        mem_free(fileName); 
        return NULL;
    }

    char* url = file_readLine(fp);
	char* strDepth = file_readLine(fp);
    char* html = file_readFile(fp);

    int depth = atoi(strDepth);
    free(strDepth);
    mem_free(fileName);

    fclose(fp);

    return webpage_new(url, depth, html);
}

/********* pagedir_validate *********
 * This function verifies whether a passed in pageDirectory 
 * is indeed a Crawler-produced directory.
*/
bool pagedir_validate(const char* pageDirectory)
{
    char filename[512];
    sprintf(filename, "%s/.crawler", pageDirectory);

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return false;
    }

    fclose(fp);
    return true;
}

/********* pagedir_init *********
 * This function returns true or false based on if a page was written to the pageDirectory
 * and marked as a Crawler-produced pageDirectory. 
*/
bool pagedir_init(const char* pageDirectory)
{
    char pathName[256];
    sprintf(pathName, "%s/.crawler", pageDirectory);
    
    FILE* fp = fopen(pathName, "w");

    // if file doesn't open
    if (fp == NULL) 
        return false;

    fclose(fp);
    return true;
}

/********* pagedir_save *********
 * This function writes a page to the pageDirectory with a given webpage's URL, depth away from 
 * the seedURL, and the html within the page.
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    char filename[512];
    sprintf(filename, "%s/%d", pageDirectory, docID);

    FILE* fp = fopen(filename, "w");

    // if file doesn't open
    if (fp == NULL) 
        return;

   fprintf(fp, "%s\n", webpage_getURL(page));
   fprintf(fp, "%d\n", webpage_getDepth(page));
   fprintf(fp, "%s\n", webpage_getHTML(page));
   
   fclose(fp);
}
