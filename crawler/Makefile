# Makefile for CS50 Tiny Search Engine Crawler
#
# Alexander Ye, Oct 2022
# CS 50, Fall 2022

PROG = crawler
OBJS = crawler.o ../common/pagedir.o
LIBS = ../libcs50/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc

# MAKE = make
# for memory-leak tests
#VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

test: crawler
	./testing.sh
	
valgrind: crawler 
	./valgrind.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f core
