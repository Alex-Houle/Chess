#include "rules.h"


uint64_t pawnMoveW(uint64_t pawns, uint64_t empty) {
    uint64_t single_push = (pawns << 8) & empty; 
    uint64_t double_push = ((pawns & RANK_2) << 16) & empty; 
    return single_push | double_push;
}

uint64_t pawnMoveB(uint64_t pawns, uint64_t empty) {
    uint64_t single_push = (pawns >> 8) & empty; 
    uint64_t double_push = ((pawns & RANK_7) >> 16) & empty;
    return single_push | double_push;
}

uint64_t knightMove(uint64_t knights) {
    uint64_t l1 = (knights >> 1) & ~FILE_H;
    uint64_t l2 = (knights >> 2) & ~(FILE_H | FILE_G);
    uint64_t r1 = (knights << 1) & ~FILE_A;
    uint64_t r2 = (knights << 2) & ~(FILE_A | FILE_B);
    uint64_t h1 = l1 | r1;
    uint64_t h2 = l2 | r2;
    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}




void generate_moves(const chessBoard* board, const gameState* game) {
    uint64_t occ = ~game->EmptySquares;
    uint64_t empty = ~occ;


    printf("Generating moves:\n");
    // White Pawns
    uint64_t wPawnMoves = pawnMoveW(board->wPawn, empty);
    printf("White Pawn Moves: 0x%016" PRIx64 "\n", wPawnMoves);

    // Black Pawns
    uint64_t bPawnMoves = pawnMoveB(board->bPawn, empty);
    printf("Black Pawn Moves: 0x%016" PRIx64 "\n", bPawnMoves);

    // White moves just temporary for debugging
    uint64_t wKnightMoves = knightMove(board->wKnight) & ~board->wPawn & ~board->wKnight & ~board->wBishop & ~board->wRook & ~board->wQueen & ~board->wKing;
    printf("White Knight Moves: 0x%016" PRIx64 "\n", wKnightMoves);

    // Black Knight moves just temp for debugging
    uint64_t bKnightMoves = knightMove(board->bKnight) & ~board->bPawn & ~board->bKnight & ~board->bBishop & ~board->bRook & ~board->bQueen & ~board->bKing;
    printf("Black Knight Moves: 0x%016" PRIx64 "\n", bKnightMoves);

}