#include "rules.h"
#include "type.h"

uint64_t knightMoves(uint64_t knights) {
    uint64_t l1 = (knights >> 1) & 0x7f7f7f7f7f7f7f7f;
    uint64_t l2 = (knights >> 2) & 0x3f3f3f3f3f3f3f3f;
    uint64_t r1 = (knights << 1) & 0xfefefefefefefefe;
    uint64_t r2 = (knights << 2) & 0xfcfcfcfcfcfcfcfc;

    uint64_t h1 = l1 | r1;
    uint64_t h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

uint64_t pawnMoves(uint64_t cPawn, uint64_t oPawn) {
    uint64_t one = (cPawn << 8) & ~oPawn;
    uint64_t two = (cPawn & 0xFF00) << 16;
    printf("%" PRId64 "\n", cPawn);
    return two; 
}

// todo make this not void
// just test it out rn 
void moves(chessBoard* board) {
    printf("%" PRId64 "\n", pawnMoves(board->wPawn, board->bPawn));
}
