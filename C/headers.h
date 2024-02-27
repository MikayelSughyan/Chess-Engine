#ifndef HEADERS_H
#define HEADERS_H

#include <stdlib.h>

//#define DEBUG
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed", #n); \
printf(" On %s ", __DATE__); \
printf("At %s ", __TIME__); \
printf("In File %s ", __FILE__); \
printf("At Line %d\n ", __LINE__); \
exit(1); }
#endif


typedef unsigned long long U64;

#define NAME "Chess Engine 1.0"
#define BRD_SQ_NUM 120

#define MAXGAMEMOVES 2048
#define MAXPOSITIONMOVES 256
#define MAXDEPTH 64
#define INFINITE 30000
#define MATE 29000

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// GLOBALS
extern int Sq120toSq64[BRD_SQ_NUM];
extern int Sq64toSq120[64];
extern U64 setMask[64];
extern U64 clearMask[64];
extern U64 pieceKeys[13][120];
extern U64 sideKey;
extern U64 castleKeys[16];
extern char pceChar[];
extern char sideChar[];
extern char rankChar[];
extern char fileChar[];

extern int pieceBig[13];
extern int pieceMaj[13];
extern int pieceMin[13];
extern int pieceVal[13];
extern int pieceCol[13];

extern int filesBRD[BRD_SQ_NUM];
extern int ranksBRD[BRD_SQ_NUM];

extern int piecePawn[13];
extern int pieceKnight[13];
extern int pieceKing[13];
extern int pieceRookQueen[13];
extern int pieceBishopQueen[13];
extern int pieceSlides[13];

// MACRO
#define FR2SQ(f,r) ( (21 + (f)) + ((r) * 10) )
#define SQ64(sq120) (Sq120toSq64[(sq120)])
#define SQ120(sq64) (Sq64toSq120[(sq64)])
#define CLRBIT(bb, sq)  ((bb) &= clearMask[sq])
#define SETBIT(bb, sq)  ((bb) |= setMask[sq])

#define IsBQ(p) (pieceBishopQueen[(p)])
#define IsRQ(p) (pieceRookQueen[(p)])
#define IsKn(p) (pieceKnight[(p)])
#define IsKi(p) (pieceKing[(p)])

/* GAME MOVE */

/*
0000 0000 0000 0000 0000 0111 1111 -> From 0x7F
0000 0000 0000 0011 1111 1000 0000 -> To >> 7, 0x7F
0000 0000 0011 1100 0000 0000 0000 -> Captured >> 14, 0xF
0000 0000 0100 0000 0000 0000 0000 -> EP 0x40000
0000 0000 1000 0000 0000 0000 0000 -> Pawn Start 0x80000
0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece >> 20, 0xF
0001 0000 0000 0000 0000 0000 0000 -> Castle 0x1000000
*/

// move macros 
#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m) >> 7) & 0x7F)
#define CAPTURED(m) (((m) >> 14) & 0xF)
#define PROMOTED(m) (((m) >> 20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000
#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

#define NOMOVE 0

// Enumerations for pieces
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK};

// Enumerations for files
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };

// Enumerations for ranks
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

// Colors 
enum { WHITE, BLACK, BOTH };

// Board squares
enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

enum { FALSE, TRUE };

// Castle enumerations
enum { WKCASTLE = 1, WQCASTLE = 2, BKCASTLE = 4, BQCASTLE = 8 };

typedef struct {
    int move;
    int score;

} S_MOVE;

typedef struct {
    S_MOVE moves[MAXPOSITIONMOVES];
    int count;
} S_MOVELIST;

typedef struct S_UNDO {
    int move;
    int castlePerm;
    int enPassant;
    int fiftyMove;
    U64 posKey;

} S_UNDO;

typedef struct S_PVENTRY {
    U64 posKey;
    int move;
} S_PVENTRY;

typedef struct S_PVTABLE {
    S_PVENTRY *ptable;
    int numEntries;
} S_PVTABLE;

typedef struct {
    int pieces[BRD_SQ_NUM];
    U64 pawns[3];
    int KingSq[2];
    int side;
    int enPassant;
    int fiftyMove;
    int ply;
    int hisPly;
    int castlePerm;
    U64 posKey;
    int pceNum[13];
    int bigPce[2];
    int majPce[2];
    int minPce[2];
    int material[2];\

    S_UNDO history[MAXGAMEMOVES];

    // piece list
    int pList[13][10];

    S_PVTABLE PvTable[1];
    int PvArray[MAXDEPTH];

    int searchHistory[12][BRD_SQ_NUM];
    int searchKillers[2][MAXDEPTH];

} S_BOARD;

typedef struct {
    int starttime;
    int stoptime;
    int depth;
    int depthset;
    int timeset;
    int movestogo;
    int infinite;
    long nodes;
    int quit;
    int stopped;

    float fh;
    float fhf;

} S_SEARCHINFO;

// HASHKEYS.c
extern U64 generatePosKey(const S_BOARD *pos);

// INIT.c
extern void Allinit();

// BITBOARD.c
extern void PrintBitBoard(U64 bb);
extern int popBit(U64 *bb);
extern int countBits(U64 b);

// BOARD.c
extern void ResetBoard(S_BOARD *pos);
extern int ParseFEN(char *fen, S_BOARD *pos);
extern void printBoard(const S_BOARD *pos);
extern void UpdateListMaterials(S_BOARD *pos);
extern int CheckBoard(const S_BOARD *pos);

// ATTACK.c
extern int SqAttacked(const int sq, const int side, const S_BOARD *pos);

// IO.c
extern char *PrMove(const int move);
extern char *PrSq(const int sq);
extern void printMoveList(const S_MOVELIST *list);
extern int ParseMove(char *ptrChar, S_BOARD *pos);

// VALIDATE.c
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid(const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);

// MOVEGEN.c
extern void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list);
extern int MoveExists(S_BOARD *pos, const int move);
extern void InitMvvLva();
extern void GenerateAllCaps(const S_BOARD *pos, S_MOVELIST *list);

// MAKEMOVE.c
extern void TakeMove(S_BOARD *pos);
extern int MakeMove(S_BOARD *pos, int move);

// PERFT.c
extern void PerftTest(int depth, S_BOARD *pos);

// SEARCH.c
extern void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info);

// PVTABLE.c
extern void InitPvTable(S_PVTABLE *table);
extern void StorePvMove(const S_BOARD *pos, const int move);
extern int ProbePvTable(const S_BOARD *pos);
extern int GetPvLine(const int depth, S_BOARD *pos);
extern void ClearPvTable(S_PVTABLE *table);

// EVALUATE.c
extern int EvalPosition(const S_BOARD *pos);

// MISC.c
extern int GetTimeMs();

// UCI.c
extern void Uci_Loop();

#endif