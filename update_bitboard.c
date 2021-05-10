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

bool captured;

void undo_move(struct Board *to, struct Board *from) {
	copy(to, from);
}

// this is the place where you only update board parameters -> if you have to do more then maybe reconsider move gen functions
bool make_move(Move mv) {
	copy(undo_bd, bd);
	captured = false;

	int from, to, flag, promotion;
	U64 from_U64, to_U64, ep_location;
	Piece pc_to_move;

	// convert mv to bitboard compatible types
	from = (U64)((mv.mv & (0x3f)));
	from_U64 = IDXtoU64(from);
	to = (U64)(((mv.mv & (0x3f<<6))>>6));
	to_U64 = IDXtoU64(to);
	flag = (U64)((mv.mv & (0x03<<14))>>14);
	promotion = ((mv.mv & (0x03<<12))>>12);
	
	pc_to_move = mv.pc;

	bd->half_move++;
	
	switch(flag) {
		// attacking or quiet move
		case 0:
			update_bb(pc_to_move, to_U64, from_U64);
			break;
		// promotion 
		case 1:
			bd->half_move = 0;
			update_bb_promotion(pc_to_move, to_U64, from_U64, promotion);
			break;
		// en Pas
		case 2:
			bd->half_move = 0;
			ep_location = IDXtoU64(bd->ep);
			update_bb_enPas(pc_to_move, ep_location, from_U64);
			break;
		// castle
		case 3:
			update_bb_castle(pc_to_move, to_U64, from_U64);
			break;
		default:
			fprintf(stderr, "ERROR: flag\n");	
			exit(0);
	}

	// check if move leaves King in check
	if(bd->to_move == WHITE) {
		if(attacked(bd->WhiteKing, bd->AllPieces, bd->WhitePieces)) {
			//copy(bd, undo_bd);
			return 0;
		}
	} else if(bd->to_move == BLACK) {
		if(attacked(bd->BlackKing, bd->AllPieces, bd->BlackPieces)) {
			//copy(bd, undo_bd);
			return 0;
		}
	}
	
	// check if move leaves King in check
	if(bd->to_move == WHITE) {
		if(attacked(bd->BlackKing, bd->AllPieces, bd->BlackPieces)) {
			checks++;
		}
	} else if(bd->to_move == BLACK) {
		if(attacked(bd->WhiteKing, bd->AllPieces, bd->WhitePieces)) {
			checks++;
		}
	}

	switch(flag) {
		// attacking or quiet move
		case 0:
			if(captured == true) {
				captures++;
			}
			break;
		// promotion 
		case 1:
			if(captured == true) {
				captures++;
			}
			promotions++;
			break;
		// en Pas
		case 2:
			captures++;
			ep++;
			break;
		// castle
		case 3:
			castles++;
			break;
		default:
			fprintf(stderr, "ERROR: flag\n");	
			exit(-1);
	}

	// reset move list
	//mv_list->total_count = 0;
	if(bd->to_move == WHITE) {
		bd->to_move = BLACK;
	} else if(bd->to_move == BLACK) {
		bd->to_move = WHITE;
		bd->whole_move++;
	} else {
		printf("ERROR: to_move error\n");
		exit(0);
	}
	
	return 1;
}

