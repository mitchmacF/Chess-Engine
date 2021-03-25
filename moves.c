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

/* Find constant time version of this function */
U64 bitScanForward(U64 bb) {
	int i = 1; assert(bb != 0);
	U64 sq = 1ULL;
	while(!(bb & sq)) {
		sq = 1ULL<<i;
		i++;
	}

	return i-1;
}

U64 Pop(U64 bb) {
	U64 sq_bit_mask = getPosition(bitScanForward(bb));
	return bb^sq_bit_mask;
}

U64 getPosition(Square sq) {
	return 1ULL<<sq;
}

// this is the place where you only update board parameters -> if you have to do more then maybe reconsider move gen functions
bool make_move(Move mv) {
	// copy(to, from)
	copy(undo_bd, bd);

	U64 from = (U64)((mv.mv & (0x3f)) + 1);
	U64 to = (U64)(((mv.mv & (0x3f<<6))>>6) + 1);
	U64 flag = (U64)((mv.mv & (0x03<<14))>>14);
	Piece pc_to_move = mv.pc;
	
	switch(flag) {
		// attacking or quiet move
		case 0:
			update_bb(pc_to_move, to, from);
			break;
		// promotion -> Pawn to Queen is default for now
		case 1:
			if(bd->to_move == WHITE) {
				bd->WhitePawns ^= from;
				bd->WhiteQueens |= to;
			} else if(bd->to_move == BLACK) {
				bd->BlackPawns ^= from;
				bd->BlackQueens |= to;
			}
			break;
		// castle -> turn off castle flag in bd
		case 3:
			if(bd->to_move == WHITE) {
				bd->WhiteKing ^= from;
				bd->WhiteKing |= to;
				// King-side
				if(to & (1ULL<<6)) {
					bd->WhiteRooks ^= (1ULL<<7);
					bd->WhiteRooks |= (1ULL<<5);
					bd->castle[0] = '-';
				// Queen-side
				} else if(to & (1ULL<<2)) {
					bd->WhiteRooks ^= (1ULL);
					bd->WhiteRooks |= (1ULL<<3);
					bd->castle[1] = '-';
				}
			} else if(bd->to_move == BLACK) {
				bd->BlackKing ^= from;
				bd->BlackKing |= to;
				// King-side
				if(to & (1ULL<<62)) {
					bd->BlackRooks ^= (1ULL<<63);
					bd->BlackRooks |= (1ULL<<61);
					bd->castle[2] = '-';
				// Queen-side
				} else if(to & (1ULL<<58)) {
					bd->BlackRooks ^= (1ULL<<57);
					bd->BlackRooks |= (1ULL<<59);
					bd->castle[3] = '-';
				}
			}
			break;
		default:
			printf("ERROR: flag\n");	
	}
	// set en pas flag off
	bd->ep = 0ULL;
	
	if(bd->to_move == WHITE) {
		if(attacked(bd->WhiteKing, bd->AllPieces, bd->WhitePieces)) {
			copy(bd, undo_bd);
			return 0;
		}
	} else if(bd->to_move = BLACK) {
		if(attacked(bd->BlackKing, bd->AllPieces, bd->BlackPieces)) {
			copy(bd, undo_bd);
			return 0;
		}
	}
	
	return 1;
}

