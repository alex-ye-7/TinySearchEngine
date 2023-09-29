/*
 * indextest.c
 * 
 * Purpose: to test our index data structure
 * by loading index file into an index and then saving index into different file
 *
 * Alexander Ye, Oct 2022
 * Lab 5, CS50 Fall 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "index.h"
#include "mem.h"

int main(const int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Need 2 arguments: oldIndexFilename and newIndexFilename\n");
        exit(1);
    }

    char *oldIndexFilename = argv[1];
    char *newIndexFilename = argv[2];

    index_t* index = index_load(oldIndexFilename);
    mem_assert(index, "Failed to load index from file\n");

    index_save(index, newIndexFilename);

    index_delete(index);

    return 0;
}