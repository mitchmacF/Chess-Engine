#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include "board.h"
#include "extern.h"

#define moveMAX 100
#define MAX_DEPTH 15 

int timeLeft(int curr_time, int allotted_time) {
	if(curr_time >= allotted_time) 
		return 0;
	else 
		return 1;
}

int search;
Move best_move;
int nodes = 0;
typedef struct tagLINE {
	int cmove;
	Move argmove[moveMAX];
} LINE;

const int PawnTable[64] = {
0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 , 0 ,
5 ,  10,   5, -20, -20,  50,  10, 5 ,
5 , -5 ,   0,  5 ,  3 ,  0 , -5 , 5 ,
0 ,  0 ,   0,  25,  25,  0 ,  0 , 0 ,
3 ,  5 ,  10,  20,  20,  10,  5 , 3 ,
5 ,  10,  20,  30,  30,  20,  10, 5 ,
50,  50,  50,  50,  50,  50,  50, 50,
0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 , 0 
};

const int KnightTable[64] = {
-50, -40, -30, -30, -30, -30, -40, -50,
-40, -20,   0,   5,   5,   0, -20, -40, 
-30,   0,   5,  10,  10,   5,   0, -30, 
-30,   5,  15,  20,  20,  15,   5, -30, 
-30,  10,  15,  20,  20,  15,   5, -30,
-30,  10,   8,  15,  15,   8,   5, -30,
-40, -20,   0,   5,   5,   0, -20, -40,
-50,- 40, -30, -30, -30, -30, -40, -50
};

const int BishopTable[64] = {
 20, -10, -10, -10, -10, -10, -10, -20,
-10,   6,   0,   0,   0,   0,   6, -10,
-10,  10,  10,  10,  10,  10,  10, -10,
-10,   0,  10,  10,  10,  10,   0, -10,
-10,   5,   5,  10,  10,   5,   5, -10,
-10,   0,   5,  10,  10,   5,   0, -10,
-10,   0,   0,   0,   0,   0,   0, -10,
-20, -10, -10, -10, -10, -10, -10, -20
};

const int RookTable[64] = {
 0,  0,  3,  7,  7,  3,  0,  0,
-5,  0,  0,  0,  0,  0,  0, -5,
-5,  0,  0,  0,  0,  0,  0, -5,
-5,  0,  0,  0,  0,  0,  0, -5,
-5,  0,  0,  0,  0,  0,  0, -5,
-5,  0,  0,  0,  0,  0,  0, -5,
 5, 10, 10, 10, 10, 10, 10,  5,
 0,  0,  0,  0,  0,  0,  0,  0
};

const int Mirror64[64] = {
56,	57,	58,	59,	60,	61,	62,	63,
48,	49,	50,	51,	52,	53,	54,	55,
40,	41,	42,	43,	44,	45,	46,	47,
32,	33,	34,	35,	36,	37,	38,	39,
24,	25,	26,	27,	28,	29,	30,	31,
16,	17,	18,	19,	20,	21,	22,	23,
8 ,	9,	10,	11,	12,	13,	14,	15,
0 ,	1,	2,	3,	4,	5,	6,	7
};

/* Macros */
#define MIRROR64(sq) (Mirror64[(sq)])
#define INFINITY INT_MAX
#define U64toIDX(c) bitScanForward(c)
#define IDXtoU64(c) getPosition(c)

void sortMvList(struct Move_list *mv_list, LINE *pline) {
	Move curr_mv, temp;
	int i;
	for(i = 0; i < mv_list->total_count; i++) {
		curr_mv = mv_list->moves[i];
		if(curr_mv.mv == pline->argmove[search-2].mv && curr_mv.pc == pline->argmove[search-2].pc) {
			temp = mv_list->moves[0];
			mv_list->moves[0] = curr_mv;
			mv_list->moves[i] = temp;
		}
	}
}

int checkMate() {
	struct Move_list *mv_list;
	int foundLegal = 0;
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;

	generateAllMoves(mv_list);
	for (int i = 0; i < mv_list->total_count; i++) {
		if(!make_move(mv_list->moves[i])) {
			undo_move(bd, mv_list->moves[i].undo);
			continue;
		}
		foundLegal = 1;
		undo_move(bd, mv_list->moves[i].undo);
	}

	if(!foundLegal)
		return 1;
	else
		return 0;
}

int materialDifference() {
	int w_score = 0;
	int b_score = 0;

	U64 w_pieces = bd->WhitePieces;
	while(w_pieces) {
		U64 w_piece = IDXtoU64(U64toIDX(w_pieces));
		if(w_piece & bd->WhitePawns)
			w_score += 100;
		else if(w_piece & bd->WhiteKnights)
			w_score += 300;
		else if(w_piece & bd->WhiteBishops)
			w_score += 310;
		else if(w_piece & bd->WhiteRooks)
			w_score += 500;
		else if(w_piece & bd->WhiteQueens)
			w_score += 900; 

		w_pieces = Pop(w_pieces);
	}

	U64 b_pieces = bd->BlackPieces;
	while(b_pieces) {
		U64 b_piece = IDXtoU64(U64toIDX(b_pieces));
		if(b_piece & bd->BlackPawns)
			b_score += 100;
		else if(b_piece & bd->BlackKnights)
			b_score += 300;
		else if(b_piece & bd->BlackBishops)
			b_score += 310;
		else if(b_piece & bd->BlackRooks)
			b_score += 500;
		else if(b_piece & bd->BlackQueens)
			b_score += 900;

		b_pieces = Pop(b_pieces);
	}

	return w_score - b_score;
}

