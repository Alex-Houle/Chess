#include <stdio.h>
#include <stdint.h>

// sturct for the board 
typedef struct{
    uint64_t wBishop;
    uint64_t wRook;
    uint64_t wKnight;
    uint64_t wQueen;
    uint64_t bBishop;
    uint64_t bRook;
    uint64_t bKnight;   
    uint64_t bQueen;
    uint64_t wPawn;
    uint64_t bPawn;
    uint64_t wKing;
    uint64_t bKing;
}chessBoard;

void bitToFen(chessBoard board, char *fenString);
void printBitBoard(const char *fen);
void fenToBit(chessBoard *board, char *fenString);
int gameOver(chessBoard board);


