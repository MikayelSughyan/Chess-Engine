#include <stdio.h>
#include "headers.h"
#include <time.h>

static void CheckUp(S_SEARCHINFO *info) {
    // check if time is up or intterupt from GUI
    if(info->timeset == TRUE && GetTimeMs() > info->stoptime) {
        info->stopped = TRUE;
    }
}

static void PickNextMove(int moveNum, S_MOVELIST *list) {
    S_MOVE temp;
    int bestScore = 0;
    int bestNum = moveNum;

    for(int i = moveNum; i < list->count; i++) {
        if(list->moves[i].score > bestScore) {
            bestScore = list->moves[i].score;
            bestNum = i;
        }
    }
    temp = list->moves[moveNum];
    list->moves[moveNum] = list->moves[bestNum];
    list->moves[bestNum] = temp;
}

static int IsRepetition(const S_BOARD *pos) {
    for(int i = (pos->hisPly - pos->fiftyMove); i < pos->hisPly - 1; i++) {
        ASSERT(i >= 0 && i < MAXGAMEMOVES);
        if(pos->posKey == pos->history[i].posKey) {
            return TRUE;
        }
    }
    return FALSE;
}

static void ClearForSearch(S_BOARD *pos, S_SEARCHINFO *info) {

    //clock_t start;

    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < BRD_SQ_NUM; j++) {
            pos->searchHistory[i][j] = 0;
        }
    }

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < MAXDEPTH; j++) {
            pos->searchKillers[i][j] = 0;
        }
    }

    ClearPvTable(pos->PvTable);
    pos->ply = 0;

    info->stopped = 0;
    info->nodes = 0;
    info->fh = 0;
    info->fhf = 0;
}

static int Quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info) {
    
	ASSERT(CheckBoard(pos));
	ASSERT(beta>alpha);
    
	if(( info->nodes & 2047 ) == 0) {
		CheckUp(info);
	}
    
	info->nodes++;

	if(IsRepetition(pos) || pos->fiftyMove >= 100) {
		return 0;
	}

	if(pos->ply > MAXDEPTH - 1) {
		return EvalPosition(pos);
	}

	int Score = EvalPosition(pos);

	ASSERT(Score>-INFINITE && Score<INFINITE);

	if(Score >= beta) {
		return beta;
	}

	if(Score > alpha) {
		alpha = Score;
	}

	S_MOVELIST list[1];
    GenerateAllCaps(pos,list);

    int MoveNum = 0;
	int Legal = 0;
	Score = -INFINITE;

	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {

		PickNextMove(MoveNum, list);

        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }

		Legal++;
		Score = -Quiescence( -beta, -alpha, pos, info);
        TakeMove(pos);

		if(info->stopped == TRUE) {
			return 0;
		}

		if(Score > alpha) {
			if(Score >= beta) {
				if(Legal==1) {
					info->fhf++;
				}
				info->fh++;
				return beta;
			}
			alpha = Score;
		}
    }

	//ASSERT(alpha >= OldAlpha);

	return alpha;
}

static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int DoNULL) {
    ASSERT(CheckBoard(pos));

    if(depth <= 0) {
        return Quiescence(alpha, beta, pos, info);
        //return EvalPosition(pos);
    }

    if(( info->nodes & 2047 ) == 0) {
		CheckUp(info);
	}

    info->nodes++;
    if(IsRepetition(pos) || pos->fiftyMove >= 100) {
        return 0;
    }

    if(pos->ply > MAXDEPTH -1) {
        return EvalPosition(pos);
    }

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);

    int legal = 0;
    int oldAlpha = alpha;
    int bestMove = NOMOVE;
    int score = -INFINITE;
    int PvMove = ProbePvTable(pos);

    if(PvMove != NOMOVE) {
        for(int moveNum = 0; moveNum < list->count; moveNum++) {
            if(list->moves[moveNum].move == PvMove) {
                list->moves[moveNum].score = 2000000;
                break;
            }
        }
    }
    
    for(int moveNum = 0; moveNum < list->count; moveNum++) {

        PickNextMove(moveNum, list);
        if(!MakeMove(pos, list->moves[moveNum].move)) {
            continue;
        }

        legal++;
        score = -AlphaBeta(-beta,-alpha, depth - 1, pos, info, TRUE);
        TakeMove(pos);

        if(info->stopped == TRUE) {
            return 0;
        }

        if(score > alpha) {
            if(score >= beta) {
                if(legal == 1) {
                    info->fhf++;
                }
                info->fh++;

                if(!(list->moves[moveNum].move & MFLAGCAP)) {
                    pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
                    pos->searchKillers[0][pos->ply] = list->moves[moveNum].move;
                }
                return beta;
            }
            alpha = score;
            bestMove = list->moves[moveNum].move;
            if(!(list->moves[moveNum].move & MFLAGCAP)) {
                pos->searchHistory[pos->pieces[FROMSQ(bestMove)]][TOSQ(bestMove)] += depth;
            }
        }
    }

    if(legal == 0) { 
        if(SqAttacked(pos->KingSq[pos->side],pos->side^1,pos)) {
            return -MATE + pos->ply;
        }
        else {
            return 0;
        }
    }

    if(alpha != oldAlpha) {
        StorePvMove(pos, bestMove);
    }
    return alpha;
}

void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info) {
    //iterative deepening
    int bestMove = NOMOVE;
    int bestScore = -INFINITE;
    int currentDepth = 0;
    int pvMoves = 0;
    int pvNum = 0;

    ClearForSearch(pos, info);

    for(currentDepth = 1; currentDepth <= info->depth; currentDepth++) {
        bestScore = AlphaBeta(-INFINITE, INFINITE, currentDepth, pos, info, TRUE);

        if(info->stopped == TRUE) {
            break;
        }

        pvMoves = GetPvLine(currentDepth, pos);
        bestMove = pos->PvArray[0];
        printf("info score cp score:%d depth %d nodes %ld time %d ",bestScore,currentDepth,info->nodes,GetTimeMs() - info->starttime);
        pvMoves = GetPvLine(currentDepth,pos);
        printf("pv");
        for(pvNum = 0; pvNum < pvMoves; pvNum++) {
            printf(" %s", PrMove(pos->PvArray[pvNum]));
        }
        printf("\n");
        //printf("Ordering:%.2f\n", (info->fhf/info->fh));
    }

    printf("bestmove %s\n", PrMove(bestMove));

}