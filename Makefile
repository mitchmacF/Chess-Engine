CC=gcc
CFLAGS=-I. -g -Wall 

all:
	$(CC) $(CFLAGS) board.c move_generation.c update_bitboard.c io.c initialize.c perft.c -o chess

clean:
	rm -f chess 
	rm -f *.txt 
