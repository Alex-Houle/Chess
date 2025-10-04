#ifndef RULES_H
#define RULES_H

#include "type.h"

int positionToBit(ChessPosition pos);
ChessPosition bitToPosition(int bit);
void bitboardToPositions(uint64_t bitboard, ChessPosition* out, int* count);
uint64_t rookMovesBB(ChessPosition position, uint64_t occupied);
uint64_t bishopMovesBB(ChessPosition position, uint64_t occupied);
uint64_t queenMovesBB(ChessPosition pos, uint64_t occ);
uint64_t knightMovesBB(ChessPosition position);
uint64_t kingMovesBB(ChessPosition pos);
uint64_t whitePawnMovesBB(ChessPosition pos, uint64_t empty, uint64_t enemy, int enPassantSquare);
uint64_t blackPawnMovesBB(ChessPosition pos, uint64_t empty, uint64_t enemy, int enPassantSquare);
bool isKingInCheck(chessBoard* board, char playerColor);
bool hasLegalMoves(chessBoard* board, gameState* game);
bool isSquareAttacked(chessBoard* board, int square, char byColor);

#endif  