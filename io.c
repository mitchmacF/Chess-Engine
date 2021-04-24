#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "extern.h"

#define rank_file_idx(r, f) ( ((r-1) * 8) + (f-1) )

struct Move_list *mv_list;
void printMoveList(struct Move_list *mv_list) {
	int i, curr_mv, score, flag; 
	char *from, *to, *piece;
	Piece pc;

	printf("Total Move List Count: %d\n", mv_list->total_count);
	
	for(i = 0; i < mv_list->total_count; i++) {
		curr_mv = mv_list->moves[i].mv;
		score = mv_list->moves[i].mv_score;
		pc = mv_list->moves[i].pc;

		from = move_notation[ (curr_mv & (0x3f))    ];
		to =   move_notation[ (curr_mv & (0x3f<<6))>>6 ];
		flag = (curr_mv & (0x03<<14))>>14;

		switch(pc) {
			case 1: 
				piece = "King"; 
				break;
			case 2: 
				piece = "Queen";
				break;
			case 3: 
				piece = "Rook";
				break;
			case 4: 
				piece = "Bishop";
				break;
			case 5: 
				piece = "Knight";
				break;
			case 6: 
				piece = "Pawn";
				break;
			default:
				printf("Error -> no piece detected");
				break;
		}

		switch(flag) {
			case 0:
				printf("%d: %s%s %s    ", i, from, to, piece);
				break;
			case 1:
				printf("%d: %s%s PROMOTION    ", i, from, to);
				break;
			case 2: 
				printf("%d: %s%s EN PAS    ", i, from, to);
				break;
			case 3: 
				printf("%d: %s%s CASTLE    ", i, from, to);
				break;
			default:
				printf("Move flag not set!!\n");
				break;
		}

	}
}

void printMove(Move mv) {
	char *from, *to, *piece;
	int curr_mv, pc; 

	curr_mv = mv.mv;
	pc = mv.pc;

	from = move_notation[ (curr_mv & (0x3f))    ];
	to =   move_notation[ (curr_mv & (0x3f<<6))>>6 ];

	switch(pc) {
		case 1: 
			piece = "King"; 
			break;
		case 2: 
			piece = "Queen";
			break;
		case 3: 
			piece = "Rook";
			break;
		case 4: 
			piece = "Bishop";
			break;
		case 5: 
			piece = "Knight";
			break;
		case 6: 
			piece = "Pawn";
			break;
		default:
			printf("Error -> no piece detected");
			break;
	}

	printf("%s%s %s\n", from, to, piece);
}

void printboard(U64 n){
	char bits[64];
    U64 i; 
	int j, k, index;

	i = 1ULL<<63;
	j = 0, index = 63;

	while(i>0){
         if(n&i)
         	bits[index] = '1';
         else 
         	bits[index] = '.';
         i >>= 1;
	
		 index--;
    }

	for(k=63; k>=0; k-=8) {
		for(j=k-7; j<=k; j++) {
			printf("%c ", bits[j]);
		}
		printf("\n");
	}
	printf("\n");
}

void printCharBoard() {
	fillCharBoard();
	int rank = 8;
	for(int j = 56; j>=0; j-=8) {
		for(int k = j; k<j+8; k++) {
			if(k == j) {
				printf("%d ", rank);
				rank--;
			}
			printf("%c ", board[k]);   
		}
		if((j) % 8 == 0) {
			printf("\n");
		}
	}
	printf("  a b c d e f g h\n");
	
	printf("\n");
	if(bd->to_move == 0) 
		printf("White\n");
	else if(bd->to_move ==1)
		printf("Black\n");

	for(int k = 0; k < 4; k++) {
		printf("%c", bd->castle[k]);
	}
	printf("\n");
	printf("ep: %llu\n", bd->ep);
	printf("half_move: %d\n", bd->half_move);
	//printf("%d %d\n\n", bd->half_move, bd->whole_move);
}

void printUndoCharBoard() {
	fillCharBoard();
	int rank = 8;
	for(int j = 56; j>=0; j-=8) {
		for(int k = j; k<j+8; k++) {
			if(k == j) {
				printf("%d ", rank);
				rank--;
			}
			printf("%c ", board[k]);   
		}
		if((j) % 8 == 0) {
			printf("\n");
		}
	}
	printf("  a b c d e f g h\n");
	
	printf("\n");
	if(undo_bd->to_move) 
		printf("White\n");
	else
		printf("Black\n");

	for(int k = 0; k < 4; k++) {
		printf("%c", undo_bd->castle[k]);
	}
	printf("\n");
	printf("%llu\n", undo_bd->ep);
	//printf("%d %d\n\n", bd->half_move, bd->whole_move);
}

