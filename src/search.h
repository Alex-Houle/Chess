#ifndef SEARCH_H
#define SEARCH_H

#include "type.h"
#include "board.h"
#include "rules.h"
#include "eval.h"

// Finds the best move for the current player using a search algorithm.
Move findBestMove(chessBoard* board, gameState* game, int depth);

#endif // SEARCH_H
