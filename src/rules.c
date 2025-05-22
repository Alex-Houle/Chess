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

uint64_t pawnMoves(uint64_t cPawn, uint64_t empty, uint64_t enemy) {
    uint64_t one = (cPawn << 8) & ~enemy ;
    uint64_t two = (cPawn & 0xFF00) << 16 & ~enemy;
    two &= one << 8;
    uint64_t dOne = cPawn << 7 & enemy; 
    uint64_t dTwo = cPawn << 9 & enemy;
    return ((one | two) & ~empty) | dOne | dTwo; 
}

// helper function to make the code more readable
uint64_t possibleKing(uint64_t king) {
    return king << 1 | king >> 1 | king << 7 | king >> 7 | king << 8 | king >> 8 | king << 9 | king >> 9;
}

// possible change is to make the function take where the enemy attacks
// that takes memory though 
uint64_t kingMoves(uint64_t king) {
    uint64_t possible = possibleKing(king);
    print_board(possible);
    printf("%" PRId64 "\n", possible);
    return possibleKing(king); 
}

// todo make this not void
// just test it out rn 
void moves(chessBoard* board, gameState* game) {
    if (1) {
        printf("%" PRId64 "\n", pawnMoves(board->wPawn, board->empty ,board->blackPeices));
    } else { 
        printf("%" PRId64 "\n", pawnMoves(board->bPawn, board->empty, board->whitePeices ));
    }
    kingMoves(board->wKing);
}
