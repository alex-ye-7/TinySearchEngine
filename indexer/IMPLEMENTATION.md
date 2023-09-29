# CS50 TSE Indexer
## Implementation Specification

In this document I want to focus on implementation-specific decisions for the indexer.

## Data structures 

At the indexer's core, we use two data structures: a hashtable of keys from the HTML pages, which corresponds to a counters set containing how many times that word appears in each docID. Both of these data structures are used to create the index data structure. Index is essentially a wrapper over hashtable.

The size of the hashtable is impossible to determine in advance, so we use 200. (NOTE: our hashtable implementation does not grow in size as hashtables did in CS 10 to keep a low load factor)

## Control flow

The Indexer is implemented in one file `indexer.c`, with three functions.

### 1. main

The `main` function parses arguments and initializes other modules, then exits zero.

### 2. indexBuild

Builds an in-memory index from webpage files it finds in the pageDirectory.

### 3. indexPage

Scans a webpage document to add its words to the index.

## General notes

We use external files such as index and pagedir in **Common** to run the program.

# Assumptions

The indexer may assume that pageDirectory has files named 1, 2, 3, …, without gaps. The indiexer may also assume the content of files in pageDirectory follow the format as defined in the specs.

The index tester may assume that the content of the index file follows the format specified below; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.
