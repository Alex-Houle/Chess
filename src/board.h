#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// Struct representing the chess board using bitboards
typedef struct {
    uint64_t wPawn;
    uint64_t wKnight;
    uint64_t wBishop;
    uint64_t wRook;
    uint64_t wQueen;
    uint64_t wKing;
    
    uint64_t bPawn;
    uint64_t bKnight;
    uint64_t bBishop;
    uint64_t bRook;
    uint64_t bQueen;
    uint64_t bKing;
} chessBoard;

typedef struct {
    bool wKingCastle;
    bool wQueenCastle;

    bool bKingCastle;
    bool bQueenCastle;

    char toMove;

} gameState;

// Function declarations
void fenToBit(chessBoard* board, char* fenString);
void bitToFen(chessBoard* board, char* output);
void printBitBoard(chessBoard* board);
int gameover(chessBoard* board);
void gameStart(chessBoard*, gameState*);

#endif // BOARD_H
