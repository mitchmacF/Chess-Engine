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
int main() {
	//char *FEN = "r3k2r/p1ppqpb1/bn2p1pn/3PN3/1p2P3/2N2p2/PPPBBPPP/R3K1RQ w KQkq - 0 1";
	char *FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	//char *FEN = "8/q7/8/8/k7/8/8/K7 w - - 0 1";
	
	/* Initialize lookup tables, board, etc. */
	init_all();
	//srand(time(0));
	int idx;

	/* Parse board position string */
	parseFEN(FEN);

	/* Generate all possible moves for current board state */

	for(int i = 0; i < 100; i++) {
		printf("Move Number: %d\n", i);
		generateAllMoves(bd->to_move);
		//printMoveList();
		printf("\n");
		idx = rand() % mv_list->total_count;
		printMove(mv_list->moves[idx]);
		while(1) {
			if(make_move(mv_list->moves[idx])) {
				//printf("\nBREAKING <<<<<< \n");
				break;
			}
			idx = rand() % mv_list->total_count;
			printf("\nNEW IDX <<<<<< \n");
		}
		/*if(!(bd->WhitePieces & ~bd->WhiteKing) && !(bd->BlackPieces & ~bd->BlackKing)) {
			printCharBoard();
			printf("\nGAME IS DRAWN...goodbye\n");
			break;
		}*/
		printCharBoard();
	}
	
	free(tbls);
	free(bd);
	return 0;
}
