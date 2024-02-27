#include "headers.h"
#include <stdio.h>

U64 generatePosKey(const S_BOARD *pos) {

    int sq = 0;
    U64 finalKey = 0;
    int piece = EMPTY; 

    // loop through all squares and set piece to the whatever value is
    // stored at that square in that array. 
    for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
        piece = pos->pieces[sq];
        if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
            ASSERT(piece >= wP && piece <= bK);
            finalKey ^= pieceKeys[piece][sq];
        }
    }

    // if its white to move
    if(pos->side == WHITE) {
        finalKey ^= sideKey;
    }

    if(pos->enPassant != NO_SQ) {
        ASSERT(pos->enPassant >= 0 && pos->enPassant < BRD_SQ_NUM);
        finalKey ^= pieceKeys[EMPTY][pos->enPassant];
    }

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    finalKey ^= castleKeys[pos->castlePerm];

    return finalKey;
}
