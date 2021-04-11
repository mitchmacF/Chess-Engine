#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "board.h"
#include "extern.h"

/* Macros */
#define U64toIDX(c) bitScanForward(c)
#define IDXtoU64(c) getPosition(c)
/* Macros */

void undo_move(struct Board *to, struct Board *from) {
	copy(to, from);
}

// this is the place where you only update board parameters -> if you have to do more then maybe reconsider move gen functions
bool make_move(Move mv) {
	copy(undo_bd, bd);

	int from, to, flag;
	U64 from_U64, to_U64, ep_location;
	Piece pc_to_move;

	// convert mv to bitboard compatible types
	from = (U64)((mv.mv & (0x3f)));
	from_U64 = IDXtoU64(from);
	to = (U64)(((mv.mv & (0x3f<<6))>>6));
	to_U64 = IDXtoU64(to);
	flag = (U64)((mv.mv & (0x03<<14))>>14);
	pc_to_move = mv.pc;
	
	switch(flag) {
		// attacking or quiet move
		case 0:
			update_bb(pc_to_move, to_U64, from_U64);
			break;
		// promotion 
		case 1:
			update_bb_promotion(pc_to_move, to_U64, from_U64);
			break;
		// en Pas
		case 2:
			ep_location = IDXtoU64(bd->ep);
			update_bb_enPas(pc_to_move, ep_location, from_U64);
			break;
		// castle
		case 3:
			update_bb_castle(pc_to_move, to_U64, from_U64);
			break;
		default:
			fprintf(stderr, "ERROR: flag\n");	
	}

	// check if move leaves King in check
	if(bd->to_move == WHITE) {
		if(attacked(bd->WhiteKing, bd->AllPieces, bd->WhitePieces)) {
			printf(">>> Attacked!!\n");
			copy(bd, undo_bd);
			return 0;
		}
	} else if(bd->to_move = BLACK) {
		if(attacked(bd->BlackKing, bd->AllPieces, bd->BlackPieces)) {
			printf(">>> Attacked!!\n");
			copy(bd, undo_bd);
			return 0;
		}
	}

	// reset move list
	mv_list->total_count = 0;
	if(bd->to_move == WHITE)
		bd->to_move = BLACK;
	else 
		bd->to_move = WHITE;
	
	return 1;
}

// Pawns only promoted to queens right now
void update_bb_promotion(Piece pc, U64 to, U64 from) {
		if(bd->to_move == WHITE) {
			bd->WhitePawns ^= from;
			bd->WhiteQueens |= to;
		} else if(bd->to_move == BLACK) {
			bd->BlackPawns ^= from;
			bd->BlackQueens |= to;
		}
}

void update_bb_enPas(Piece pc, U64 to, U64 from) {
	
	if(bd->to_move == WHITE) {
		// clear white pawn curr location, move it to where ep square is
		bd->WhitePawns ^= from;
		bd->WhitePawns |= to;
	
		// clear white pawn curr location, update all white pieces bit board
		bd->WhitePieces ^= from;
		bd->WhitePieces |= to;

		// make sure there is black pawn in front of ep square -> then capture
		if((to>>8) & bd->BlackPawns) {
				bd->BlackPieces ^= (to>>8);
				bd->BlackPawns ^= (to>>8);
		} else {
			printf(">>>> EP ERROR\n");
			exit(-1);
		}
	
		// remove black pawn from all pieces bit board
		bd->AllPieces ^= (to>>8);

	} else if(bd->to_move == BLACK) {
		// clear black pawn curr location, move it to where ep square is
		bd->BlackPawns ^= from;
		bd->BlackPawns |= to;
	
		// clear black pawn curr location, update all white pieces bit board
		bd->BlackPieces ^= from;
		bd->BlackPieces |= to;

		// make sure there is white pawn in front of ep square -> then capture
		if((to<<8) & bd->WhitePawns) {
			bd->WhitePieces ^= (to<<8);
			bd->WhitePawns ^= (to<<8);
		} else {
			printf(">>>> EP ERROR\n");
			exit(-1);
		}

		// remove white pawn from all pieces bit board
		bd->AllPieces ^= (to<<8);
	}

	bd->AllPieces ^= from;
	bd->AllPieces |= to;

	bd->ep = 0;
}

