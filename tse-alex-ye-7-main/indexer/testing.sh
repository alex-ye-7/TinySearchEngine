#!/bin/bash 
#
# TESTING.sh: test script to test the TSE indexer
#
# Input: call "make test" or "./testing.sh", I have set up the test cases
# Output: the result of many erroneous and correct test cases  and their commmands
#
# Alexander Ye, Fall 2022
# CS 50, Fall 2022

# TEST 1: No arguments
./indexer 

# TEST 2: One argument
./indexer ../data/letters-10

# TEST 3: Three or more arguments
./indexer ../data/letters-10 ../data/letters-10/index random

# TEST 4: invalid pageDirectory (non-existent path)
./indexer ../data/letters-20 ../data/letters-10/index

# TEST 5: invalid pageDirectory (not a crawler directory)
./indexer ../data/indexer_outputs ../data/letters-2/index

# TEST 6: invalid indexFile (non-existent path)
./indexer ../data/letters-10 ../data/indexer_random/doesntexist

# TEST 7: invalid indexFile (read-only directory, test)
./indexer ../data/test ../data/indexer_outputs/shouldnotwork

# TEST 8: invalid indexFile (existing, read-only file)
./indexer ../data/indexer_outputs ../data/indexer_outputs/readonly

# TEST 9: test that works
./indexer ../data/letters-10 ../data/indexer_outputs/index-letters-10

# TEST 10: indextest
./indextest /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-index-0 ../data/indexer_outputs/index-output

# TEST 11: valgrind
valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/toscrape-1 ../data/indexer_outputs/index-toscrape-1