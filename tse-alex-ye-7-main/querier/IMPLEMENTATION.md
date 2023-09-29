# CS50 TSE Querier
## Implementation Specification

# Implmentation Comments

Some important notes about my implementation is that I added a trim function to the word.h and word.c in the common directory. This trim function is responsible for removing the leading and trailing spaces in a given string. 

This was a useful function when it came to tokenizing the query. After trimming the leading and trailing spaces, I used strchr repeatedly to search for the first non-leading space to split the first word off the query. Then I reallocated the size of the array for that new word, and continued this whole process for the entire query. 

Another notable implementation comment is that the in-class activity recommended a MERGE function to push the temporary result andSequence to final result orSequence. I was having trouble getting this function to work for some reason and decided to just have some repeatative code (only twice).

# Testing Plan

1. Test `querier` with various invalid arguments.
    1. no arguments
    2. one argument
    3. three or more arguments
    4. invalid `pageDirectory` (non-existent path)
    5. invalid `indexFile` (non-existent path)
2. Test `querier` with various invalid queries
    1. a bad character
    2. a query that starts with an operator
    3. a query with two consecutive operators 
3. Some successful queries 
    1. capital letters in query
    2. multiple spaces in between words of query
4. Use *fuzzquery* to generate random queries to pipe into the *querier*
5. Run *valgrind* on the *querier* to ensure no memory leaks or errors.
