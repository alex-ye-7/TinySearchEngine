# Makefile for CS50 Tiny Search Engine Indexer
#
# Alexander Ye, Oct 2022
# CS 50, Fall 2022

LIBS = ../common/common.a ../libcs50/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50 
CC = gcc

.PHONY: all test clean

all: indexer indextest

indexer: indexer.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest: indextest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

test: indexer 
	bash -v testing.sh
	
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer
	rm -f indextest
	rm -f core
