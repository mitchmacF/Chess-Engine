#ifndef BOARD_H
#define BOARD_H 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_MOVES 128
//#define getPosition(sq) (1ULL<<sq)

typedef unsigned long long U64;

typedef enum { 
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8
} Square;

typedef enum {
	WHITE, BLACK
} Side;

enum Move_type {
	Quiet,
	Castle = 1 << 14,
	Promotion = 2 << 14, 
	EP = 3 << 14
};

typedef enum {
	KING = 1,
	QUEEN,
	ROOK,
	BISHOP,
	KNIGHT,
	PAWN
} Piece;

enum { 
	FILE_A, 
	FILE_B, 
	FILE_C, 
	FILE_D, 
	FILE_E, 
	FILE_F, 
	FILE_G, 
	FILE_H 
};

enum { 
	RANK_1, 
	RANK_2, 
	RANK_3, 
	RANK_4, 
	RANK_5, 
	RANK_6, 
	RANK_7, 
	RANK_8 
};

struct Board {
	/* White pieces */
	U64 WhitePawns;
	U64 WhiteRooks;
	U64 WhiteKnights;
	U64 WhiteBishops;
	U64 WhiteQueens;
	U64 WhiteKing;
	
	/* Black pieces */
	U64 BlackPawns;
	U64 BlackRooks;
	U64 BlackKnights;
	U64 BlackBishops;
	U64 BlackQueens;
	U64 BlackKing;
	
	/* Other useful bitboards */
	U64 WhitePieces;
	U64 BlackPieces;
	U64 AllPieces;

	/* To move: 0 - w, 1 - b */
	Side to_move;

	/* castle */
	char castle[4];

	/* En passant */
	U64 ep;

	/* 50 move rule */
	int half_move;
	int whole_move;
};

/* Lookup Tables */
struct Lookup_tables {
	U64 MaskRank[8];
	U64 ClearRank[8];
	U64 MaskFile[8];
	U64 ClearFile[8];
	U64 DiagonalMask[15];
	U64 AntiDiagonalMask[15];
}; 

typedef struct {
	int mv;
	int mv_score;
	Piece pc;
} Move;

struct Move_list {
	Move moves[MAX_MOVES];
	int total_count;
};

#endif