void update_bb(Piece pc, U64 to, U64 from) {

	if(bd->to_move == WHITE) {
		switch(pc) {
			case KING:
				bd->WhiteKing ^= from;
				bd->WhiteKing |= to;
				break;
			case QUEEN:
				bd->WhiteQueens ^= from;
				bd->WhiteQueens |= to;
				break;
			case ROOK:
				bd->WhiteRooks ^= from;
				bd->WhiteRooks |= to;
				break;
			case BISHOP:
				bd->WhiteBishops ^= from;
				bd->WhiteBishops |= to;
				break;
			case KNIGHT:
				bd->WhiteKnights ^= from;
				bd->WhiteKnights |= to;
				break;
			case PAWN:
				bd->WhitePawns ^= from;
				bd->WhitePawns |= to;
				break;
			default:
				printf("ERROR: Piece to move, type unknown.");
				break;
		}
		
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
				bd->BlackKing ^= from;
				bd->BlackKing |= to;
				break;
			case QUEEN:
				bd->BlackQueens ^= from;
				bd->BlackQueens |= to;
				break;
			case ROOK:
				bd->BlackRooks ^= from;
				bd->BlackRooks |= to;
				break;
			case BISHOP:
				bd->BlackBishops ^= from;
				bd->BlackBishops |= to;
				break;
			case KNIGHT:
				bd->BlackKnights ^= from;
				bd->BlackKnights |= to;
				break;
			case PAWN:
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

		bd->BlackPieces ^= from;
		bd->BlackPieces |= to;
	}

	bd->AllPieces ^= from;
	bd->AllPieces |= to;
}

// move: 16 bits
// bits  0- 5: origin (0 - 63)
// bits  6-11: destination (0 - 63)
// bits 12-13: promotion piece type -> see enum Pieces
// bits 14-15: special move flag: promotion (1), en passant (2), castling (3)
void fill_move_list(U64 moves, unsigned int from, unsigned int promotion, unsigned int flag, Piece p) {
	U64 current_move, from_U64;
	unsigned int to;

	//printboard(moves);
	//printf("\n");
	while(moves) {
		to = U64toIDX(moves);
		from_U64 = IDXtoU64((Square)from);

		// check for promotion
		if((to & tbls->MaskRank[RANK_1] || to & tbls->MaskRank[RANK_8]) && (from_U64 & bd->WhitePawns || from_U64 & bd->BlackPawns)) {
			flag |= 0x01;
		} 

		current_move = 
			((from      & 	0x3f)		) |
			((to        & 	0x3f) << 6 	) |
			((promotion & 	0x03) << 12	) | 
			((flag      & 	0x03) << 14	);      

		Move mv = {current_move, 0, p};

		// store move in move list
		mv_list->moves[mv_list->total_count] = mv;
		mv_list->total_count++;

		moves = Pop(moves);
	}
}

void genMoves(U64 (*f)(U64, U64, U64), U64 pieces, U64 all_pieces, U64 side, Piece p) {
	U64 piece, moves, from, ep_attackers;
	
	while(pieces) {
		piece = IDXtoU64(U64toIDX(pieces));
		moves = (*f)(piece, all_pieces, side);
		if(moves) {
			from = U64toIDX(pieces);
			fill_move_list(moves, from, 0x00, 0x00, p);
		} 
		pieces = Pop(pieces);
	}
}

// check for errors
void genSpecialMoves() {
	U64 pieces, piece, moves, from, ep_attackers;
	
	// en passant -> enPas() returns locations of pawns that can capture ep square
	ep_attackers = enPas();
	while(ep_attackers) {
		from = U64toIDX(ep_attackers);
		fill_move_list(bd->ep, from, 0x00, 0x02, PAWN);
		ep_attackers = Pop(ep_attackers);
	} 
	
	// castling -> gen_castle_moves return locations (king or queen side) that king can move
	if(bd->to_move == WHITE) { 
		moves = generate_castling_moves(bd->WhiteKing, bd->AllPieces, bd->WhitePieces);
		from = bd->WhiteKing;
	} else {
		moves = generate_castling_moves(bd->BlackKing, bd->AllPieces, bd->BlackPieces); 
		from = bd->BlackKing;
	}
	while(moves) {
		U64 to = U64toIDX(moves);
		fill_move_list(to, from, 0x00, 0x03, KING);
		moves = Pop(moves);
	} 
}

