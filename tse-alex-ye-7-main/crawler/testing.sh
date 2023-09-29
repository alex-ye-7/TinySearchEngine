#!/bin/bash 
#
# TESTING.sh: test script to test crawler.c
#
# Input: call "make test" or "./testing.sh", I have set up the test cases
# Output: the result of many erroneous and correct test cases  and their commmands
#
# Alexander Ye, Fall 2022
# CS 50, Fall 2022

# TEST 1: Wrong number of arguments tests
./crawler

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html 

# TEST 2: external link
./crawler https://www.youtube.com/watch?v=dQw4w9WgXcQ ../data/test 2

# TEST 3: depth out of range
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/test 12

#TEST 4: 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 0

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 2

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-10 10

# TEST 5: different seed page
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html ../data/letters-A 2

# TEST 6: toscrape
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-0 0 

./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-1 1

# TEST 7: wikipedia
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wiki-0 0

./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wiki-1 1
