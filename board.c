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

	//char *FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // 1 
	//char *FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"; // 2
	/* Initialize lookup tables, board, etc. */
	init_all();
	Uci_Loop();

	free(tbls);
	free(bd);
	free(undo_bd);
	return 0;
}
