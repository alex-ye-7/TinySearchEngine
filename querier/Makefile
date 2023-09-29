# Makefile for CS50 Tiny Search Engine Querier
#
# Alexander Ye, Oct 2022
# CS 50, Fall 2022

LIBS = ../common/common.a ../libcs50/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50 
CC = gcc

.PHONY: all test clean

all: querier fuzzquery

querier: querier.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

fuzzquery: fuzzquery.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
	
test: querier fuzzquery
	bash -v testing.sh
	
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f querier
	rm -f fuzzquery
	rm -f core
