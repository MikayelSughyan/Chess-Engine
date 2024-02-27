#include "headers.h"
#include <stdio.h>
#include <time.h>

long leafNodes;

void Perft(int depth, S_BOARD *pos) {
    ASSERT(CheckBoard(pos));

    if(depth == 0) {
        leafNodes++;
        return;    
    }

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);

    for(int moveNum = 0; moveNum < list->count; moveNum++) {
        if(!MakeMove(pos, list->moves[moveNum].move)) {
            continue;
        }
        Perft(depth-1, pos);
        TakeMove(pos);
    }

    return;
}

void PerftTest(int depth, S_BOARD *pos) {

    clock_t start, end;
    double cpu_time_used;

    ASSERT(CheckBoard(pos));

	printBoard(pos);
	printf("\nStarting Test To Depth:%d\n",depth);	
	leafNodes = 0;
	start = clock();
    S_MOVELIST list[1];
    GenerateAllMoves(pos,list);	
    
    int move;	    
    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        move = list->moves[MoveNum].move;
        if ( !MakeMove(pos,move))  {
            continue;
        }
        long cumnodes = leafNodes;
        Perft(depth - 1, pos);
        TakeMove(pos);        
        long oldnodes = leafNodes - cumnodes;
        printf("move %d : %s : %ld\n",MoveNum+1,PrMove(move),oldnodes);
    }
	
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("\nTest Complete : %ld nodes visited in %fs\n", leafNodes, cpu_time_used);

    return;
}