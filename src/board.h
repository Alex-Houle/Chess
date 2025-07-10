#ifndef BOARD_H
#define BOARD_H
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "type.h"

void fenToBit(chessBoard* board, char* fenString);
void bitToFen(chessBoard* board, char* output);
void printBitBoard(chessBoard* board);
int gameover(chessBoard* board);
void gameStart(chessBoard*, gameState*);
void move(chessBoard* , gameState* , int , int );
void print_board(uint64_t bitboard);

#endif 