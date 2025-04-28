#ifndef MOVES_H
#define MOVES_H


#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "types.h"





uint64_t pawnMoveW(uint64_t pawns, uint64_t empty);
uint64_t pawnMoveB(uint64_t pawns, uint64_t empty);
uint64_t knightMove(uint64_t knights);

uint64_t occupied(const chessBoard* board);

void generate_moves(const chessBoard* board, const gameState*);

#endif // MOVES_H