// need to change to only generate moves for side who's turn it is
void generateAllMoves(Side side_to_move) {
	U64 (*fn_ptr)(U64, U64, U64);
	genSpecialMoves();
	
	if(side_to_move == WHITE) {
		
		fn_ptr = &generate_white_pawn_moves;
		genMoves(fn_ptr, bd->WhitePawns, bd->AllPieces, bd->WhitePieces, PAWN);

		fn_ptr = &generate_king_moves;
		genMoves(fn_ptr, bd->WhiteKing, bd->AllPieces, bd->WhitePieces, KING);

		fn_ptr = &generate_queen_moves;
		genMoves(fn_ptr, bd->WhiteQueens, bd->AllPieces, bd->WhitePieces, QUEEN);

		fn_ptr = &generate_bishop_moves;
		genMoves(fn_ptr, bd->WhiteBishops, bd->AllPieces, bd->WhitePieces, BISHOP);

		fn_ptr = &generate_knight_moves;
		genMoves(fn_ptr, bd->WhiteKnights, bd->AllPieces, bd->WhitePieces, KNIGHT);

		fn_ptr = &generate_rook_moves;
		genMoves(fn_ptr, bd->WhiteRooks, bd->AllPieces, bd->WhitePieces, ROOK);

	} else if (side_to_move == BLACK) {
		
		fn_ptr = &generate_black_pawn_moves;
		genMoves(fn_ptr, bd->BlackPawns, bd->AllPieces, bd->BlackPieces, PAWN);

		fn_ptr = &generate_king_moves;
		genMoves(fn_ptr, bd->BlackKing, bd->AllPieces, bd->BlackPieces, KING);

		fn_ptr = &generate_queen_moves;
		genMoves(fn_ptr, bd->BlackQueens, bd->AllPieces, bd->BlackPieces, QUEEN);

		fn_ptr = &generate_bishop_moves;
		genMoves(fn_ptr, bd->BlackBishops, bd->AllPieces, bd->BlackPieces, BISHOP);

		fn_ptr = &generate_knight_moves;
		genMoves(fn_ptr, bd->BlackKnights, bd->AllPieces, bd->BlackPieces, KNIGHT);

		fn_ptr = &generate_rook_moves;
		genMoves(fn_ptr, bd->BlackRooks, bd->AllPieces, bd->BlackPieces, ROOK);

	} else {
		printf("Error: side_to_move\n");
	}
}

U64 flip(U64 x) {
    return  ((x << 56)) | ((x << 40) & (0x00ff000000000000)) | ((x << 24) & (0x0000ff0000000000)) | ((x << 8) & (0x000000ff00000000)) | ((x >> 8) & (0x00000000ff000000)) | ((x >> 24) & (0x0000000000ff0000)) | ((x >> 40) & (0x000000000000ff00)) | ((x >> 56));
}

U64 generate_castling_moves(U64 king_location, U64 all_pieces, U64 side) {
	U64 opp_side, castle_king_side = 0ULL, castle_queen_side = 0ULL;
	bool king, queen = 1; 

	// King side 
	if((king_location<<1 & side) || (king_location<<2 & side) || attacked(king_location, all_pieces, side) || attacked(king_location<<1, all_pieces, side) || attacked(king_location<<2, all_pieces, side)) {
		king = 0;
	}

	// Queen side
	if((king_location>>1 & side) || (king_location>>2 & side) || attacked(king_location, all_pieces, side) || attacked(king_location>>1, all_pieces, side) || attacked(king_location>>2, all_pieces, side)) {
		queen = 0;
	}
	
	if(side & bd->WhitePieces) {
		if(bd->castle[0] != '\0' && king) {
				castle_king_side = 1ULL<<6;
		}
		if(bd->castle[1] != '\0' && queen) {
				castle_queen_side = 1ULL<<2;
		}
	} else {
		if(bd->castle[2] != '\0' && king) {
				castle_king_side = 1ULL<<62;
		}
		if(bd->castle[3] != '\0' && queen) {
				castle_queen_side = 1ULL<<58;
		}
	}
	
	return (castle_king_side | castle_queen_side);
}

