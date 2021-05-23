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
			nodes += temp;
			/*if(depth == search) {
				printMove(mv_list->moves[i]);
				printf(" %d\n", temp);
			}*/
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
	int q, e, c;
	q = 1;
	e = 1;
	c = 1;
	for (i = 0; i < mv_list->total_count; i++) {
		int curr_mv = mv_list->moves[i].mv;
		int flag = (curr_mv & (0x03<<14))>>14;
		if(flag == 0 && q) {
			free(mv_list->moves[i].undo);
			q = 0;
		} else if(flag == 2 && e) {
			free(mv_list->moves[i].undo);
			e = 0;
		} else if(flag == 3 && c) {
			free(mv_list->moves[i].undo);
			c = 0;
		}
	}

	free(mv_list);
	return nodes;
}
