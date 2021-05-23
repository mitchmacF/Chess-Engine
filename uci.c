/* This uci implementation is based off BlueFever Software's implementation. Their code can be found on github and YouTube */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include "board.h"
#include "extern.h"

#define buffer 400 * 6
char *start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void ParseSearch(char* line);
void ParseBoardFEN(char* lineIn);

void Uci() {
	char line[buffer];
	printf("id name %s\n", "Mitchell");
	printf("id author Mitchell\n");
	printf("uciok\n");
	int quit = 0;

	while (1) {
		memset(&line[0], 0, sizeof(line));
		fflush(stdout);
		if (!fgets(line, buffer, stdin))
			continue;

		if (line[0] == '\n')
			continue;

		if (!strncmp(line, "isready", 7)) {
			printf("readyok\n");
			continue;
		} else if (!strncmp(line, "position", 8)) {
			ParseBoardFEN(line);
		} else if (!strncmp(line, "ucinewgame", 10)) {
			clear_board();
			parseFEN(start_pos);
			printCharBoard();
		} else if (!strncmp(line, "go", 2)) {
			ParseSearch(line);
		} else if (!strncmp(line, "quit", 4)) {
			quit = 1;
			break;
		} else if (!strncmp(line, "uci", 3)) {
			printf("id author Mitchell\n");
			printf("uciok\n");
		}
		if(quit) break;
	}
}

void ParseBoardFEN(char* lineIn) {
	lineIn += 9;
    char *ptrChar = lineIn;
	
    if(strncmp(lineIn, "startpos", 8) == 0){
		clear_board();
		parseFEN(start_pos);
    } else {
		ptrChar = strstr(lineIn, "fen");
		if(ptrChar == NULL) {
			//printf("1 Initializing start pos\n");
			//parseFEN(start_pos);
		} else {
            ptrChar+=4;
            parseFEN(ptrChar);
        }
    }
	
	ptrChar = strstr(lineIn, "moves");
	
	if(ptrChar != NULL) {
        ptrChar += 6;
        while(*ptrChar) {
              Move mv = parseMove(ptrChar);
			  if(!mv.mv && !mv.mv_score && !mv.pc) {
				  printf("ERROR: INVALID\n");
				  exit(0);
			  }
			  make_move(mv);
			  while(*ptrChar && *ptrChar != ' ') ptrChar++;
			  ptrChar++;
        }
    }
	printf("\n");
	printCharBoard();
}

void ParseSearch(char* line) {
	char *line_ptr;
	int GUI_time = 0, search_time = 0;

	if((line_ptr = strstr(line, "wtime")) && bd->to_move == WHITE) {
		GUI_time = atoi(line_ptr + 5);
	}
	else if((line_ptr = strstr(line, "btime")) && bd->to_move == BLACK) {
		GUI_time = atoi(line_ptr + 5);
	}

	// convert GUI time to time we can handle
	search_time = (GUI_time/1000)/(40);
	Search(search_time);
	printCharBoard();
}