void fillCharBoard() {
	U64 loc;
	memset(board, '.', 64*sizeof(char));
	
	/* White Pieces */
	U64 w_pawn = bd->WhitePawns;
	while(w_pawn) {
		loc = bitScanForward(w_pawn);
		board[loc] = 'P'; 
		w_pawn = Pop(w_pawn);
	}

	U64 w_rook = bd->WhiteRooks;
	while(w_rook) {
		loc = bitScanForward(w_rook);
		board[loc] = 'R'; 
		w_rook = Pop(w_rook);
	}

	U64 w_knight = bd->WhiteKnights;
	while(w_knight) {
		loc = bitScanForward(w_knight);
		board[loc] = 'N'; 
		w_knight = Pop(w_knight);
	}

	U64 w_bishop = bd->WhiteBishops;
	while(w_bishop) {
		loc = bitScanForward(w_bishop);
		board[loc] = 'B'; 
		w_bishop = Pop(w_bishop);
	}

	U64 w_queen = bd->WhiteQueens;
	while(w_queen) {
		loc = bitScanForward(w_queen);
		board[loc] = 'Q'; 
		w_queen = Pop(w_queen);
	}

	U64 w_king = bd->WhiteKing;
	while(w_king) {
		loc = bitScanForward(w_king);
		board[loc] = 'K'; 
		w_king = Pop(w_king);
	}
	
	/* Black Pieces */
	U64 b_pawn = bd->BlackPawns;
	while(b_pawn) {
		loc = bitScanForward(b_pawn);
		board[loc] = 'p'; 
		b_pawn = Pop(b_pawn);
	}

	U64 b_rook = bd->BlackRooks;
	while(b_rook) {
		loc = bitScanForward(b_rook);
		board[loc] = 'r'; 
		b_rook = Pop(b_rook);
	}

	U64 b_knight = bd->BlackKnights;
	while(b_knight) {
		loc = bitScanForward(b_knight);
		board[loc] = 'n'; 
		b_knight = Pop(b_knight);
	}

	U64 b_bishop = bd->BlackBishops;
	while(b_bishop) {
		loc = bitScanForward(b_bishop);
		board[loc] = 'b'; 
		b_bishop = Pop(b_bishop);
	}

	U64 b_queen = bd->BlackQueens;
	while(b_queen) {
		loc = bitScanForward(b_queen);
		board[loc] = 'q'; 
		b_queen = Pop(b_queen);
	}

	U64 b_king = bd->BlackKing;
	while(b_king) {
		loc = bitScanForward(b_king);
		board[loc] = 'k'; 
		b_king = Pop(b_king);
	}
}

void printState() {

	printf("Printing board info for white\n");
	
	printf("pawns\n");
	printboard(bd->WhitePawns);
	printf("\n");
	
	printf("rooks\n");
	printboard(bd->WhiteRooks);
	printf("\n");
	
	printf("knights\n");
	printboard(bd->WhiteKnights);
	printf("\n");
	
	printf("bishops\n");
	printboard(bd->WhiteBishops);
	printf("\n");
	
	printf("queen\n");
	printboard(bd->WhiteQueens);
	printf("\n");
	
	printf("king\n");
	printboard(bd->WhiteKing);
	printf("\n");

	printf("all white pieces\n");
	printboard(bd->WhitePieces);
	
	printf("all white attacking\n");
	printboard(bd->WhiteAttacking);
	
	printf(">>>>>>>>>>>>>>>>>>>> Printing board info for black\n");
	
	printf("pawns\n");
	printboard(bd->BlackPawns);
	printf("\n");
	
	printf("rooks\n");
	printboard(bd->BlackRooks);
	printf("\n");
	
	printf("knights\n");
	printboard(bd->BlackKnights);
	printf("\n");
	
	printf("bishops\n");
	printboard(bd->BlackBishops);
	printf("\n");
	
	printf("queen\n");
	printboard(bd->BlackQueens);
	printf("\n");
	
	printf("king\n");
	printboard(bd->BlackKing);
	printf("\n");

	printf("all black pieces\n");
	printboard(bd->BlackPieces);
	
	printf("all black attacking\n");
	printboard(bd->BlackAttacking);

}

