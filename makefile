CC=gcc
CFLAGS=-c -Wall -std=c99 -Wall -pedantic

all: q1 q2 test

q1: q1.c
	$(CC) q1.c -o q1

q2: q2.c
	$(CC) q2.c -o q2

test: test.c
	$(CC) test.c -o test

clean:
	rm -rf *o q1 q2 test
