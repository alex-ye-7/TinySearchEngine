/*
* index.h - header for index.c
*
* Alexander Ye, Fall 2022
* Lab 5, CS 50, Fall 2022
*/

#ifndef __INDEX_H
#define __INDEX_H

#include "counters.h"
#include "hashtable.h"

typedef hashtable_t index_t;

/********* index_new *********
* Makes new index data type, a wrapper over hashtable_t
*/
index_t *index_new(int number_slots);

/********* index_insert *********
* Inserts a word into the hashtable
*/
bool index_insert(index_t *index, const char *key, counters_t *ctrs);

/********* index_find *********
* Finds the counters set assosiated with a word in the hashtable
*/
counters_t* index_find(index_t* index, const char* key);

/********* index_save *********
* Saves an index to the index filename passed in by the caller
*/
void index_save(index_t *index, const char *indexFilename);

/********* index_load *********
* Loads an index from a passed in index filename 
*/
index_t* index_load(const char *indexFilename);

/********* index_delete *********
* Deletes index
*/
void index_delete(index_t *index);

#endif // __INDEX_H