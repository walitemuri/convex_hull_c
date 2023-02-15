CC=gcc
CFLAGS=-c -Wall -std=c99 -Wall -pedantic

all: P12 P11 P21 P22
P11: P11.c
	$(CC) P11.c -o P11

P12: P12.c
	$(CC) P12.c -o P12

P21: P21.c
	$(CC) P21.c -o P21 -lm

P22: P22.c
	$(CC) P22.c -o P22 -lm

clean:
	rm -rf *o P11 P12 P21 P22
