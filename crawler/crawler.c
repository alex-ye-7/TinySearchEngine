/*
 * crawler.c
 * 
 * Purpose: to crawl the web and retrieve webpages starting with a specified URL
 *
 * Alexander Ye, Oct 2022
 * Lab 4, CS50 Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "bag.h"
#include "hashtable.h"
#include "mem.h"
#include "webpage.h"

/********* logr *********
* Prints out some logging code
*/
static void logr(const char *word, const int depth, const char *url)
{
    printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

/********* itemcount *********
* Counts the non-null items in the bag. Taken from the bag unit test code provided in Lab 3
*/
void itemcount(void* arg, void* item)
{
  int* nitems = arg;

  if (nitems != NULL && item != NULL)
    (*nitems)++;
}

/********* bagcount *********
* Utilizes the itemcount function to iterate through bag and count the number of non-null items
*/
int bagcount(bag_t* bag) 
{
    int bagcount = 0;
    bag_iterate(bag, &bagcount, itemcount);
    return bagcount;
}

/********* praseArgs *********
* The parseArgs function will extract arguments from the command line
* into the function parameters. 
*/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)
{   
    // normalize the URL and validate it is an internal URL
    *seedURL = normalizeURL(argv[1]);

    if (isInternalURL(*seedURL) == false) {
        fprintf(stderr, "%s is not an internal URL\n", argv[1]);

        if (*seedURL != NULL) 
            free(*seedURL); // normalizeURL returns a copied string...have to free
        mem_free(seedURL);
        mem_free(pageDirectory);
        exit(2);
    }
    
    // call pagedir_init on the pageDirectory input 
    if (pagedir_init(argv[2]) == false) {
        fprintf(stderr, "Can't init pageDirectory\n");

        if (*seedURL != NULL) 
            free(*seedURL); // normalizeURL returns a copied string...have to free
        mem_free(seedURL);
        mem_free(pageDirectory);
        exit(3);
    }
    *pageDirectory = argv[2];

    // ensure max depth is between 0 and 10
    int input_depth = atoi(argv[3]);
    if (input_depth < 0 || input_depth > 10) {
        fprintf(stderr, "maxDepth should be between 0 and 10\n");

        if (*seedURL != NULL) 
            free(*seedURL); // normalizeURL returns a copied string...have to free
        mem_free(seedURL);
        mem_free(pageDirectory);
        exit(4);
    }
    *maxDepth = input_depth;
}

/********* pageScan *********
* pageScan extracts URLs from a given page and processes each one, adding them to our hashtable
* and bag. It ignores non-internal URLs and duplicates.
*/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    int pos = 0;
    char* url;
    int current_depth = webpage_getDepth(page);

    logr("Scanning", webpage_getDepth(page), webpage_getURL(page));

    // while another URL on the page
    while ((url = webpage_getNextURL(page, &pos)) != NULL) {
        logr("Found", webpage_getDepth(page), url);

        // check if URL is Internal
        if (isInternalURL(url) == false) {
            logr("IgnExtern", webpage_getDepth(page), url);
            free(url);  // not using mem_free because getNextURL function is not using mem_malloc
            continue;
        }

        // insert the webpage into the hashtable and check if success or not
        bool result1 = hashtable_insert(pagesSeen, url, "");
        if (result1 == false) {
            logr("IgnDupl", webpage_getDepth(page), url);
            free(url);  // not using mem_free because getNextURL function is not using mem_malloc
            continue;
        } 

        // create a webpage_t and insert the webpage into the bag
        webpage_t* new_page = webpage_new(url, current_depth+1, NULL);
        mem_assert(new_page, "Failed to create new page\n");
        
        bag_insert(pagesToCrawl, new_page);
        logr("Added", webpage_getDepth(page), webpage_getURL(new_page));
    }
}

/********* crawl *********
* crawl does the work of crawling from seedURL to maxDepth and saving pages in pageDirectory.
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
    // initialize the hashtable and add the seedURL
    hashtable_t* pagesSeen = hashtable_new(200);
    mem_assert(pagesSeen, "Failed new hashtable\n");

    bool result = hashtable_insert(pagesSeen, seedURL, "");
    if (result == false) {
        fprintf(stderr, "Could not insert seedURL to hashtable\n");
        exit(5);
    }

    // initialize the bag and add a webpage representing the seedURL at depth 0
    bag_t* pagesToCrawl = bag_new();
    mem_assert(pagesToCrawl, "Failed new bag\n");

    webpage_t* new_page = webpage_new(seedURL, 0, NULL);
    mem_assert(new_page, "Failed new page\n");

    bag_insert(pagesToCrawl, new_page);

    int docID = 1;
    // while bag is not empty
    while (bagcount(pagesToCrawl) > 0) {
        // pull a webpage from bag
        webpage_t* page = (webpage_t *)bag_extract(pagesToCrawl);

        // succesfully fetch the HTML for that webpage
        if (webpage_fetch(page)) {
            logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
            pagedir_save(page, pageDirectory, docID);

            // if the webpage is not at maxDepth, call pageScan on that HTML
            if (webpage_getDepth(page) < maxDepth){
                pageScan(page, pagesToCrawl, pagesSeen);
            }
            docID++;
        } 
        webpage_delete(page);
    }

    // delete the hashtable and bag
    hashtable_delete(pagesSeen, NULL);
    bag_delete(pagesToCrawl, webpage_delete);
}

// The main function simply calls parseArgs and crawl, then exits zero.
int main(const int argc, char* argv[])
{
    // number of parameters check
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments. Need ./crawler, a seed url, a page directory, and depth to crawl.\n");
        return 1;
    }

    char** seedURL = mem_malloc_assert(sizeof(char*), "Failed to malloc seedURL");
    char** pageDirectory = mem_malloc_assert(sizeof(char*), "Failed to malloc pageDirectory");
    int maxDepth = 0;

    parseArgs(argc, argv, seedURL, pageDirectory, &maxDepth);
    
    crawl(*seedURL, *pageDirectory, maxDepth);

    mem_free(seedURL);
    mem_free(pageDirectory);
    mem_report(stdout, "Memory report for crawler");
    return 0;
}