void printUndoState() {

	printf("Printing board info for white\n");
	
	printf("pawns\n");
	printboard(undo_bd->WhitePawns);
	printf("\n");
	
	printf("rooks\n");
	printboard(undo_bd->WhiteRooks);
	printf("\n");
	
	printf("knights\n");
	printboard(undo_bd->WhiteKnights);
	printf("\n");
	
	printf("bishops\n");
	printboard(undo_bd->WhiteBishops);
	printf("\n");
	
	printf("queen\n");
	printboard(undo_bd->WhiteQueens);
	printf("\n");
	
	printf("king\n");
	printboard(undo_bd->WhiteKing);
	printf("\n");

	printf("all white pieces\n");
	printboard(undo_bd->WhitePieces);
	
	printf("all white attacking\n");
	printboard(undo_bd->WhiteAttacking);
	
	printf(">>>>>>>>>>>>>>>>>>>> Printing board info for black\n");
	
	printf("pawns\n");
	printboard(undo_bd->BlackPawns);
	printf("\n");
	
	printf("rooks\n");
	printboard(undo_bd->BlackRooks);
	printf("\n");
	
	printf("knights\n");
	printboard(undo_bd->BlackKnights);
	printf("\n");
	
	printf("bishops\n");
	printboard(undo_bd->BlackBishops);
	printf("\n");
	
	printf("queen\n");
	printboard(undo_bd->BlackQueens);
	printf("\n");
	
	printf("king\n");
	printboard(undo_bd->BlackKing);
	printf("\n");

	printf("all black pieces\n");
	printboard(undo_bd->BlackPieces);
	
	printf("all black attacking\n");
	printboard(undo_bd->BlackAttacking);

}

// Still not filling EP square except when it's empty
void parseFEN(char *FEN) {

	int rank = 8, file = 1, empty = 0, count, i;

	while(*FEN) {
		i = rank_file_idx(rank, file);
		count = 1;

		if(empty) {
			switch(*FEN) {
				case '-':
					//if(empty == 2) {
						//bd->castle[0] = '-';
					/*} else */
					if(empty == 3) {
						bd->ep = 0ULL;
					}
					break;
				case ' ':
					empty++;
					break;
				case 'w':
					if(empty == 1) {
						bd->to_move = WHITE;
					}
					break;
				case 'b':
					if(empty == 1) {
						bd->to_move = BLACK;
					}
					break;
				case 'K':
					bd->castle[0] = 'K';
					break;
				case 'Q':
					bd->castle[1] = 'Q';
					break;
				case 'k':
					bd->castle[2] = 'k';
					break;
				case 'q':
					bd->castle[3] = 'q';
					break;
				case '0':
					if(empty == 4) {
						bd->half_move = *FEN - '0';
					} else if(empty == 5) {
						bd->whole_move = *FEN - '0';
					}
					break;
				case '1':
					if(empty == 4) {
						bd->half_move = *FEN - '0';
					} else if(empty == 5) {
						bd->whole_move = *FEN - '0';
					}
					break;
			}
			*FEN++;
		 	continue;	
		}

		switch(*FEN) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				count = *FEN - '0';
				break;

			case ' ':
				empty++;
				break;
			case '/':
				rank--;
				file = 1;
				FEN++;
				continue;

			case 'K':
				bd->WhiteKing |= getPosition(i);
				break;
			case 'Q':
				bd->WhiteQueens |= getPosition(i);
				break;
			case 'B':
				bd->WhiteBishops |= getPosition(i);
				break;
			case 'N':
				bd->WhiteKnights |= getPosition(i);
				break;
			case 'R':
				bd->WhiteRooks |= getPosition(i);
				break;
			case 'P':
				bd->WhitePawns |= getPosition(i);
				break;

			case 'k':
				bd->BlackKing |= getPosition(i);
				break;
			case 'q':
				bd->BlackQueens |= getPosition(i);
				break;
			case 'b':
				bd->BlackBishops |= getPosition(i);
				break;
			case 'n':
				bd->BlackKnights |= getPosition(i);
				break;
			case 'r':
				bd->BlackRooks |= getPosition(i);
				break;
			case 'p':
				bd->BlackPawns |= getPosition(i);
				break;
			default:
				printf("error\n");
		}

		*FEN++;
		if(!empty) {
			file+=count;
		}
	}

	/* all white pieces */
	bd->WhitePieces = bd->WhitePawns | bd->WhiteRooks | bd->WhiteKnights | bd->WhiteBishops | bd->WhiteQueens | bd->WhiteKing;
	
	/* all black pieces */
	bd->BlackPieces = bd->BlackPawns | bd->BlackRooks | bd->BlackKnights | bd->BlackBishops | bd->BlackQueens | bd->BlackKing;

	/* all pieces */
	bd->AllPieces = bd->WhitePieces | bd->BlackPieces;
}