U64 enPas() {
	if(!bd->ep) 
		return 0ULL;
	
	U64 ep_sq = IDXtoU64(bd->ep);
	U64 pawn_attackers = 0ULL;

	if(bd->to_move == WHITE) { 
		if((ep_sq>>7 & bd->WhitePawns)) {
			pawn_attackers |= ep_sq>>7;
		}
		if((ep_sq>>9 & bd->WhitePawns)) {
			pawn_attackers |= ep_sq>>9;
		}
	} else if(bd->to_move == BLACK) {
		if(ep_sq<<7 & bd->BlackPawns) {
			pawn_attackers |= ep_sq<<7;
		}
		if(ep_sq<<9 & bd->BlackPawns) {
			pawn_attackers |= ep_sq<<9;
		}
	}
	return pawn_attackers;
}

// use bitset for debugging 
// left shift goes "up" the board from LSB towards MSB
// (remember LSB is 0 or A1)
// right shift goes "down" the board from MSB towards LSB
U64 generate_king_moves(U64 current_king_location, U64 all_pieces, U64 side) {
	
	U64 king_clear_a_file = current_king_location & tbls->ClearFile[FILE_A];
	U64 king_clear_h_file = current_king_location & tbls->ClearFile[FILE_H];
	
	U64 move_3 = king_clear_h_file << 9; 
	U64 move_4 = king_clear_h_file << 1; 
	U64 move_5 = king_clear_h_file >> 7; 
	
	U64 move_6 = current_king_location >> 8; 
	U64 move_2 = current_king_location << 8; 
	
	U64 move_1 = king_clear_a_file << 7; 
	U64 move_8 = king_clear_a_file >> 1; 
	U64 move_7 = king_clear_a_file >> 9; 

	U64 all_king_moves = move_1 | move_2 | move_3 | move_4 | move_5 | move_6 | move_7 | move_8;

	U64 legal_king_moves = all_king_moves & ~side;

	return legal_king_moves;
}

U64 generate_knight_moves(U64 current_knight_location, U64 all_pieces, U64 side) {
	
	U64 knight_clear_a_file = current_knight_location & tbls->ClearFile[FILE_A]; 
	U64 knight_clear_a_and_b_file = current_knight_location & (tbls->ClearFile[FILE_A] & tbls->ClearFile[FILE_B]); 
	U64 knight_clear_h_file = current_knight_location & tbls->ClearFile[FILE_H]; 
	U64 knight_clear_h_and_g_file = current_knight_location & (tbls->ClearFile[FILE_H] & tbls->ClearFile[FILE_G]);

	U64 move_2 = knight_clear_a_file << 15;
	U64 move_7 = knight_clear_a_file >> 17;
	U64 move_1 = knight_clear_a_and_b_file << 6;
	U64 move_8 = knight_clear_a_and_b_file >> 10;

	U64 move_3 = knight_clear_h_file << 17;
	U64 move_6 = knight_clear_h_file >> 15;
	U64 move_4 = knight_clear_h_and_g_file << 10;
	U64 move_5 = knight_clear_h_and_g_file >> 6;

	U64 all_knight_moves = move_1 | move_2 | move_3 | move_4 | move_5 | move_6 | move_7 | move_8;

	U64 legal_knight_moves = all_knight_moves & ~side;

	return legal_knight_moves;
}

