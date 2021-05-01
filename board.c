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

	if(argc != 2) {
		fprintf(stderr, "Usage: exe depth\n");
		exit(0);
	}

	//char *FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // 1 
	char *FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"; // 2
	/* Initialize lookup tables, board, etc. */
	init_all();
	//srand(time(0));
	//int idx;

	/* Parse board position string */
	parseFEN(FEN);

	int depth = atoi(argv[1]);
	U64 nodes = Perft(depth);
	printf("Total nodes = %llu\n", nodes);
	printf("Captures %d\n", captures);
	printf("Castles %d\n", castles);
	printf("EP %d\n", ep);
	printf("Promotions %d\n", promotions);
	printf("Checks %d\n", checks);

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
	free(undo_bd);
	return 0;
}
