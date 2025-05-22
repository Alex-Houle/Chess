#include "rules.h"
#include "type.h"
#include "board.h"
#include <stdint.h>

uint64_t knightMoves(uint64_t knights) {
    uint64_t l1 = (knights >> 1) & 0x7f7f7f7f7f7f7f7f;
    uint64_t l2 = (knights >> 2) & 0x3f3f3f3f3f3f3f3f;
    uint64_t r1 = (knights << 1) & 0xfefefefefefefefe;
    uint64_t r2 = (knights << 2) & 0xfcfcfcfcfcfcfcfc;

    uint64_t h1 = l1 | r1;
    uint64_t h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

uint64_t pawnMoves(uint64_t cPawn, uint64_t oPawn, uint64_t empty, uint64_t enemy) {
    uint64_t one = (cPawn << 8) & ~enemy ;
    uint64_t two = (cPawn & 0xFF00) << 16 & ~enemy;
    two &= one << 8;
    uint64_t dOne = cPawn << 7 & enemy; 
    uint64_t dTwo = cPawn << 9 & enemy;
    print_board(one | two); 
    return (one | two) & ~empty; 
}
        
// todo make this not void
// just test it out rn 
void moves(chessBoard* board, gameState* game) {
    if (1) {
        printf("%" PRId64 "\n", pawnMoves(board->wPawn, board->bPawn, board->empty ,board->blackPeices));
    } else { 
        printf("%" PRId64 "\n", pawnMoves(board->bPawn, board->bPawn, board->empty, board->whitePeices ));
    }
}
