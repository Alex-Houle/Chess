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
    uint64_t notAFile = ~FILE_A;
    uint64_t notHFile = ~FILE_H;

    uint64_t attacks = 0;

    attacks |= (king & notAFile) >> 1;      // West
    attacks |= (king & notHFile) << 1;      // East
    attacks |= (king & ~FILE_A) >> 9;       // SW
    attacks |= king >> 8;                   // South
    attacks |= (king & ~FILE_H) >> 7;       // SE
    attacks |= (king & ~FILE_A) << 7;       // NW
    attacks |= king << 8;                   // North
    attacks |= (king & ~FILE_H) << 9;       // NE

    return attacks;
}
 

// possible change is to make the function take where the enemy attacks
// that takes memory though 
uint64_t kingMoves(uint64_t king, uint64_t friends) {
    uint64_t possible = possibleKing(king);
    return possible & ~friends; 
}

// reverse the bit in a uint64_t
uint64_t reverse(uint64_t bb) {
    uint64_t reversed = 0ull;
    for (int i = 0; i < 64; i++) {
        reversed <<= 1;
        if ((bb & 1 ) == 1) {
            reversed ^=1;
        }
        bb >>= 1;
    }
    return reversed; 
}


// todo make this not void
// just test it out rn 
void moves(chessBoard* board, gameState* game) {
    // this is to stop com
    if (game->bKingCastle == 0) {
        printf("Problem");
    }
    
    if (0) {
        printf("%" PRId64 "\n", pawnMoves(board->wPawn, board->empty ,board->blackPeices));
    } else { 
       // printf("%" PRId64 "\n", pawnMoves(board->bPawn, board->empty, board->whitePeices ));
    }
    printf("%" PRId64 "\b", reverse(kingMoves(board->wKing, board->whitePeices)));
    printf("\n");
    print_board(reverse(reverse(kingMoves(board->wKing, board->whitePeices))));
    
}