int Evaluate() {

	if(checkMate()) {
		return -INFINITY;
	}
	
	int score = materialDifference();

	// White Pieces Evaluation
	U64 white_pawns = bd->WhitePawns;
	while(white_pawns) {
		U64 wP = IDXtoU64(U64toIDX(white_pawns));
		score += PawnTable[U64toIDX(wP)];
		white_pawns = Pop(white_pawns);
	}

	U64 white_knights = bd->WhiteKnights;
	while(white_knights) {
		U64 wN = IDXtoU64(U64toIDX(white_knights));
		score += KnightTable[U64toIDX(wN)];
		white_knights = Pop(white_knights);
	}

	U64 white_bishops = bd->WhiteBishops;
	while(white_bishops) {
		U64 wB = IDXtoU64(U64toIDX(white_bishops));
		score += BishopTable[U64toIDX(wB)];
		white_bishops = Pop(white_bishops);
	}

	U64 white_rooks = bd->WhiteRooks;
	while(white_rooks) {
		U64 wR = IDXtoU64(U64toIDX(white_rooks));
		score += RookTable[U64toIDX(wR)];
		white_rooks = Pop(white_rooks);
	}
	
	// Black Pieces Evaluation
	U64 black_pawns = bd->BlackPawns;
	while(black_pawns) {
		U64 bP = IDXtoU64(U64toIDX(black_pawns));
		score -= PawnTable[MIRROR64(U64toIDX(bP))];
		black_pawns = Pop(black_pawns);
	}

	U64 black_knights = bd->BlackKnights;
	while(black_knights) {
		U64 bN = IDXtoU64(U64toIDX(black_knights));
		score -= KnightTable[MIRROR64(U64toIDX(bN))];
		black_knights = Pop(black_knights);
	}

	U64 black_bishops = bd->BlackBishops;
	while(black_bishops) {
		U64 bB = IDXtoU64(U64toIDX(black_bishops));
		score -= BishopTable[MIRROR64(U64toIDX(bB))];
		black_bishops = Pop(black_bishops);
	}

	U64 black_rooks = bd->BlackRooks;
	while(black_rooks) {
		U64 bR = IDXtoU64(U64toIDX(black_rooks));
		score -= RookTable[MIRROR64(U64toIDX(bR))];
		black_rooks = Pop(black_rooks);
	}

	if(bd->to_move == WHITE)
		return score;
	else
		return -score;
}

// consider two diff mate methods
int AlphaBeta(int depth, int alpha, int beta, LINE *pline) {
	
	LINE line;
	int val, legal = 0, best_score, curr_time;
	struct Move_list *mv_list;
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;

	if (depth == 0) {
		nodes++;
		pline->cmove = 0;
		free(mv_list);
		return Evaluate();
	}

	generateAllMoves(mv_list);
	if(search != 1) {
		sortMvList(mv_list, pline);
	}
	for (int i = 0; i < mv_list->total_count; i++) {
		Move curr_move = mv_list->moves[i];
		if(!make_move(mv_list->moves[i])) {
			undo_move(bd, mv_list->moves[i].undo);
			continue;
		}
		printf("depth %d 	alpha %d 		beta %d\n", depth, alpha, beta);
		val = -AlphaBeta(depth - 1, -beta, -alpha, &line);
		undo_move(bd, mv_list->moves[i].undo);
		if(depth == search) {
			if(i == 0) {
				best_score = val;	
				best_move = curr_move;
			}
			/*if(bd->to_move == WHITE) {
				if(best_score < val) {
					best_move = curr_move;
					best_score = val;
				}
			} else {
				if(best_score < val) {
					best_move = curr_move;
					best_score = val;
				}
			}*/
			if(best_score < val) {
				best_move = curr_move;
				best_score = val;
			}
		}

		if (val >= beta) {
			return beta;
		}
		if (val > alpha) {
			alpha = val;
			pline->argmove[0] = mv_list->moves[i];
			memcpy(pline->argmove + 1, line.argmove,
				line.cmove * sizeof(Move));
			pline->cmove = line.cmove + 1;
		}
	}
	free(mv_list->moves[0].undo);
	free(mv_list);
	return alpha;
}

void Search(int time) {
	printf("time to start %d\n", time);

	double fstart, fnow; 
	struct timespec start, now;

	clock_gettime(CLOCK_MONOTONIC, &start);
	fstart = (double)start.tv_sec + (double)start.tv_nsec / 1000000000.;

	LINE pline;
	Move curr_move;
	int i, mate = 0, score, curr_score = 0;
	nodes = 0;
		
	clock_gettime(CLOCK_MONOTONIC, &now);
	
	for(int depth = 1;depth < 3; depth++) {
		search = depth;
		
		/* print score and best move */
		score = AlphaBeta(depth, -INFINITY, INFINITY, &pline);
		printf("\n%d: score %d\n", depth, score);
		printf("best move ");
		printMove(best_move);
		printf("\n");
		
		/* check and print time */
		clock_gettime(CLOCK_MONOTONIC, &now);
		fnow = (double)now.tv_sec + (double)now.tv_nsec / 1000000000.;
		printf("elapsed time: %lf nodes: %d\n", fnow-fstart, nodes);
		printf("\n");

		if(!timeLeft((int)(fnow-fstart), time)) {
			printf("TIMES UP\n");
			break;
		}
	}
	/*printf("\n");
	for(i = 0; i < pline.cmove; i++) {
		printMove(pline.argmove[i]);
		printf("\n");
	}*/
	make_move(best_move);
	printf("bestmove ");
	printMove(best_move);
	printf("\n");
}
