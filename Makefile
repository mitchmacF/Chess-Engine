CC=gcc
CFLAGS=-I. -g

all:
	$(CC) $(CFLAGS) board.c uci.c search_eval.c move_generation.c update_bitboard.c input_output.c initialize.c perft.c -o chess
clean:
	rm -f chess 
	rm -f *.txt 
