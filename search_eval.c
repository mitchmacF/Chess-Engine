#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <limits.h>
#include "board.h"
#include "extern.h"

const int PawnTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int KnightTable[64] = {
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0		
};

const int BishopTable[64] = {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int RookTable[64] = {
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0		
};

const int Mirror64[64] = {
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};

/* Macros */
#define MIRROR64(sq) (Mirror64[(sq)])
#define INFINITY INT_MAX
#define U64toIDX(c) bitScanForward(c)
#define IDXtoU64(c) getPosition(c)

int materialDifference() {
	int w_score = 0;
	int b_score = 0;

	U64 w_pieces = bd->WhitePieces;
	while(w_pieces) {
		U64 w_piece = IDXtoU64(U64toIDX(w_pieces));
		if(w_piece & bd->WhitePawns)
			w_score += 1;
		else if(w_piece & bd->WhiteKnights)
			w_score += 3;
		else if(w_piece & bd->WhiteBishops)
			w_score += 3;
		else if(w_piece & bd->WhiteRooks)
			w_score += 5;
		else if(w_piece & bd->WhiteQueens)
			w_score += 9;

		w_pieces = Pop(w_pieces);
	}

	U64 b_pieces = bd->BlackPieces;
	while(b_pieces) {
		U64 b_piece = IDXtoU64(U64toIDX(b_pieces));
		if(b_piece & bd->BlackPawns)
			b_score += 1;
		else if(b_piece & bd->BlackKnights)
			b_score += 3;
		else if(b_piece & bd->BlackBishops)
			b_score += 3;
		else if(b_piece & bd->BlackRooks)
			b_score += 5;
		else if(b_piece & bd->BlackQueens)
			b_score += 9;

		b_pieces = Pop(b_pieces);
	}

	return w_score - b_score;
}

int Evaluate() {
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


int AlphaBeta(int depth, int alpha, int beta) {
	struct Move_list *mv_list;
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;

	if (depth == 0) {
		free(mv_list);
		return Evaluate();
	}

	generateAllMoves(mv_list);
	for (int i = 0; i < mv_list->total_count; i++) {
		if(!make_move(mv_list->moves[i])) {
			undo_move(bd, mv_list->moves[i].undo);
			continue;
		}
		int val = -AlphaBeta(depth - 1, -beta, -alpha);

		undo_move(bd, mv_list->moves[i].undo);
		
		if (val >= beta)
			return beta;
		if (val > alpha)
			alpha = val;
	}
	free(mv_list->moves[0].undo);
	free(mv_list);
	return alpha;
}

void Search() {
	Move curr_move, best_move;
	int i, mate = 0, curr_move_score = 0, best_move_score = 0;
	struct Move_list *mv_list;
	
	mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	mv_list->total_count = 0;

	generateAllMoves(mv_list);
	for(i = 0; i < mv_list->total_count; i++) {
		if(!make_move(mv_list->moves[i])) {
			undo_move(bd, mv_list->moves[i].undo);
			mate++;
			if(mate++ == mv_list->total_count) {
				printf("CHECKMATE\n");
				exit(0);
			}
			continue;
		} else {
			best_move = mv_list->moves[i];
			best_move_score = -AlphaBeta(5, -INFINITY, INFINITY);
			undo_move(bd, mv_list->moves[i].undo);
			break;
		}
	}
	for(i = 0; i < mv_list->total_count; i++) {
		curr_move = mv_list->moves[i];
		if(!make_move(mv_list->moves[i])) {
			undo_move(bd, mv_list->moves[i].undo);
			continue;
		}
		curr_move_score = -AlphaBeta(5, -INFINITY, INFINITY);
		printMove(curr_move);
		printf(": ");
		printf("score: %d %d\n", curr_move_score, best_move_score);
	
		undo_move(bd, mv_list->moves[i].undo);
		if(curr_move_score > best_move_score) {
			best_move_score = curr_move_score;
			best_move = curr_move;
		}
	}
	make_move(best_move);
	printf("bestmove ");
	printMove(best_move);
	printf("\n");
	free(mv_list);
}
