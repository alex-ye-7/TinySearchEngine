/*
 * indexer.c
 * 
 * Purpose: reads the document files produced by the TSE crawler, builds an index, 
 * and writes that index to a file
 *
 * Alexander Ye, Oct 2022
 * Lab 5, CS50 Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "counters.h"
#include "hashtable.h"
#include "index.h"
#include "mem.h"
#include "word.h"
#include "webpage.h"

/* local functions */
static bool fileExist(const char* filename);
static bool fileWritable(const char* filename);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID);

/********* main *********
* 
*/
int main(const int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "indexer needs two arguments: pageDirectory and indexFilename\n");
        exit(1);
    }

    // validating parameters
    if (pagedir_validate(argv[1]) == false) {
        fprintf(stderr, "%s is not a crawler directory\n", argv[1]);
        exit(2);
    }

    // index file exists but not writable 
    if (fileExist(argv[2]) && fileWritable(argv[2]) == false) {
        fprintf(stderr, "Index file %s is not writable\n", argv[2]);
        exit(3);
    }

    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];

    // build the index and save index to file
    index_t* index = indexBuild(pageDirectory);
    index_save(index, indexFilename);

    index_delete(index);
    return 0;
}

/********* indexBuild *********
* Loop over all webpages stored by crawler in pageDirectory, 
* index those pages by looking for each word on each page, 
* make entry in index (hashtable) for each word.
*/
static index_t* indexBuild(char* pageDirectory)
{
    // create new index
    index_t* index = index_new(200);
    int docID = 1; 
    webpage_t* page;
    
    // load a webpage 
    while ((page = pagedir_load(pageDirectory, docID)) != NULL) {
        indexPage(index, page, docID); //make entry for each word in this page

        webpage_delete(page);
        docID++;
    }
    return index;
}

/********* indexPage *********
* Find words on a single page, create a counter for each word, 
* make entry into index (hashtable)
*/
static void indexPage(index_t* index, webpage_t* page, const int docID)
{
    char* next_word;
    int pos = 0;

    // get next word
    while ((next_word = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(next_word) > 2) {
            normalizeWord(next_word);

            // get counters for this word from index
            counters_t* ctrs = index_find(index, next_word);

            // if word not in index, make new counters
            if (ctrs == NULL) {
                ctrs = counters_new();
                index_insert(index, next_word, ctrs);
            }
            counters_add(ctrs, docID);
        }
        free(next_word);
    }
}

/********* fileExist *********
* Check to see if file is a valid pathname
*/
static bool fileExist(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	if (fp == NULL)
		return false;
	fclose(fp);
	return true;
}

/********* fileWritable *********
* Check to see if file is writable
*/
static bool fileWritable(const char* filename)
{
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
		return false;
	fclose(fp);
	return true;
}
