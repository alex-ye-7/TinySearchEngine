/*
 * querier.c
 * 
 * Purpose: the TSE Querier reads the index file produced by the TSE Indexer, 
 * and page files produced by the TSE Crawler, 
 * and answers search queries submitted via stdin.
 *
 * Alexander Ye, Oct 2022
 * Lab 6, CS50 Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> 
#include "pagedir.h"
#include "counters.h"
#include "hashtable.h"
#include "index.h"
#include "mem.h"
#include "word.h"
#include "webpage.h"
#include "file.h"

/******** local data types *******/
 struct twocts { 
	counters_t *result;
	counters_t *another; } ; //store result in first counter

/******** local function prototypes *******/

/* The fileno() function is provided by stdio,
 * but for some reason not declared by stdio.h, so declare here.
 */
int fileno(FILE *stream);

static void prompt(void);
static bool fileExist(const char* filename);
static void processAll(index_t* index, const char* pageDirectory, char* line);
static counters_t* processQuery(index_t* index, char** words, const int num_words);

// parsing and checking the query
static char** tokenizeString(char* line, int* size);
static bool checkChar(const char* line);
static bool checkWords(char** words, int size);

// functions for intersecting and combining counters
static void INTERSECT(counters_t* orSequence, counters_t* andSequence);
static void intersect_helper(void* arg, const int key, const int count);
static void UNION(counters_t* orSequence, counters_t* andSequence);
static void union_helper(void* arg, const int key, const int count);

// functions for displaying 
static void itemcount(void *arg, const int key, const int count);
static int countersCount(counters_t* ctrs);
static void rank(void* arg, const int key, const int count);
char* getURL(const char* pageDirectory, int docID);
static void displayCleanQuery(char** words, int size);
static void displayResult(const char* pageDirectory, counters_t* result);

/********* main function**********/
int main(const int argc, char* argv[])
{
    // number of arguments check
    if (argc != 3) {
        fprintf(stderr, "querier needs two arguments: pageDirectory and indexFilename\n");
        exit(1);
    }

    // validating pageDirectory
    if (pagedir_validate(argv[1]) == false) {
        fprintf(stderr, "%s is not a crawler directory\n", argv[1]);
        exit(2);
    }
    char *pageDirectory = argv[1];

    // read a file named pageDirectory/1
    char find_one[512];
    sprintf(find_one, "%s/1", pageDirectory);
    if (fileExist(find_one) == false) {
        fprintf(stderr, "File named %s cannot be read\n", find_one);
        exit(3);
    }

    // make sure index file can be read
    if (fileExist(argv[2]) == false) {
        fprintf(stderr, "Index file %s cannot be read \n", argv[2]);
        exit(4);
    }
    char *indexFilename = argv[2];

    // load index from file and null check
    index_t* index = index_load(indexFilename);
    mem_assert(index, "Cannot load index file\n");

    // continually collects query
    prompt();
    char* line;
    while ((line = file_readLine(stdin)) != NULL) {
        processAll(index, pageDirectory, line);
        free(line);
        prompt();
    }

    index_delete(index);

    return 0;
}