U64 generate_white_pawn_moves(U64 current_white_pawn_location, U64 all_pieces, U64 side) {

	U64 black_pieces = all_pieces & ~side; 
	
	U64 one_rank_forward = (current_white_pawn_location << 8) & ~all_pieces;
	
	U64 two_ranks_forward = ((one_rank_forward & tbls->MaskRank[RANK_3]) << 8) & ~all_pieces;

	U64 pawn_forward_moves = one_rank_forward | two_ranks_forward;

	// left attack
	U64 left_pawn_attack = (current_white_pawn_location & tbls->ClearFile[FILE_A]) << 7;

	// right attack
	U64 right_pawn_attack = (current_white_pawn_location & tbls->ClearFile[FILE_H]) << 9;
	
	// all attacks valid and invalid
	U64 all_pawn_attacks = left_pawn_attack | right_pawn_attack;	

	// valid attacks
	U64 all_valid_attacks = all_pawn_attacks & black_pieces;	

	// all legal moves
	U64 all_legal_white_pawn_moves = pawn_forward_moves | all_valid_attacks;

	return all_legal_white_pawn_moves;
}

U64 generate_black_pawn_moves(U64 current_black_pawn_location, U64 all_pieces, U64 side) {

	U64 white_pieces = all_pieces & ~side;
	
	U64 one_rank_forward = (current_black_pawn_location >> 8) & ~all_pieces;
	
	U64 two_ranks_forward = ((one_rank_forward & tbls->MaskRank[RANK_6]) >> 8) & ~all_pieces;

	U64 pawn_forward_moves = one_rank_forward | two_ranks_forward;

	// left attack
	U64 left_pawn_attack = (current_black_pawn_location & tbls->ClearFile[FILE_H]) >> 7;

	// right attack
	U64 right_pawn_attack = (current_black_pawn_location & tbls->ClearFile[FILE_A]) >> 9;
	
	// all attacks valid and invalid
	U64 all_pawn_attacks = left_pawn_attack | right_pawn_attack;	

	// valid attacks
	U64 all_valid_attacks = all_pawn_attacks & white_pieces;	

	// all legal moves
	U64 all_legal_black_pawn_moves = pawn_forward_moves | all_valid_attacks;

	return all_legal_black_pawn_moves;
}

U64 generate_bishop_moves(U64 bishop_location, U64 board_state, U64 side) {
	Square sq = bitScanForward(bishop_location);
	return DiagonalAttacks(sq, board_state, side) | AntiDiagonalAttacks(sq, board_state, side);
}

U64 generate_rook_moves(U64 rook_location, U64 board_state, U64 side) {
	Square sq = bitScanForward(rook_location);
	U64 opp_side = board_state & ~side;
	return FileAttacks(sq, board_state, side) | RankAttacks(sq, side, opp_side);
}

U64 generate_queen_moves(U64 queen_location, U64 board_state, U64 side) {
	Square sq = bitScanForward(queen_location);
	U64 opp_side = board_state & ~side;
	return FileAttacks(sq, board_state, side) | RankAttacks(sq, side, opp_side) | DiagonalAttacks(sq, board_state, side) | AntiDiagonalAttacks(sq, board_state, side);
}

// super_piece & opp_side will not work 
bool attacked(U64 piece_location, U64 board_state, U64 side) {
	Square sq = bitScanForward(piece_location);
	U64 square_position = getPosition(sq);
	
	U64 opp_side = ~side;

	U64 super_piece = generate_queen_moves(sq, board_state, side) | generate_knight_moves(square_position, board_state, side);

	if(side & bd->WhitePieces) {
		if(super_piece & (bd->WhitePawns | bd->WhiteRooks | bd->WhiteKnights | bd->WhiteBishops | bd->WhiteKing | bd->WhiteQueens))
			return true;
		else 
			return false;
	} else {
		if(super_piece & (bd->BlackPawns | bd->BlackRooks | bd->BlackKnights | bd->BlackBishops | bd->BlackKing | bd->BlackQueens))
			return true;
		else 
			return false;
	}
}

U64 FileAttacks(Square sq, U64 board_state, U64 side) {
	U64 fwd, rev;
	int file = sq%8;
	U64 sq_bit_mask = getPosition(sq);

	fwd = board_state & tbls->MaskFile[file];
	rev = flip(fwd);

	U64 rev_sq_bit_mask = flip(sq_bit_mask);
	
	fwd -= sq_bit_mask<<1;
	rev -= rev_sq_bit_mask<<1;

	fwd ^= flip(rev); 
	fwd &= tbls->MaskFile[file];
	return (fwd & ~side);
}

