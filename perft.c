// perft.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "board.h"
#include "extern.h"

U64 Perft(int depth) {
	/*printf("Beginning of Perft Depth: %d\n", depth);
	printCharBoard();
	printf("\n");*/
	struct Move_list *mv_list;
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;
	
	int i;
	U64 nodes = 0;

	if (depth == 0) {
		free(mv_list);
		return 1ULL;
	}

	//n_moves = GenerateLegalMoves(move_list);
	generateAllMoves(mv_list);

	for (i = 0; i < mv_list->total_count; i++) {
		if(make_move(mv_list->moves[i])) {
			//printf("i %d\n", i);
			//printMoveList(mv_list);
			//printf("\n");
			int temp = 0;
			temp = Perft(depth-1);
			/*if(depth == 1) {
				printMove(mv_list->moves[i]);
				printf(" %d\n", temp);
			}*/
			nodes += temp;
			//printf("Check!!!\n");
			//continue;
		}
		//printf("After Perft return, depth: %d i: %d and print board\n", depth, i);
		/*printCharBoard();
		printf("\n");*/
		undo_move(bd, mv_list->moves[i].undo);
		//printf("Print undo char board after undo\n");
		//printUndoCharBoard();
	}
	free(mv_list->moves[0].undo);
	free(mv_list);
	return nodes;
}