/********* prompt *********
* Prompts user for a query
*/
static void prompt(void)
{
  // print a prompt if stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
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

/********* processAll *********
* Encapsulates all the functions of normalizing and checking the query
*/
static void processAll(index_t* index, const char* pageDirectory, char* line)
{
    normalizeWord(line);

    if (checkChar(line) == false) {
        return;
    }

    int size=0;
    char** words = tokenizeString(line, &size);

    displayCleanQuery(words, size);

    if (checkWords(words, size) == false) {
        free(words);
        return;
    }

    counters_t* result = processQuery(index, words, size);
    displayResult(pageDirectory, result);

    if (result) {
        counters_delete(result);
    }
    
    free(words);
}


/********* tokenizeString *********
* Parses the query line into an array of words and counts the size
*/
static char** tokenizeString(char* line, int* size)
{
    char** words = NULL;
    
    // trim will elimating leading and trailing spaces
    char* word = trim(line);
    char* rest;
    int n = 0; 

    // strchr searches for the first occurence of a space in the string
    while ((rest = strchr(word, ' ')) != NULL) {
        *rest++ = '\0';
        words = realloc(words, ++n*sizeof(char*));
        words[n-1] = word;
        word = trim(rest);
    }
    // add the final word to words array
    words = realloc(words, ++n*sizeof(char*));
    words[n-1] = word;

    *size = n;
    return words;
}


/********* checkChar *********
* Verify the string contains only letters and spaces
*/
static bool checkChar(const char* line)
{
    for (int i=0; i < strlen(line); i++) {
        // if it a special character or non-letter
        if (!isalpha(line[i]) && !isspace(line[i])) {
            fprintf(stderr, "Error: Bad character '%c' in query.\n", line[i]);
            return false;
        }
    }
    return true;
}

/********* checkWords *********
* Makes sure the syntax of the query is correct
* ex: "and" and "or" can't be the first word of the query
*/
static bool checkWords(char** words, int size)
{
    if (size <= 0) { return false; }

    // "or" and "and" cannot be the first word of query
    if (strcmp(words[0], "or") == 0) {
        fprintf(stderr, "Error: 'or' cannot be first\n");
        return false;
    }
    if (strcmp(words[0], "and") == 0) {
        fprintf(stderr, "Error: 'and' cannot be first\n");
        return false;
    }

    // "or" and "and" cannot be the last word of query
    if (strcmp(words[size-1], "or") == 0) {
        fprintf(stderr, "Error: 'or' cannot be last\n");
        return false;
    }
    if (strcmp(words[size-1], "and") == 0) {
        fprintf(stderr, "Error: 'and' cannot be last\n");
        return false;
    }

    // operators can't be adjacent
    for (int i=0; i < size; i++) {
        if (strcmp(words[i], "and") == 0 && strcmp(words[i+1], "or") == 0) {
            fprintf(stderr, "Error: 'and' and 'or' cannot be adjacent\n");
            return false;
        } 
        if (strcmp(words[i], "and") == 0 && strcmp(words[i+1], "and") == 0) {
            fprintf(stderr, "Error: 'and' and 'and' cannot be adjacent\n");
            return false;
        } 
        if (strcmp(words[i], "or") == 0 && strcmp(words[i+1], "and") == 0) {
            fprintf(stderr, "Error: 'or' and 'and' cannot be adjacent\n");
            return false;
        } 
    }
    return true;
}


/********* processQuery *********
* Accumulates the combined counters of the words of the query 
* The "and" operator takes precedence over the "or" operator,
*/
static counters_t* processQuery(index_t* index, char** words, const int num_words)
{
    counters_t* andSequence = NULL;   // holds result of AND operations
    counters_t* orSequence = counters_new();    // accumulates the final result combining OR sequences
    bool shortCircut= false;          // stop early if AND sequence will fail

    for (int i=0; i < num_words; i++) {
        // "OR"
        if (strcmp(words[i], "or") == 0) {
            UNION(orSequence, andSequence);
            counters_delete(andSequence);
            andSequence = NULL;
            shortCircut = false;
            continue;
        } 
        if (shortCircut) {
            continue;
        }
        // "AND"
        if (strcmp(words[i], "and") == 0) {
            continue;
        }
        // it's a real word
        else {
            counters_t* match = index_find(index, words[i]);
            // no pages contain this query word
            if (match == NULL) {
                shortCircut = true; // AND will fail for next word, so skip it and others until see OR
                if (andSequence != NULL) {
                    counters_delete(andSequence);
                    andSequence = NULL;
                }
            }
            // found counters for this word in index
            else {
                if (andSequence == NULL) {  // this is the first word of AND sequence
                    andSequence = counters_new();
                    UNION(andSequence, match);
                }
                else {
                    INTERSECT(andSequence, match);
                }
            }
        }
    }
    UNION(orSequence, andSequence);
    if (andSequence) {
        counters_delete(andSequence);
        andSequence = NULL;
    }
    return orSequence;
}

/********* INTERSECT *********
* Used for the "and" operator
* Finds the common docID between two counters, and sets the docID with the smaller occurence frequency
* Uses the helper function below it
*/
static void INTERSECT(counters_t* orSequence, counters_t* andSequence)
{
 	struct twocts args = { orSequence, andSequence };
	counters_iterate(orSequence, &args, intersect_helper);
}

static void intersect_helper(void* arg, const int key, const int count)
{
	struct twocts *args = arg;
    int min_count = counters_get(args->another, key);
    if (count < min_count) {
        min_count = count;
    }
    counters_set(args->result, key, min_count);
}

/********* UNION *********
* Used for the "or" operator
* Finds the common docID between two counters, and sets the docID with the combined occurence frequency
* Uses the helper function below it
*/
static void UNION(counters_t* orSequence, counters_t* andSequence)
{
    counters_iterate(andSequence, orSequence, union_helper);
}

static void union_helper(void* arg, const int key, const int count)
{
    counters_t* ctrs = (counters_t *) arg;
    int wordCount = counters_get(ctrs, key);
    counters_set(ctrs, key, count + wordCount);
}


/********* displayCleanQuery *********
* Prints the clean query
*/
static void displayCleanQuery(char** words, int size)
{
    fprintf(stdout, "Query: ");

    for (int i=0; i < size; i++) {
        fprintf(stdout, "%s ", words[i]);
    }
    fprintf(stdout, "\n");
}

/********* displayResult *********
* Prints the document set in decreasing order by score (frequency) along with its important information
*/
static void displayResult(const char* pageDirectory, counters_t* result) {
    int n = countersCount(result); // counters number of items in the counter

    if (n == 0) {
        fprintf(stdout, "No documents match.\n");
        fprintf(stdout, "-----------------------------------------------\n");
        return;
    }

    fprintf(stdout, "Matches %d documents (ranked):\n", n);
    for (int i=0; i < n; i++) {
        int entry[2] = {0, 0}; // array that will hold the docID and count of the current highest score

        counters_iterate(result, &entry, rank);

        int docID = entry[0];
        int score = entry[1];

        char* url = getURL(pageDirectory, docID);
        mem_assert(url, "Can't read URL from docID\n");

        fprintf(stdout, "score %4d doc %4d: %s\n", score, docID, url);
        counters_set(result, docID, 0);
        free(url);
    }
}

/********* itemcount *********
* Counts number of non-NULL, nonzero items
*/
static void itemcount(void *arg, const int key, const int count) {
    int* n = arg;
    if (n != NULL && key > 0 && count > 0) {
        (*n)++;
    }
}

/********* countersCount *********
* Counts number of counters (documents in this case) in the result
*/
static int countersCount(counters_t* ctrs) {
    int count = 0;
    counters_iterate(ctrs, &count, itemcount);
    return count;
}

/********* rank *********
* Check a counternode and sees if it is the greatest occurence so far
*/
static void rank(void* arg, const int key, const int count) {
    int *p = arg;
    if (count > p[1]) {
        p[1] = count;
        p[0] = key;
    }
}

/********* getURL *********
* Obtains the URL by reading the first line of the relevant document file from the pageDirectory
*/
char* getURL(const char* pageDirectory, int docID)
{
    char filename[512];
    sprintf(filename, "%s/%d", pageDirectory, docID);

    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Couldn't retrieve URL\n");
        return NULL;
    }

    char* url = file_readLine(fp); // URL is in the first line
    fclose(fp);

    return url;
}