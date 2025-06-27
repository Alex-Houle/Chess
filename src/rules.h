# ifndef RULES_H
#define RULES_H

#include <stdint.h>
#include "type.h" // for ChessPosition

uint64_t whitePawnMovesBB(ChessPosition pos, uint64_t empty, uint64_t enemy);
uint64_t knightMovesBB(ChessPosition pos);
uint64_t bishopMovesBB(ChessPosition pos, uint64_t occupied);
uint64_t rookMovesBB(ChessPosition pos, uint64_t occupied);
uint64_t queenMovesBB(ChessPosition pos, uint64_t occupied);
uint64_t kingMovesBB(ChessPosition pos);

#endif