#include "rules.h"

uint64_t knightMoves(uint64_t knights) {
    uint64_t l1 = (knights >> 1) & 0x7f7f7f7f7f7f7f7fULL;
    uint64_t l2 = (knights >> 2) & 0x3f3f3f3f3f3f3f3fULL;
    uint64_t r1 = (knights << 1) & 0xfefefefefefefefeULL;
    uint64_t r2 = (knights << 2) & 0xfcfcfcfcfcfcfcfcULL;

    uint64_t h1 = l1 | r1;
    uint64_t h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

uint64_t pawnMoves(uint64_t cPawn, uint64_t empty, uint64_t enemy) {
    uint64_t one_step = (cPawn << 8) & empty;
    const uint64_t RANK_2_MASK = RANK_2;
    uint64_t two_step = ((cPawn & RANK_2_MASK) << 16)
                        & empty
                        & ((cPawn << 8) & empty);
    uint64_t cap_left  = ((cPawn & ~FILE_A) << 7) & enemy;
    uint64_t cap_right = ((cPawn & ~FILE_H) << 9) & enemy;

    return one_step | two_step | cap_left | cap_right;
}

// helper function to make the code more readable  
inline uint64_t possibleKing(uint64_t king) {
    uint64_t notAFile = ~FILE_A;
    uint64_t notHFile = ~FILE_H;

    uint64_t attacks = 0;
    attacks |= (king & notAFile) >> 1;
    attacks |= (king & notHFile) << 1;
    attacks |= (king & ~FILE_A) >> 9;
    attacks |= king >> 8;
    attacks |= (king & ~FILE_H) >> 7;
    attacks |= (king & ~FILE_A) << 7;
    attacks |= king << 8;
    attacks |= (king & ~FILE_H) << 9;
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
    uint64_t reversed = 0ULL;
    for (int i = 0; i < 64; i++) {
        reversed <<= 1;
        reversed |= (bb & 1ULL);
        bb >>= 1;
    }
    return reversed;
}

uint64_t lineAttacks(uint64_t occ, const SMasks *pMask) {
    uint64_t lower = pMask->lower & occ;
    uint64_t upper = pMask->upper & occ;
    uint64_t ms1B = (uint64_t)0x8000000000000000ULL >> lzcnt64(lower | 1ULL);
    uint64_t odiff = upper ^ (upper - ms1B);
    return pMask->lineEx & odiff;
}

// todo make this not void
// just test it out rn 
void moves(chessBoard* board, gameState* game) {
    if (game->bKingCastle == false) {
        printf("Problem: Black cannot castle king‐side.\n");
    }

    {
        uint64_t wPawnMoves = pawnMoves(
            board->wPawn,
            board->empty,
            board->blackPeices
        );
        printf("White pawn move bitboard: 0x%016" PRIu64 "\n", wPawnMoves);
    }

    {
        uint64_t rawKing = kingMoves(board->wKing, board->whitePeices);
        uint64_t dispKing = reverse(rawKing);
        printf("White king move bitboard (reversed for display): 0x%016" PRIu64 "\n", dispKing);
        print_board(dispKing);
    }
}

