#!/bin/bash 
#
# TESTING.sh: script to run valgrind on crawler.c
#
#!/bin/bash 
#
# VALGRIND.sh: valgrind test script to test for memory leaks in crawler.c
#
# Input: call "make valgrind" or ./valgrind.sh, I have set up the test cases
# Output: the result of many erroneous and correct test cases and their commmands
#
# Alexander Ye, Fall 2022
# CS 50, Fall 2022

# Memory check with not enough arguments:
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html

# Memory check with external link:
valgrind --leak-check=full --show-leak-kinds=all ./crawler https://www.youtube.com/watch?v=dQw4w9WgXcQ ../data/test 2

# Memory check with nonexistant page directory:
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/nonexistant 12

# Memory check with out of bounds depth:
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/test 12

# Memory check for valid test case:
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/test 10
