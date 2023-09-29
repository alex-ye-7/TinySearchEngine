# CS50 TSE Querier
## Design Specification

# Abstarct Data Types

The abstract data types used in this lab were the *index* and the *counters*. 

The key data structure is the *index*, mapping from *word* to *(docID, #occurrences)* pairs.
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID. 

A small note I would like to make is that I used Professor Pierson's index_load function that was provided in the Slack as mine from the Indexer had some problems. 

# Some psuedocode

The querier will run as follows:

    check for the correct number of arguments and validate parameters
    load index from the index file
    prompt the user for query
    while there is a query that can be read
        call processAll
        free the line
        prompt the user for input again

where *processAll:*

    normalizes the query
    checks for special characters
    tokenizes the string to produce an array of words
    displays the clean query
    checks query syntax
    calls processQuery that calculates a result counter
    display result
    free the words array

where *processQuery:* 

    creates two counters, orSequence for final result and andSequence to accumulate results of AND operations
    shortCircuit = false     // stop early if AND sequence will fail
    loop over all words in query
      if OR
         combine andSequence and orSequence
         delete and null andSequence
         shortCircuit reset
         continue
      if shortCircuit
         countinue
      if AND
         continue to next word
      else 
         get the match counters for this word
         if match is null
            shortCircuit == true
            if andSequence is null
               delete andSequence and set to null
         else
            if andSequence is null
               make a new andSequence
               combine andSequence and match
            else
               intersect andSequence and match

    once loop is done combine the andSequence and orSequence
    delete and null andSequence if it exists
    return orSequence
    