U64 DiagonalAttacks(Square sq, U64 board_state, U64 side) {
	U64 fwd, rev;
	int file = sq%8;
	int rank = sq/8;
	U64 sq_bit_mask = getPosition(sq);
	U64 diag_line_mask = tbls->DiagonalMask[(7-(rank-file))];

	fwd = board_state & diag_line_mask; 
	rev = flip(fwd);

	U64 rev_sq_bit_mask = flip(sq_bit_mask);
	
	fwd -= sq_bit_mask<<1;
	rev -= rev_sq_bit_mask<<1;

	fwd ^= flip(rev); 
	fwd &= diag_line_mask; 
	return (fwd & ~side);
}

U64 AntiDiagonalAttacks(Square sq, U64 board_state, U64 side) {
	U64 fwd, rev;
	int file = sq%8;
	int rank = 7-(sq/8);
	U64 sq_bit_mask = getPosition(sq);
	U64 anti_diag_line_mask = tbls->AntiDiagonalMask[(7-(file-rank))];

	fwd = board_state & anti_diag_line_mask; 
	rev = flip(fwd);

	U64 rev_sq_bit_mask = flip(sq_bit_mask);
	
	fwd -= sq_bit_mask<<1;
	rev -= rev_sq_bit_mask<<1;

	fwd ^= flip(rev); 
	fwd &= anti_diag_line_mask; 
	return (fwd & ~side);
}

U64 RankAttacks(Square sq, U64 side, U64 opposing_side) {

	int i, j, rank = sq/8;
	int file = sq%8;

	// attacking piece bit mask and temp variable that we'll use in for loop
	const U64 bit_mask = getPosition(sq);
	U64 temp = bit_mask; 

	// get rank mask
	U64 side_rank_mask = tbls->MaskRank[rank];
	U64 opp_side_rank_mask = side_rank_mask;
	U64 rank_attacks = side_rank_mask; 

	// flip square attacking piece is on
	rank_attacks &= ~bit_mask;

	// get only pieces on rank that belong to attacking piece's side
	side_rank_mask &= side;
	side_rank_mask &= ~bit_mask;

	// get only pieces on rank that belong to opposing side
	opp_side_rank_mask &= opposing_side;

	// 3 cases: 
	// unoccupied sq
	// own side occupies sq
	// opposite side occupies sq
	for(i=file+1; i<8; i++) {
		temp = temp<<1;
		// if we run into our own piece 0 out that square and rest of squares
		if(temp & side_rank_mask) {
			for(j=i; j<8; j++) {
				rank_attacks &= ~temp;
				temp = temp<<1;
			}
			break;
		// if we run into opposing piece  0 out not that square and rest of squares
		} else if(temp & opp_side_rank_mask) {
			for(j=i; j<8; j++) {
				if(j==i) {
					temp = temp<<1;
					continue;
				}
				rank_attacks &= ~temp;
				temp = temp<<1;
			}
			break;
		// empty square
		} 
	}
	
	temp = bit_mask; 
	for(i=file-1; i>=0; i--) {
		temp = temp>>1;
		// if we run into our own piece 0 out that square and rest of squares
		if(temp & side_rank_mask) {
			for(j=i; j>=0; j--) {
				rank_attacks &= ~temp;
				temp = temp>>1;
			}
			break;
		// if we run into opposing piece  0 out not that square and rest of squares
		} else if(temp & opp_side_rank_mask) {
			for(j=i; j>=0; j--) {
				if(j==i) {
					temp = temp>>1;
					continue;
				}
				rank_attacks &= ~temp;
				temp = temp>>1;
			}
			break;
		// empty square
		} 
	}

	return rank_attacks;
}