void update_bb_castle(Piece pc, U64 to, U64 from) {
	if(bd->to_move == WHITE) {
		printf(">>>>>>>>>>>>>>>>>>>> WHITE CASTLING\n");
		// King-side
		if(to & (1ULL<<6)) {
			bd->WhiteKing ^= from;
			bd->WhiteKing |= to;
			bd->WhiteRooks ^= (1ULL<<7);
			bd->WhiteRooks |= (1ULL<<5);
			bd->castle[0] = '-';
			bd->castle[1] = '-';
			// Queen-side
		} else if(to & (1ULL<<2)) {
			bd->WhiteKing ^= from;
			bd->WhiteKing |= to;
			bd->WhiteRooks ^= (1ULL);
			bd->WhiteRooks |= (1ULL<<3);
			bd->castle[0] = '-';
			bd->castle[1] = '-';
		} 
	} else if(bd->to_move == BLACK) {
		printf(">>>>>>>>>>>>>>>>>>>> BLACK CASTLING\n");
		// King-side
		if(to & (1ULL<<62)) {
			bd->BlackKing ^= from;
			bd->BlackKing |= to;
			bd->BlackRooks ^= (1ULL<<63);
			bd->BlackRooks |= (1ULL<<61);
			bd->castle[2] = '-';
			bd->castle[3] = '-';
			// Queen-side
		} else if(to & (1ULL<<58)) {
			bd->BlackKing ^= from;
			bd->BlackKing |= to;
			bd->BlackRooks ^= (1ULL<<57);
			bd->BlackRooks |= (1ULL<<59);
			bd->castle[2] = '-';
			bd->castle[3] = '-';
		}
	}
	bd->ep = 0;
}

void update_bb(Piece pc, U64 to, U64 from) {
	if(bd->to_move == WHITE) {
		switch(pc) {
			case KING:
				bd->ep = 0;
				bd->WhiteKing ^= from;
				bd->WhiteKing |= to;
				bd->castle[0] = '-';
				bd->castle[1] = '-';
				break;
			case QUEEN:
				bd->ep = 0;
				bd->WhiteQueens ^= from;
				bd->WhiteQueens |= to;
				break;
			case ROOK:
				bd->ep = 0;
				if(from & 1) {
					bd->castle[1] = '-';
				} else if(from & 1ULL<<7) {
					bd->castle[0] = '-';
				}
				bd->WhiteRooks ^= from;
				bd->WhiteRooks |= to;
				break;
			case BISHOP:
				bd->ep = 0;
				bd->WhiteBishops ^= from;
				bd->WhiteBishops |= to;
				break;
			case KNIGHT:
				bd->ep = 0;
				bd->WhiteKnights ^= from;
				bd->WhiteKnights |= to;
				break;
			case PAWN:
				if(to & from<<16) {
					bd->ep = bitScanForward(from<<8); 
				} else {
					bd->ep = 0;
				}
				bd->WhitePawns ^= from;
				bd->WhitePawns |= to;
				break;
			default:
				printf("ERROR: Piece to move, type unknown.");
				break;
		}
		
		// black piece captured
		if(to & bd->BlackPieces) {
			bd->BlackPieces ^= to;
			if(to & bd->BlackQueens) {
				bd->BlackQueens ^= to;	
			} else if(to & bd->BlackRooks) {
				bd->BlackRooks ^= to;
			} else if(to & bd->BlackBishops) {
				bd->BlackBishops ^= to;
			} else if(to & bd->BlackKnights) {
				bd->BlackKnights ^= to;
			} else if(to & bd->BlackPawns) {
				bd->BlackPawns ^= to;
			}
		}

		bd->WhitePieces ^= from;
		bd->WhitePieces |= to;
	} else if(bd->to_move == BLACK) {
		switch(pc) {
			case KING:
				bd->ep = 0;
				bd->BlackKing ^= from;
				bd->BlackKing |= to;
				bd->castle[2] = '-';
				bd->castle[3] = '-';
				break;
			case QUEEN:
				bd->ep = 0;
				bd->BlackQueens ^= from;
				bd->BlackQueens |= to;
				break;
			case ROOK:
				bd->ep = 0;
				if(from & 1ULL<<56) {
					bd->castle[3] = '-';
				} else if(from & 1ULL<<63) {
					bd->castle[2] = '-';
				}
				bd->BlackRooks ^= from;
				bd->BlackRooks |= to;
				break;
			case BISHOP:
				bd->ep = 0;
				bd->BlackBishops ^= from;
				bd->BlackBishops |= to;
				break;
			case KNIGHT:
				bd->ep = 0;
				bd->BlackKnights ^= from;
				bd->BlackKnights |= to;
				break;
			case PAWN:
				if(to & from>>16) {
					bd->ep = bitScanForward(from>>8); 
				} else {
					bd->ep = 0;
				}
				bd->BlackPawns ^= from;
				bd->BlackPawns |= to;
				break;
			default:
				printf("ERROR: Piece to move, type unknown.");
				break;
		}

		if(to & bd->WhitePieces) {
			bd->WhitePieces ^= to;
			if(to & bd->WhiteQueens) {
				bd->WhiteQueens ^= to;	
			} else if(to & bd->WhiteRooks) {
				bd->WhiteRooks ^= to;
			} else if(to & bd->WhiteBishops) {
				bd->WhiteBishops ^= to;
			} else if(to & bd->WhiteKnights) {
				bd->WhiteKnights ^= to;
			} else if(to & bd->WhitePawns) {
				bd->WhitePawns ^= to;
			}
		}

		// white piece captured
		bd->BlackPieces ^= from;
		bd->BlackPieces |= to;
	}

	bd->AllPieces ^= from;
	bd->AllPieces |= to;
}
