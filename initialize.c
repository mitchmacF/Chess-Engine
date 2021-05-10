#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "extern.h"

struct Lookup_tables *tbls;
//struct Move_list *mv_list;
char *move_notation[64];
struct Board *bd;
struct Board *undo_bd;
char board[64]; 
Move INVALID = {0, 0, 0, 0};

// For perft debugging
int captures = 0;
int ep = 0;
int castles = 0;
int promotions = 0;
int checks = 0;

void init_mv_notation() {

	char *temp_mv_notation[64] = {
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
	};

	for(int i = 0; i < 64; i++) {
		move_notation[i] = temp_mv_notation[i];
	}
}

void init_lookup_tables_mv_list() {

	tbls = (struct Lookup_tables *)malloc(sizeof(struct Lookup_tables));
	//mv_list = (struct Move_list *)malloc(sizeof(struct Move_list));
	//mv_list->total_count = 0;
	

	U64 maskrank[8] = {0xFF, 0xFF00, 0xFF0000, 0xFF000000, 0xFF00000000,
						0xFF0000000000, 0xFF000000000000, 0xFF00000000000000};

	U64 clearrank[8] = {0xFFFFFFFFFFFFFF00, 0xFFFFFFFFFFFF00FF, 0xFFFFFFFFFF00FFFF, 
							0xFFFFFFFF00FFFFFF, 0xFFFFFF00FFFFFFFF, 0xFFFF00FFFFFFFFFF,
							0xFF00FFFFFFFFFFFF, 0x00FFFFFFFFFFFFFF};

	U64 maskfile[8] = { 0x101010101010101, 0x202020202020202, 0x404040404040404, 
						   0x808080808080808, 0x1010101010101010, 0x2020202020202020,
						   0x4040404040404040, 0x8080808080808080};

	U64 clearfile[8] = { 0xFEFEFEFEFEFEFEFE, 0xFDFDFDFDFDFDFDFD,	0xFBFBFBFBFBFBFBFB,
							0xF7F7F7F7F7F7F7F7, 0xEFEFEFEFEFEFEFEF, 0xDFDFDFDFDFDFDFDF,
							0xBFBFBFBFBFBFBFBF, 0x7F7F7F7F7F7F7F7F};

	U64 diagonalmask[15] = {0x100000000000000, 0x201000000000000, 0x402010000000000, 
						   0x804020100000000, 0x1008040201000000, 0x2010080402010000,
						   0x4020100804020100, 0x8040201008040201, 0x80402010080402, 
						   0x804020100804, 0x8040201008, 0x80402010, 0x804020, 0x8040, 0x80};


	U64 antidiagonalmask[15] = {0x8000000000000000, 0x4080000000000000, 0x2040800000000000, 
							  0x1020408000000000, 0x810204080000000, 0x408102040800000, 
							  0x204081020408000, 0x102040810204080, 0x1020408102040, 
							  0x10204081020, 0x102040810, 0x1020408, 0x10204, 0x102, 0x1};

	for(int i = 0; i < 8; i++) {
		tbls->MaskRank[i] = maskrank[i];
		tbls->ClearRank[i] = clearrank[i];
		tbls->MaskFile[i] = maskfile[i];
		tbls->ClearFile[i] = clearfile[i];
	}
	for(int i = 0; i < 15; i++) {
		tbls->DiagonalMask[i] = diagonalmask[i];
		tbls->AntiDiagonalMask[i] = antidiagonalmask[i];
	}
}

