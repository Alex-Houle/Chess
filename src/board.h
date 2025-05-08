#ifndef BOARD_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "type.h"

// Struct representing the chess board using bitboards
// Function declarations
void fenToBit(chessBoard* board, char* fenString);
void bitToFen(chessBoard* board, char* output);
void printBitBoard(chessBoard* board);
int gameover(chessBoard* board);
void gameStart(chessBoard*, gameState*);
void move(chessBoard* , gameState* , int , int );


#endif // BOARD_H
