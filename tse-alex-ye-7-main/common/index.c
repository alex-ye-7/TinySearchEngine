/*
* index.c 
*
* Purpose: creates the index data structure and the logic for saving and loading index files
*
* Alexander Ye, Fall 2022
* Lab 5, CS 50, Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "counters.h"
#include "hashtable.h"
#include "mem.h"
#include "file.h"

typedef hashtable_t index_t;

/********* ct_delete *********
* Helper function for deleting counters
*/
static void ct_delete(void *item)
{
	counters_t* ct = item;

	if (ct != NULL) {
		counters_delete(ct);
	}
}

/********* printCounter *********
* Helper function to print the counter set (docID and occurences of word)
*/
static void printCounter(void* arg, const int key, int item)
{
	fprintf(arg, "%d %d ", key, item);
}

/********* printKey *********
* Helper function to print the keys (words in index)
*/
static void printKey(void* arg, const char* key, void* item)
{
	fprintf(arg, "%s ", key);
	counters_iterate(item, arg, printCounter);
	fprintf(arg, "\n");
}

/********* index_new *********
* Makes new index data type, a wrapper over hashtable_t
*/
index_t *index_new(int number_slots)
{
    return (index_t *) hashtable_new(number_slots);
}

/********* index_insert *********
* Inserts a word into the hashtable
*/
bool index_insert(index_t *index, const char *key, counters_t *ctrs)
{
    return hashtable_insert(index, key, ctrs);
}

/********* index_find *********
* Finds the counters set assosiated with a word in the hashtable
*/
counters_t* index_find(index_t* index, const char* key)
{
    return (counters_t *) hashtable_find(index, key);
}

/********* index_save *********
* Saves an index to the index filename passed in by the caller
*/
void index_save(index_t *index, const char *indexFilename) 
{
    if (index == NULL || indexFilename == NULL) {
        return;
    }

    FILE *fp = fopen(indexFilename, "w");
    if (fp == NULL) {
        fprintf(stderr, "failed to open %s", indexFilename);
        return;
    }
    
    hashtable_iterate(index, fp, printKey);
    fclose(fp);
}

/********* index_load *********
* Loads an index from a passed in index filename 
* * This code was taken from Professor Pierson
*/

index_t* index_load(const char* indexFilename)
{
  if (indexFilename == NULL) {
    fprintf(stderr, "index: indexFilename NULL\n");
    return NULL;
  }

  FILE* fp = fopen(indexFilename, "r");
  if (fp == NULL) {
    fprintf(stderr, "index: cannot open index file %s\n", indexFilename);
    return NULL;
  }

  // how many words in the index?
  int wordcount = file_numLines(fp);
  // check if it’s a empty file
  if (wordcount == 0) {
    fprintf(stderr, "index: cannot read lines from %s\n", indexFilename);
    return NULL;
  }

  // create an empty index with ~2words/slot; index_new never returns NULL
  index_t* index = index_new(wordcount/2+1);

  // read words from file
  char* word;
  while ( (word = file_readWord(fp)) != NULL ) {
    // found a word, now build a counters list for it
    counters_t* ctrs = mem_assert(counters_new(), "new counters");
    // now read (docId count) pairs into the counters list
    int docID, count;
    while (fscanf(fp, "%d %d ", &docID, &count) == 2) {
      if (!counters_set(ctrs, docID, count)) {
        fprintf(stderr, "index: unexpected, counters_set returned false\n");
      }
    }
    if (!hashtable_insert(index, word, ctrs)) {
      fprintf(stderr, "index: unexpected, hashtable_insert returned false\n");
    }
    free(word);
  }
  fclose(fp);
  return index;
}

/********* index_delete *********
* Deletes index
*/
void index_delete(index_t *index) 
{
    hashtable_delete(index, ct_delete);
}