void init_board() {
	bd = (struct Board *)malloc(sizeof(struct Board));
	bd->WhitePawns = 0ULL;
	bd->WhiteRooks = 0ULL;
	bd->WhiteKnights = 0ULL;
	bd->WhiteBishops = 0ULL;
	bd->WhiteQueens = 0ULL;
	bd->WhiteKing = 0ULL;

	bd->BlackPawns = 0ULL;
	bd->BlackRooks = 0ULL;
	bd->BlackKnights = 0ULL;
	bd->BlackBishops = 0ULL;
	bd->BlackQueens = 0ULL;
	bd->BlackKing = 0ULL;

	bd->WhitePieces = 0ULL;
	bd->BlackPieces = 0ULL;
	bd->WhiteAttacking = 0ULL;
	bd->BlackAttacking = 0ULL;
	bd->AllPieces = 0ULL;

	bd->to_move = 0;

	bd->castle[0] = '-';
	bd->castle[1] = '-';
	bd->castle[2] = '-';
	bd->castle[3] = '-';
	
	bd->ep = 0ULL;

	bd->half_move = 0;
	bd->whole_move = 1;
	
	undo_bd = (struct Board *)malloc(sizeof(struct Board));
	undo_bd->WhitePawns = 0ULL;
	undo_bd->WhiteRooks = 0ULL;
	undo_bd->WhiteKnights = 0ULL;
	undo_bd->WhiteBishops = 0ULL;
	undo_bd->WhiteQueens = 0ULL;
	undo_bd->WhiteKing = 0ULL;

	undo_bd->BlackPawns = 0ULL;
	undo_bd->BlackRooks = 0ULL;
	undo_bd->BlackKnights = 0ULL;
	undo_bd->BlackBishops = 0ULL;
	undo_bd->BlackQueens = 0ULL;
	undo_bd->BlackKing = 0ULL;

	undo_bd->WhitePieces = 0ULL;
	undo_bd->BlackPieces = 0ULL;
	undo_bd->WhiteAttacking = 0ULL;
	undo_bd->BlackAttacking = 0ULL;
	undo_bd->AllPieces = 0ULL;

	undo_bd->to_move = 0;

	undo_bd->castle[0] = '-';
	undo_bd->castle[1] = '-';
	undo_bd->castle[2] = '-';
	undo_bd->castle[3] = '-';
	
	undo_bd->ep = 0ULL;

	undo_bd->half_move = 0;
	undo_bd->whole_move = 1;
}

void copy(struct Board *to, struct Board *from) {
	
	to->WhitePawns = from->WhitePawns;
	to->WhiteRooks = from->WhiteRooks;
	to->WhiteKnights = from->WhiteKnights;
	to->WhiteBishops = from->WhiteBishops;
	to->WhiteQueens = from->WhiteQueens;
	to->WhiteKing = from->WhiteKing;

	to->BlackPawns = from->BlackPawns;
	to->BlackRooks = from->BlackRooks;
	to->BlackKnights = from->BlackKnights;
	to->BlackBishops = from->BlackBishops;
	to->BlackQueens = from->BlackQueens;
	to->BlackKing = from->BlackKing;

	to->WhitePieces = from->WhitePieces;
	to->BlackPieces = from->BlackPieces;
	to->WhiteAttacking = from->WhiteAttacking;
	to->BlackAttacking = from->BlackAttacking;
	to->AllPieces = from->AllPieces;

	to->to_move = from->to_move;

	to->castle[0] = from->castle[0];
	to->castle[1] = from->castle[1];
	to->castle[2] = from->castle[2];
	to->castle[3] = from->castle[3];
	
	to->ep = from->ep;

	to->half_move = from->half_move;
	to->whole_move = from->whole_move;
}

void init_visual_board() {
	memset(board, '.', 64*sizeof(char));
}

void init_all() {
	init_lookup_tables_mv_list();
	init_board();
	init_visual_board();
	init_mv_notation();
}

void clear_board() {
	bd->WhitePawns = 0ULL;
	bd->WhiteRooks = 0ULL;
	bd->WhiteKnights = 0ULL;
	bd->WhiteBishops = 0ULL;
	bd->WhiteQueens = 0ULL;
	bd->WhiteKing = 0ULL;

	bd->BlackPawns = 0ULL;
	bd->BlackRooks = 0ULL;
	bd->BlackKnights = 0ULL;
	bd->BlackBishops = 0ULL;
	bd->BlackQueens = 0ULL;
	bd->BlackKing = 0ULL;

	bd->WhitePieces = 0ULL;
	bd->BlackPieces = 0ULL;
	bd->WhiteAttacking = 0ULL;
	bd->BlackAttacking = 0ULL;
	bd->AllPieces = 0ULL;

	bd->to_move = 0; 
	bd->castle[0] = '-';
	bd->castle[1] = '-';
	bd->castle[2] = '-';
	bd->castle[3] = '-';
	
	bd->ep = 0ULL;

	bd->half_move = 0;
	bd->whole_move = 1;
}
