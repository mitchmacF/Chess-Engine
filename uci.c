#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include "board.h"
#include "extern.h"

#define INPUTBUFFER 400 * 6
char *start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void ParseGo(char* line) {
	char *line_ptr;
	int GUI_time = 0, search_time = 0;

	if((line_ptr = strstr(line, "wtime")) && bd->to_move == WHITE) {
		GUI_time = atoi(line_ptr + 5);
	}
	else if((line_ptr = strstr(line, "btime")) && bd->to_move == BLACK) {
		GUI_time = atoi(line_ptr + 5);
	}

	// convert GUI time to time we can handle
	//search_time = (GUI_time/1000)/(40-bd->whole_move);
	//Search(search_time);
	Search(GUI_time/1000);
	printCharBoard();
}

// position fen fenstr
// position startpos
// ... moves e2e4 e7e5 b7b8q
//void ParsePosition(char* lineIn, S_BOARD *pos) {
void ParsePosition(char* lineIn) {
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

void Uci_Loop() {
	
	setbuf(stdin, NULL);
    setbuf(stdout, NULL);
	
	char line[INPUTBUFFER];
    printf("id name %s\n", "Mitchell");
    printf("id author my_engine\n");
    printf("uciok\n");
	int quit = 0;

	while (1) {
		memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if (!fgets(line, INPUTBUFFER, stdin))
        continue;

        if (line[0] == '\n')
        continue;

        if (!strncmp(line, "isready", 7)) {
            printf("readyok\n");
            continue;
        } else if (!strncmp(line, "position", 8)) {
            ParsePosition(line);
        } else if (!strncmp(line, "ucinewgame", 10)) {
			parseFEN(start_pos);
			printCharBoard();
        } else if (!strncmp(line, "go", 2)) {
            ParseGo(line);
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