void update_bb_promotion(Piece pc, U64 to, U64 from, int promotion) {
		if(bd->to_move == WHITE) {
			if(promotion+2 == QUEEN) {
				bd->WhiteQueens |= to;
			} else if(promotion+2 == ROOK) {
				bd->WhiteRooks |= to;
			} else if(promotion+2 == BISHOP) {
				bd->WhiteBishops |= to;
			} else if(promotion+2 == KNIGHT) {
				bd->WhiteKnights |= to;
			} else {
				printf("ERROR in promotion\n");
				printf("Promotion = %d\n", promotion);
				exit(0);
			}
		
			// black piece capture
			if(to & bd->BlackPieces) {
				captured = true;
				bd->BlackPieces ^= to;
				if(to & bd->BlackQueens) {
					bd->BlackQueens ^= to;	
				} else if(to & bd->BlackRooks) {
					// Remove king-side permissions
					if(to & (1ULL<<63)) {
						bd->castle[2] = '-';
						// Queen-side
					} else if(to & (1ULL<<56)) {
						bd->castle[3] = '-';
					}
					bd->BlackRooks ^= to;
				} else if(to & bd->BlackBishops) {
					bd->BlackBishops ^= to;
				} else if(to & bd->BlackKnights) {
					bd->BlackKnights ^= to;
				} else if(to & bd->BlackPawns) {
					printf("BIG ERROR\n");
					exit(-1);
					bd->BlackPawns ^= to;
				}
			}
			bd->WhitePawns ^= from;
			bd->WhitePieces ^= from;
			bd->WhitePieces |= to;
		} else if(bd->to_move == BLACK) {
			if(promotion+2 == QUEEN) {
				bd->BlackQueens |= to;
			} else if(promotion+2 == ROOK) {
				bd->BlackRooks |= to;
			} else if(promotion+2 == BISHOP) {
				bd->BlackBishops |= to;
			} else if(promotion+2 == KNIGHT) {
				bd->BlackKnights |= to;
			} else {
				printf("ERROR in promotion\n");
				exit(0);
			}
			
			if(to & bd->WhitePieces) {
				captured = true;
				bd->WhitePieces ^= to;
				if(to & bd->WhiteQueens) {
					bd->WhiteQueens ^= to;	
				} else if(to & bd->WhiteRooks) {
					// Remove king-side permissions
					if(to & (1ULL<<7)) {
						bd->castle[0] = '-';
						// Queen-side
					} else if(to & (1ULL)) {
						bd->castle[1] = '-';
					} 
					bd->WhiteRooks ^= to;
				} else if(to & bd->WhiteBishops) {
					bd->WhiteBishops ^= to;
				} else if(to & bd->WhiteKnights) {
					bd->WhiteKnights ^= to;
				} else if(to & bd->WhitePawns) {
					printf("BIG ERROR\n");
					exit(-1);
					bd->WhitePawns ^= to;
				}
			}
			bd->BlackPawns ^= from;
			bd->BlackPieces ^= from;
			bd->BlackPieces |= to;
		}
		bd->AllPieces ^= from;
		bd->AllPieces |= to;
		bd->ep = 0;
}

void update_bb_enPas(Piece pc, U64 to, U64 from) {
	//printf(">>>>>>>>> EP \n");
	
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
				bd->AllPieces ^= (to>>8);
		} else {
			printf(">>>> 1 EP ERROR\n");
			exit(-1);
		}
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
			bd->AllPieces ^= (to<<8);
		} else {
			printf(">>>> EP ERROR\n");
			exit(-1);
		}
	}

	bd->AllPieces ^= from;
	bd->AllPieces |= to;

	bd->ep = 0;
}

