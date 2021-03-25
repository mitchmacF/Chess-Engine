CC=gcc
CFLAGS=-I. -g -Wall 

all:
	$(CC) board.c moves.c io.c init.c -o chess

clean:
	rm -f chess 
