#include "headers.h"
#include <stdlib.h>

// fills all 64 bits with 15 bits at a time
#define RAND_64 ( ((U64)rand()) | \
                (((U64)rand()) << (15)) | \
                (((U64)rand()) << (30)) | \
                (((U64)rand()) << (45)) | \
                ((((U64)rand()) & (0xf)) << (60)) )

int Sq120toSq64[BRD_SQ_NUM];
int Sq64toSq120[64];

U64 setMask[64];
U64 clearMask[64];

U64 pieceKeys[13][120];
U64 sideKey;
U64 castleKeys[16];

int filesBRD[BRD_SQ_NUM];
int ranksBRD[BRD_SQ_NUM];

void initFilesRanksBRD() {

    int sq = A1;

    for(int i = 0; i < BRD_SQ_NUM; i++) {
        filesBRD[i] = OFFBOARD;
        ranksBRD[i] = OFFBOARD;
    }

    for(int rank = RANK_1; rank <= RANK_8; rank++) {
        for(int file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file, rank);
            filesBRD[sq] = file;
            ranksBRD[sq] = rank;
        }
    }
}

void initHashKeys() {
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 120; j++) {
            pieceKeys[i][j] = RAND_64;
        }
    }
    sideKey = RAND_64;
    for (int k = 0; k < 16; k++)
    {
        castleKeys[k] = RAND_64;
    }
    
}

void initBitMasks() {

    for(int i= 0; i < 64; i++) {
        setMask[i] = 0ULL;
        clearMask[i] = 0ULL;
    }

    for (int i = 0; i < 64; i++)
    {
        setMask[i] |= (1ULL << i);
        clearMask[i] = ~setMask[i];
    }
    
}

void InitSq120to64() {
    
    int sq = A1;
    int sq64 = 0;
    // DOUBLE CHECK FOR INDEX INCREMENTS
    for (int i = 0; i < BRD_SQ_NUM; i++) {
        Sq120toSq64[i] = 65;
    }
    for (int i = 0; i < 64; i++) {
        Sq64toSq120[i] = 120;
    }
    for(int rank = RANK_1; rank <= RANK_8; rank++) {
        for (int file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file,rank);
            Sq64toSq120[sq64] = sq;
            Sq120toSq64[sq] = sq64;
            sq64++;
        }
        
    }

}

void Allinit() {
    InitSq120to64();
    initBitMasks();
    initHashKeys();
    initFilesRanksBRD();
    InitMvvLva();
}