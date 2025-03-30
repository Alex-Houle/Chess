#include <stdio.h>
#include "board.h"


int main() {
    /* 
    chessBoard board;
    board.wBishop = 0x0000000000000024ULL;
    board.wRook = 0x0000000000000081ULL;
    board.wKnight = 0x0000000000000042ULL;
    board.wQueen = 0x0000000000000008ULL;
    board.wKing = 0x0000000000000010ULL;
    board.wPawn = 0x000000000000FF00ULL;
    board.bBishop = 0x2400000000000000ULL;
    board.bRook = 0x8100000000000000ULL;
    board.bKnight = 0x4200000000000000ULL;
    board.bQueen = 0x0800000000000000ULL;
    board.bKing = 0x1000000000000000ULL;
    board.bPawn = 0x00FF000000000000ULL;
    */
    chessBoard board;
    char fenString[64];

    printf("Give the fen string: ");
    scanf("%s", fenString);
    fenToBit(&board, fenString);
    printBitBoard(board);
    return 0;
}