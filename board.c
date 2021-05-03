#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include "board.h"
#include "extern.h"

// Update move list
int main(int argc, char **argv) {

	/*if(argc != 2) {
		fprintf(stderr, "Usage: exe depth\n");
		exit(0);
	}*/

	//char *FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // 1 
	char *FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"; // 2
	/* Initialize lookup tables, board, etc. */
	init_all();

	/* Parse board position string */
	parseFEN(FEN);

	
	/*printCharBoard();
	Search();
	printCharBoard();
	Search();
	printCharBoard();*/

	int depth = atoi(argv[1]);
	U64 nodes = Perft(depth);
	printf("Total nodes = %llu\n", nodes);
	printf("Captures %d\n", captures);
	printf("Castles %d\n", castles);
	printf("EP %d\n", ep);
	printf("Promotions %d\n", promotions);
	printf("Checks %d\n", checks);

	free(tbls);
	free(bd);
	free(undo_bd);
	return 0;
}
