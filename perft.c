// perft.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "board.h"
#include "extern.h"

long leafNodes;

U64 Perft(int depth) {
	printf("Depth: %d\n", depth);
	printCharBoard();
	printf("\n");
	struct Move_list *mv_list;
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;
	
	int n_moves, i;
	U64 nodes = 0;

	if (depth == 0) {
		undo_move(bd, undo_bd);
		return 1ULL;
	}

	//n_moves = GenerateLegalMoves(move_list);
	generateAllMoves(mv_list);
	for (i = 0; i < mv_list->total_count; i++) {
		if(!make_move(mv_list->moves[i])) {
			printf("Check!!\n");
			continue;
		}
		nodes += Perft(depth - 1);
		printf(">>>> Depth: %d\n", depth);
		printCharBoard();
		printf("\n");
		printMoveList(mv_list);
		printf("Returned from PERFT!!\n");
		printf("\n");
		undo_move(bd, undo_bd);
	}
	return nodes;
}

/*void Perft(int depth) {

	if(depth == 0) {
		leafNodes++;
		return;
	}
	struct Move_list *mv_list;
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;

	generateAllMoves(mv_list);

	int i = 0;
	for(i = 0; i < mv_list->total_count; ++i) {	

		if ( !make_move(mv_list->moves[i]))  {
			continue;
		}
		Perft(depth - 1);
		undo_move(bd, undo_bd);
	}

	return;
}


void PerftTest(int depth) {

	printCharBoard();
	printf("\nStarting Test To Depth:%d\n",depth);	
	leafNodes = 0;
	
	struct Move_list *mv_list;
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;

	generateAllMoves(mv_list);	

	int move;	    
	int i = 0;
	for(i = 0; i < mv_list->total_count; ++i) {
		Move mv = mv_list->moves[i];
		printMove(mv);
		if ( !make_move(mv))  {
			continue;
		}

		long cumnodes = leafNodes;
		Perft(depth - 1);
		undo_move(bd, undo_bd);
		long oldnodes = leafNodes - cumnodes;
		printf("move %d : %ld\n",i+1,  oldnodes);
		printMove(mv);
	}

	printf("\nTest Complete : %ld nodes visited\n",leafNodes);

	return;
}*/
