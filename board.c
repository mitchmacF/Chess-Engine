#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "board.h"
#include "extern.h"

// it would be nice to have char board that updates after every move

int main() {
	//char *FEN = "8/pppppppp/8/8/8/8/PPPPPPPP/8 w KQkq - 0 1";
	//char *FEN = "r3k2r/p1ppqpb1/bn2p1pn/3PN3/1p2P3/2N2p2/PPPBBPPP/R3K1RQ w KQkq - 0 1";
	char *FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	
	/* Initialize lookup tables, board, etc. */
	init_all();

	/* Parse board position string */
	parseFEN(FEN);

	printCharBoard();
	//printState();

	/* Generate all possible moves for current board state */
	generateAllMoves(bd->to_move);
	printMoveList();

	free(tbls);
	free(bd);
	return 0;
}
