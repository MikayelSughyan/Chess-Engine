#include <stdio.h>
#include <stdlib.h>
#include "headers.h"
#include "time.h"

#define FEN1 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FEN2 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define KNIGHTKING "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"

#define WAC1 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

int main() {
    Allinit();
    
    Uci_Loop();
    
    return 0;
} 