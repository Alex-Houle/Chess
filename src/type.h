#ifndef TYPE_H
#define TYPE_H
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>


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

  uint64_t EmptySquares;
} gameState;

#endif
