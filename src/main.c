#include <stdio.h>
#include "board.h"


int main() {
    chessBoard board;
    board.wBishop = 0x0000000000000024;
    board.wRook = 0x0000000000000081;
    board.wKnight = 0x0000000000000042;
    board.wQueen = 0x0000000000000008;
    board.bBishop = 0x2400000000000000;
    board.bRook = 0x8100000000000000;
    board.bKnight = 0x4200000000000000;
    board.bQueen = 0x0800000000000000;
    char fenString[64];
    bitToFen(board, fenString);
    printf("%s",fenString);
    return 0;
}


/**
 * Take a fen stirng and converts the fenString into bitboards
 */
void fenToBit() {

}

//Take bit boards and converts them to a fenString 
void bitToFen(chessBoard board, char *fenString) {
    int index = 0;
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0 && i != 0) {
            fenString[index] = '/';
            index++;
        }
        if (board.wBishop & (1 << i)) {
            fenString[index] = 'B';
            index++;
        } else if (board.wRook & (1 << i)) {
            fenString[index] = 'R';
            index++;
        } else if (board.wKnight & (1 << i)) {
            fenString[index] = 'N';
            index++;
        } else if (board.wQueen & (1 << i)) {
            fenString[index] = 'Q';
            index++;
        } else if (board.bBishop & (1 << i)) {
            fenString[index] = 'b';
            index++;
        } else if (board.bRook & (1 << i)) {
            fenString[index] = 'r';
            index++;
        } else if (board.bKnight & (1 << i)) {
            fenString[index] = 'n';
            index++;
        } else if (board.bQueen & (1 << i)) {
            fenString[index] = 'q';
            index++;
        } else {
            fenString[index] = '1';
            index++;
        }
    }
    fenString[index] = '\0';
    printf("%s\n", fenString);
    return;
}