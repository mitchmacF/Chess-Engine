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

	int depth = 2;
	U64 nodes = Perft(depth);
	printf("Tot nodes = %llu\n", nodes);

	/******************/
	
		//generateAllMoves(mv_list);
		//printCharBoard();
		/*printf("\n");
		make_move(mv_list->moves[0]);
		printCharBoard();
		printf("\n");
		undo_move(bd, undo_bd);
		printCharBoard();
		printf("\n");*/



	/******************/

	/* Generate all possible moves for current board state */

	/*for(int i = 0; i < 100; i++) {
		//printState();
		printf("\nMove Number: %d\n", i);
		generateAllMoves(mv_list);
		printMoveList(mv_list);
		printf("\n");
		fflush(stdout);
		//idx = rand() % mv_list->total_count;
		scanf("%d", &idx);
		//idx = 0;
		printMove(mv_list->moves[idx]);
		while(1) {
			if(make_move(mv_list->moves[idx])) {
				//printf("\nBREAKING <<<<<< \n");
				break;
			}
			idx = rand() % mv_list->total_count;
			printf("\nNEW IDX <<<<<< \n");
		}
		if(!(bd->WhitePieces & ~bd->WhiteKing) && !(bd->BlackPieces & ~bd->BlackKing)) {
			printCharBoard();
			printf("\nGAME IS DRAWN...goodbye\n");
			break;
		}
		printCharBoard();
	}*/

	/*for(int i = 0; i < 100; i++) {
		printf("\nMove Number: %d", i);
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
		if(!(bd->WhitePieces & ~bd->WhiteKing) && !(bd->BlackPieces & ~bd->BlackKing)) {
			printCharBoard();
			printf("\nGAME IS DRAWN...goodbye\n");
			break;
		}
		printCharBoard();
	}*/

	
	free(tbls);
	free(bd);
	return 0;
}
