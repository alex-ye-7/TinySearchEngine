#!/bin/bash 
#
# TESTING.sh: test script to test the TSE querier
#
# Input: call "make test" or "./testing.sh", I have set up the test cases
# Output: the result of many erroneous and correct test cases and their commmands
#
# Alexander Ye, Fall 2022
# CS 50, Fall 2022

# TEST 1: No arguments
./querier 

# TEST 2: One argument
./querier ../data/letters-10

# TEST 3: Three or more arguments
./querier ../data/letters-10 ../data/letters-10/index random

# TEST 4: invalid pageDirectory (non-existent path)
./querier ../data/badpageDirectory ../data/letters-2/index

# TEST 6: invalid indexFile (non-existent path)
./querier ../data/letters-10 ../data/indexer_random/doesntexist

# TEST 7: bad character
echo git-flow | ./querier ../data/toscrape-1 ../data/indexer_outputs/index-toscrape-1

# TEST 8: capital letters with many spaces
echo      Tiny      Search    Engine    | ./querier ../data/toscrape-1 ../data/indexer_outputs/index-toscrape-1

# TEST 9: syntax error
echo and earth | ./querier ../data/toscrape-1 ../data/indexer_outputs/index-toscrape-1

# TEST 10: syntax error
echo travel and or Europe | ./querier ../data/toscrape-1 ../data/indexer_outputs/index-toscrape-1

# TEST 11: test that works
echo travel or Europe | ./querier ../data/toscrape-1 ../data/indexer_outputs/index-toscrape-1

# TEST 12: fuzzquery
./fuzzquery ../data/indexer_outputs/index-toscrape-1 10 0 | ./querier ../data/toscrape-1 ../data/indexer_outputs/index-toscrape-1

# TEST 13: valgrind
valgrind --leak-check=full --show-leak-kinds=all ./fuzzquery ../data/indexer_outputs/index-toscrape-1 5 10 | ./querier ../data/toscrape-1 ../data/indexer_outputs/index-toscrape-1