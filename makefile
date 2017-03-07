#Variables
CC = g++
GCC = gcc

CFLAGS = -Wall -pedantic -ansi
GCFLAGS = gcc -c
DEBUG = -g

all: isr-prog2

isr-prog2: stemmer.o
	$(CC) $(CFLAGS) $(DEBUG) isr-prog2.cpp stemmer.o -o isr-prog2

stemmer.o: 
	$(GCFLAGS) stemmer.c

clean:
	rm -rf *o out