void update_bb_castle(Piece pc, U64 to, U64 from) {
	if(bd->to_move == WHITE) {
		//printf(">>>>>>>>>>>>>>>>>>>> WHITE CASTLING\n");
		// King-side
		if(to & (1ULL<<6)) {

			if(!(bd->WhiteRooks & (1ULL<<7)) || bd->castle[0] == '-') {
				printf("ERROR in CASTLING FUNCTION\n");
				exit(-1);
			}

			bd->WhiteRooks ^= (1ULL<<7);
			bd->WhitePieces ^= (1ULL<<7);
			bd->AllPieces ^= (1ULL<<7);

			bd->WhiteRooks |= (1ULL<<5);
			bd->WhitePieces |= (1ULL<<5);
			bd->AllPieces |= (1ULL<<5);
			// Queen-side
		} else if(to & (1ULL<<2)) {
			
			if(!(bd->WhiteRooks & (1ULL)) || bd->castle[1] == '-') {
				printf("ERROR in CASTLING FUNCTION\n");
				exit(-1);
			}

			bd->WhiteRooks ^= (1ULL);
			bd->WhitePieces ^= (1ULL);
			bd->AllPieces ^= (1ULL);

			bd->WhiteRooks |= (1ULL<<3);
			bd->WhitePieces |= (1ULL<<3);
			bd->AllPieces |= (1ULL<<3);
		} else {
			printf("ERROR in CASTLING FUNCTION\n");
			exit(-1);
		}
		bd->WhiteKing ^= from;
		bd->WhiteKing |= to;

		// Update White King Location
		bd->WhitePieces ^= from;
		bd->WhitePieces |= to;
		
		bd->castle[0] = '-';
		bd->castle[1] = '-';
	} else if(bd->to_move == BLACK) {
		//printf(">>>>>>>>>>>>>>>>>>>> BLACK CASTLING\n");
		// King-side
		if(to & (1ULL<<62)) {
			
			if(!(bd->BlackRooks & (1ULL<<63)) || bd->castle[2] == '-') {
				printf("ERROR in CASTLING FUNCTION\n");
				exit(-1);
			}

			bd->BlackRooks ^= (1ULL<<63);
			bd->BlackPieces ^= (1ULL<<63);
			bd->AllPieces ^= (1ULL<<63);

			bd->BlackRooks |= (1ULL<<61);
			bd->BlackPieces |= (1ULL<<61);
			bd->AllPieces |= (1ULL<<61);
			// Queen-side
		} else if(to & (1ULL<<58)) {
			
			if(!(bd->BlackRooks & (1ULL<<56)) || bd->castle[3] == '-') {
				printf("ERROR in CASTLING FUNCTION\n");
				exit(-1);
			}

			bd->BlackRooks ^= (1ULL<<56);
			bd->BlackPieces ^= (1ULL<<56);
			bd->AllPieces ^= (1ULL<<56);

			bd->BlackRooks |= (1ULL<<59);
			bd->BlackPieces |= (1ULL<<59);
			bd->AllPieces |= (1ULL<<59);
		} else {
			printf("ERROR in CASTLING FUNCTION\n");
			exit(-1);
		}
		bd->BlackKing ^= from;
		bd->BlackKing |= to;

		// Update Black King Location
		bd->BlackPieces ^= from;
		bd->BlackPieces |= to;

		bd->castle[2] = '-';
		bd->castle[3] = '-';
	}
	// Update King Location on AllPieces bb
	bd->AllPieces ^= from;
	bd->AllPieces |= to;
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
				if(from & 1ULL) {
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
				bd->half_move = 0;
				break;
			default:
				printf("ERROR: Piece to move, type unknown: %d\n", pc);
				exit(-1);
				//break;
		}
		
		// black piece capture
		if(to & bd->BlackPieces) {
			captured = true;
			bd->half_move = 0;
			bd->BlackPieces ^= to;
			if(to & bd->BlackQueens) {
				bd->BlackQueens ^= to;	
			// Need logic for removing castling permissions here 
			} else if(to & bd->BlackRooks) {
				// Remove king-side permissions
				if(to & (1ULL<<63)) {
					bd->castle[2] = '-';
				// Queen-side
				} else if(to & (1ULL<<56)) {
					bd->castle[3] = '-';
				}
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
				bd->half_move = 0;
				break;
			default:
				printf("ERROR: Piece to move, type unknown: %d\n", pc);
				exit(-1);
				//break;
		}

		if(to & bd->WhitePieces) {
			captured = true;
			bd->half_move = 0;
			bd->WhitePieces ^= to;
			if(to & bd->WhiteQueens) {
				bd->WhiteQueens ^= to;	
			} else if(to & bd->WhiteRooks) {
				// Remove king-side permissions
				if(to & (1ULL<<7)) {
					bd->castle[0] = '-';
				// Queen-side
				} else if(to & (1ULL)) {
					bd->castle[1] = '-';
				} 
				bd->WhiteRooks ^= to;
			} else if(to & bd->WhiteBishops) {
				bd->WhiteBishops ^= to;
			} else if(to & bd->WhiteKnights) {
				bd->WhiteKnights ^= to;
			} else if(to & bd->WhitePawns) {
				bd->WhitePawns ^= to;
			}
		}

		// white piece capture
		bd->BlackPieces ^= from;
		bd->BlackPieces |= to;
	}

	bd->AllPieces ^= from;
	bd->AllPieces |= to;
}
