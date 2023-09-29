# Makefile for CS50 Tiny Search Engine common
#
# Alexander Ye,
# CS 50, Fall 2022

PROG = 
OBJS = pagedir.o index.o word.o
LIBS = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc


$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

pagedir.o: pagedir.h
index.o: index.h
word.o: word.h

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -f $(LIBS)
	rm -f core
