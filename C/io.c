#include <stdio.h>
#include "headers.h"


char *PrSq(const int sq) {

    static char SqStr[3];

    int file = filesBRD[sq];
    int rank = ranksBRD[sq];

    sprintf(SqStr, "%c%c", ('a' + file), ('1' + rank));

    return SqStr;
}

char *PrMove(const int move) {

    static char MvStr[6];

    int ff = filesBRD[FROMSQ(move)];
    int rf = ranksBRD[FROMSQ(move)];
    int ft = filesBRD[TOSQ(move)];
    int rt = ranksBRD[TOSQ(move)];

    int promoted = PROMOTED(move);

    if(promoted) {
        char pchar = 'q';
        if(IsKn(promoted)) {
            pchar = 'n';
        }
        else if(IsRQ(promoted) && !IsBQ(promoted)) {
            pchar = 'r';
        }
        else if(!IsRQ(promoted) && IsBQ(promoted)) {
            pchar = 'b';
        }
        sprintf(MvStr, "%c%c%c%c%c", ('a' + ff), 
                ('1' + rf), ('a' + ft), ('1' + rt), pchar);
    }
    else {
        sprintf(MvStr, "%c%c%c%c", ('a' + ff), 
                ('1' + rf), ('a' + ft), ('1' + rt));
    }
    return MvStr;
}

int ParseMove(char *ptrChar, S_BOARD *pos) {
    
	ASSERT(CheckBoard(pos));

	if(ptrChar[1] > '8' || ptrChar[1] < '1') return NOMOVE;
    if(ptrChar[3] > '8' || ptrChar[3] < '1') return NOMOVE;
    if(ptrChar[0] > 'h' || ptrChar[0] < 'a') return NOMOVE;
    if(ptrChar[2] > 'h' || ptrChar[2] < 'a') return NOMOVE;

    int from = FR2SQ(ptrChar[0] - 'a', ptrChar[1] - '1');
    int to = FR2SQ(ptrChar[2] - 'a', ptrChar[3] - '1');

    ASSERT(SqOnBoard(from) && SqOnBoard(to));

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);
    int move = 0;
    int PromPce = EMPTY;

    for(int moveNum = 0; moveNum < list->count; moveNum++) {
        move = list->moves[moveNum].move;
        if(FROMSQ(move) == from && TOSQ(move) == to) {
            PromPce = PROMOTED(move);
            if(PromPce != EMPTY) {
                if(IsRQ(PromPce) && !IsBQ(PromPce) && ptrChar[4] == 'r') {
                    return move;
                }
                else if(!IsRQ(PromPce) && IsBQ(PromPce) && ptrChar[4] == 'b') {
                    return move;
                }
                else if(IsRQ(PromPce) && IsBQ(PromPce) && ptrChar[4] == 'q') {
                    return move;
                }
                else if(IsKn(PromPce) && ptrChar[4] == 'n') {
                    return move;
                }
                continue;
            }
            return move;
        }
    }

    return NOMOVE;
}

void printMoveList(const S_MOVELIST *list) {

    int score = 0;
    int move = 0;
    printf("MoveList:\n");

    for(int i = 0; i < list->count; i++) {

        move = list->moves[i].move;
        score = list->moves[i].score;

        printf("Move:%d > %s (score:%d)\n", i+1, PrMove(move), score);
    }
    printf("MoveList Total %d Moves:\n\n", list->count);
}