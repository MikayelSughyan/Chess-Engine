#include <stdio.h>
#include "headers.h"
#include <string.h>

#define INPUTBUFFER 400 * 6

void ParseGo(char* line, S_SEARCHINFO *info, S_BOARD *pos) {
    int depth = -1, movestogo = 30,movetime = -1;
	int time = -1, inc = 0;
    char *ptr = NULL;
	info->timeset = FALSE;

    if ((ptr = strstr(line,"infinite"))) {
		;
	}

	if ((ptr = strstr(line,"binc")) && pos->side == BLACK) {
		inc = atoi(ptr + 5);
	}

	if ((ptr = strstr(line,"winc")) && pos->side == WHITE) {
		inc = atoi(ptr + 5);
	}

    if ((ptr = strstr(line,"wtime")) && pos->side == WHITE) {
		time = atoi(ptr + 6);
	}

	if ((ptr = strstr(line,"btime")) && pos->side == BLACK) {
		time = atoi(ptr + 6);
	}

	if ((ptr = strstr(line,"movestogo"))) {
		movestogo = atoi(ptr + 10);
	}

	if ((ptr = strstr(line,"movetime"))) {
		movetime = atoi(ptr + 9);
	}

	if ((ptr = strstr(line,"depth"))) {
		depth = atoi(ptr + 6);
	}

    
	if(movetime != -1) {
		time = movetime;
		movestogo = 1;
	}

	info->starttime = GetTimeMs();
	info->depth = depth;

    if(time != -1) {
		info->timeset = TRUE;
		time /= movestogo;
		time -= 50;
		info->stoptime = info->starttime + time + inc;
	}

	if(depth == -1) {
		info->depth = MAXDEPTH;
	}

	printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
		time,info->starttime,info->stoptime,info->depth,info->timeset);
	SearchPosition(pos, info);
}

void ParsePosition(char* lineIN, S_BOARD *pos) {
    lineIN += 9;
    char *ptrChar = lineIN;
    if(strncmp(lineIN, "startpos", 8) == 0) {
        ParseFEN(START_FEN, pos);
    } 
    else {
        ptrChar = strstr(lineIN, "fen");
        if(ptrChar == NULL) {
            ParseFEN(START_FEN, pos);
        }
        else {
            ptrChar += 4;
            ParseFEN(ptrChar, pos);
        }
    }

    ptrChar = strstr(lineIN, "moves");
    int move;

    if(ptrChar != NULL) {
        ptrChar += 6;
        while(*ptrChar) {
            move = ParseMove(ptrChar, pos);
            if(move == NOMOVE) break;
            MakeMove(pos, move);
            pos->ply = 0;
            while(*ptrChar && *ptrChar != ' ') ptrChar++;
            ptrChar++;
        }
    }
    printBoard(pos);
}

void Uci_Loop() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char line[INPUTBUFFER];
    printf("id name %s\n", NAME);
    printf("id author Mikayel\n");
    printf("uciok\n");

    S_BOARD pos[1];
    S_SEARCHINFO info[1];
    InitPvTable(pos->PvTable);

    while (TRUE)    
    {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if(!fgets(line, INPUTBUFFER, stdin)) {
            continue;
        }
        if(line[0] == '\n') {
            continue;
        }

        if(!strncmp(line, "isready", 7)) {
            printf("readyok\n");
            continue;
        }
        else if(!strncmp(line, "position", 8)) {
            ParsePosition(line, pos);
        }
        else if(!strncmp(line, "ucinewgame", 10)) {
            ParsePosition("position startpos\n", pos);
        }
        else if(!strncmp(line, "go", 2)) {
            ParseGo(line, info, pos);
        }
        else if(!strncmp(line, "quit", 4)) {
            info->quit = TRUE;
            break;
        }
        else if(!strncmp(line, "uci", 3)) {
            printf("id name %s\n", NAME);
            printf("id author Mikayel\n");
            printf("uciok\n");
        }

        if(info->quit) 
            {break;}
    }
    free(pos->PvTable->ptable);
}