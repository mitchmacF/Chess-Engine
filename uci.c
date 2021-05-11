// uci.c

//#include "stdio.h"
//#include "defs.h"
//#include "string.h"

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

// go depth 6 wtime 180000 btime 100000 binc 1000 winc 1000 movetime 1000 movestogo 40
//void ParseGo(char* line, S_SEARCHINFO *info, S_BOARD *pos) {
void ParseGo(char* line) {
	/*int depth = -1, movestogo = 30,movetime = -1;
	int time = -1, inc = 0;
    char *ptr = NULL;
	//info->timeset = FALSE;
	
	if ((ptr = strstr(line,"infinite"))) {
		;
		printf("reached\n");
	} 
	
	//if ((ptr = strstr(line,"binc")) && pos->side == BLACK) {
	if ((ptr = strstr(line,"binc")) && bd->to_move == BLACK) {
		inc = atoi(ptr + 5);
		printf("reached\n");
	}
	
	//if ((ptr = strstr(line,"winc")) && pos->side == WHITE) {
	if ((ptr = strstr(line,"winc")) && bd->to_move == WHITE) {
		inc = atoi(ptr + 5);
		printf("reached\n");
	} 
	
	//if ((ptr = strstr(line,"wtime")) && pos->side == WHITE) {
	if ((ptr = strstr(line,"wtime")) && bd->to_move == WHITE) {
		time = atoi(ptr + 6);
		printf("reached\n");
	} 
	  
	//if ((ptr = strstr(line,"btime")) && pos->side == BLACK) {
	if ((ptr = strstr(line,"btime")) && bd->to_move == BLACK) {
		time = atoi(ptr + 6);
		printf("reached\n");
	} 
	  
	if ((ptr = strstr(line,"movestogo"))) {
		movestogo = atoi(ptr + 10);
		printf("reached\n");
	}
	  
	if ((ptr = strstr(line,"movetime"))) {
		movetime = atoi(ptr + 9);
		printf("reached\n");
	}
	  
	if ((ptr = strstr(line,"depth"))) {
		depth = atoi(ptr + 6);
		printf("reached\n");
	} 
	
	if(movetime != -1) {
		time = movetime;
		printf("reached\n");
		movestogo = 1;
	}
	
	//info->starttime = GetTimeMs();
	//info->depth = depth;*/
	
	/*if(time != -1) {
		info->timeset = TRUE;
		time /= movestogo;
		time -= 50;		
		info->stoptime = info->starttime + time + inc;
	} 
	
	if(depth == -1) {
		info->depth = MAXDEPTH;
	}*/
	
	/*printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
		time,info->starttime,info->stoptime,info->depth,info->timeset);
	SearchPosition(pos, info);*/
	Search();
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
			  //printf("PARSE MOVES HERE\n");
			  //break;
              Move mv = parseMove(ptrChar);
			  //printMove(mv);
			  if(!mv.mv && !mv.mv_score && !mv.pc) {
				  printf("ERROR: INVALID\n");
				  exit(0);
			  }
			  make_move(mv);
              //pos->ply=0;
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
            //ParsePosition("position startpos\n");
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
