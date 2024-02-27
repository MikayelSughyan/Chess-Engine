
#include "headers.h"
#include <stdio.h>

const int knDir[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
const int rkDir[4] = { -1, -10, 1, 10 };
const int biDir[4] = { -9, -11, 11, 9 };
const int kiDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

int SqAttacked(const int sq, const int side, const S_BOARD *pos) {

    int pce, t_sq, dir;

    ASSERT(SqOnBoard(sq));
    ASSERT(SideValid(side));
    ASSERT(CheckBoard(pos));

    // pawns
    if(side == WHITE) {
        if(pos->pieces[sq - 11] == wP || pos->pieces[sq - 9] == wP) {
            return TRUE;
        }
    }
    else {
        if(pos->pieces[sq + 11] == bP || pos->pieces[sq + 9] == bP) {
            return TRUE;
        }
    }

    // knights
    for(int i = 0; i < 8; i++) {
        pce = pos->pieces[sq + knDir[i]];
        if(pce != OFFBOARD && IsKn(pce) && pieceCol[pce] == side) { // keep eye out
            return TRUE;
        }
    }

    // rooks and queens
    for(int i = 0; i < 4; i++) {
        dir = rkDir[i];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while(pce != OFFBOARD) {
            if(pce != EMPTY) {
                if(IsRQ(pce) && pieceCol[pce] == side) {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }

    // bishop and queens
    for(int i = 0; i < 4; i++) {
        dir = biDir[i];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while(pce != OFFBOARD) {
            if(pce != EMPTY) {
                if(IsBQ(pce) && pieceCol[pce] == side) {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }

    // kings
    for(int i = 0; i < 8; i++) {
        pce = pos->pieces[sq + kiDir[i]];
        if(pce != OFFBOARD && IsKi(pce) && pieceCol[pce] == side) {
            return TRUE;
        }
    }

    return FALSE;
}