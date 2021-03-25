#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "extern.h"

#define rank_file_idx(r, f) ( ((r-1) * 8) + (f-1) )

/*
8	56 57 58 59 60 61 62 64
7	48 49 50 51 52 53 54 55
6	40 41 42 43 44 45 46 47
5	32 33 34 35 36 37 38 39 
4	24 25 26 27 28 29 30 21
3	16 17 18 19 20 21 22 23
2	8  9  10 11 12 13 14 15
1	1  2  3  4  5  6  7  8

r
  c	1  2  3  4  5  6  7  8
*/

void printMoveList() {
	int i, curr_mv, score; 
	char *from, *to;

	printf("Total Move Count: %d\n", mv_list->total_count);
	
	for(i = 0; i < mv_list->total_count; i++) {
		curr_mv = mv_list->moves[i].mv;
		score = mv_list->moves[i].mv_score;

		from = move_notation[ (curr_mv & (0x3f))    ];
		to =   move_notation[ (curr_mv & (0x3f<<6))>>6 ];

		printf("%s%s\n", from, to);
	}
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
	for(int j = 63; j>=0; j--) {
		printf("%c ", board[j]);   
		if((j) % 8 == 0) {
			printf("\n");
		}
	}
	printf("\n");
	if(bd->to_move) 
		printf("Black\n");
	else
		printf("White\n");

	for(int k = 0; k < 4; k++) {
		printf("%c", bd->castle[k]);
	}
	printf("\n");
	printf("%llu\n", bd->ep);
	printf("%d %d\n\n", bd->half_move, bd->whole_move);
}

void fillCharBoard() {
	U64 loc;
	
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

}

//"bnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
void parseFEN(char *FEN) {

	int rank = 8, file = 1, empty = 0, count, i;

	while(*FEN) {
		i = rank_file_idx(rank, file);
		count = 1;

		if(empty) {
			switch(*FEN) {
				case '-':
					if(empty == 2) {
						bd->castle[0] = '-';
					} else if(empty == 3) {
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
