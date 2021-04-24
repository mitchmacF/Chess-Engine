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

	char *FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // 1 
	//char *FEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"; // 2
	//char *FEN = "4k3/8/8/8/8/8/8/4K2R w K - 0 1 ;D1 15"; // 3 D3
	//char *FEN = "4k3/8/8/8/8/8/8/R3K3 w Q - 0 1 ;D1 16"; // 4 D3
	//char *FEN = "4k2r/8/8/8/8/8/8/4K3 w k - 0 1 ;D1 5"; // 5 D4
	//char *FEN = "r3k3/8/8/8/8/8/8/4K3 w q - 0 1 ;D1 5"; // 6 D4
	//char *FEN = "4k3/8/8/8/8/8/8/R3K2R w KQ-- - 0 1"; // 7 - D3 

	//char *FEN = "r3k2r/8/8/8/8/8/8/4K3 w --kq - 0 1"; // 8 
	//char *FEN = "8/8/8/8/8/8/6k1/4K2R w K--- - 0 1"; // 9 works through D6

	//char *FEN  = "8/8/8/8/8/8/1k6/R3K3 w -Q-- - 0 1"; // 10 works through D5 maybe D6
	//char *FEN = "4k2r/6K1/8/8/8/8/8/8 w --k-- - 0 1"; // 11 breaks at D4
	//char *FEN = "r3k3/1K6/8/8/8/8/8/8 w ---q - 0 1"; // 12 breaks at D2 -> half/whole move is not working correctly
		
	//char *FEN = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";  // 13 breaks at d3
	//char *FEN = "r3k2r/8/8/8/8/8/8/1R2K2R w K-kq - 0 1"; // 14 breaks at D3
	//char *FEN = "4k3/8/8/8/8/8/8/4K2R b K - 0 1"; // 20 D3
	//char *FEN = "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1"; // 30
	//char *FEN = "K7/8/2n5/1n6/8/8/8/k6N w - - 0 1"; //40
	//
	// 40 - 50
	//char *FEN = "K7/8/2n5/1n6/8/8/8/k6N w - - 0 1"; // 40
	//char *FEN = "k7/8/2N5/1N6/8/8/8/K6n w - - 0 1";  // 41
	// char *FEN = "8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1"; // 42
	//char *FEN = "8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1"; // 43 
	//char *FEN = "8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1"; // 44
	//char *FEN = "K7/8/2n5/1n6/8/8/8/k6N b - - 0 1"; // 45
	//char *FEN = "k7/8/2N5/1N6/8/8/8/K6n b - - 0 1"; // 46
	//char *FEN = "B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1"; // 47
	//char *FEN = "8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1";  // 48
	//char *FEN = "k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1";  // 49
	//char *FEN = "K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1"; // 50


	//char *FEN = "k7/8/7p/8/8/6P1/8/K7 b - - 0 1"; // 100
	//char *FEN = "k7/8/8/6p1/7P/8/8/K7 b - - 0 1"; // 101
	//char *FEN = "k7/8/6p1/8/8/7P/8/K7 b - - 0 1";  // 102
	//char *FEN = "k7/8/8/3p4/4p3/8/8/7K b - - 0 1"; // 103
	//char *FEN = "k7/8/3p4/8/8/4P3/8/7K b - - 0 1"; // 104
	//char *FEN = "7k/8/8/p7/1P6/8/8/7K w - - 0 1"; // 105
	//char *FEN = "7k/8/p7/8/8/1P6/8/7K w - - 0 1";  // 106
	//char *FEN = "7k/8/8/1p6/P7/8/8/7K w - - 0 1"; // 107
	//char *FEN = "7k/8/1p6/8/8/P7/8/7K w - - 0 1";  // 108
	//char *FEN = "k7/7p/8/8/8/8/6P1/K7 w - - 0 1";  // 109
	//char *FEN = "k7/6p1/8/8/8/8/7P/K7 w - - 0 1";  // 110
	//char *FEN = "3k4/3pp3/8/8/8/8/3PP3/3K4 w - -0 1";  // 111
	//char *FEN = "7k/8/8/p7/1P6/8/8/7K b - - 0 1"; // 112
	//char *FEN = "7k/8/p7/8/8/1P6/8/7K b - - 0 1"; // 113 
	//char *FEN = "7k/8/8/1p6/P7/8/8/7K b - - 0 1"; // 114
	//char *FEN = "7k/8/1p6/8/8/P7/8/7K b - - 0 1";  // 115
	//char *FEN = "k7/7p/8/8/8/8/6P1/K7 b - - 0 1"; // 116
	//char *FEN = "k7/6p1/8/8/8/8/7P/K7 b - - 0 1";    // 117
	//char *FEN = "3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1";   // 118
	//char *FEN = "8/Pk6/8/8/8/8/6Kp/8 w - - 0 1";   // 119  -> D1 gets 8 instead of 11 think it's cause we only promote to Q, not B, N, R
	//char *FEN = "n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1";  // 120 -> D1 off, yep def promotion related
	//char *FEN = "8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1"; // 121 D1
	//char *FEN = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"; // 122 D1
	//char *FEN = "8/Pk6/8/8/8/8/6Kp/8 b - - 0 1"; // 123 D1
	//char *FEN = "n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1"; // 124 D1
	//char *FEN = "8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1"; // 125 D1
	//char *FEN = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1"; //126 D1
	
	/* Initialize lookup tables, board, etc. */
	init_all();
	//srand(time(0));
	int idx;

	/* Parse board position string */
	parseFEN(FEN);

	int depth = atoi(argv[1]);
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
