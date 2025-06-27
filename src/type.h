#ifndef TYPE_H
#define TYPE_H
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define BIT(sq) (1ULL << (sq))


typedef struct {
    uint64_t lower;   
    uint64_t upper;   
    uint64_t lineEx;  
} SMasks;

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

  uint64_t empty;

  uint64_t blackPeices;
  uint64_t whitePeices; 

} chessBoard;

typedef struct {
  bool wKingCastle;
  bool wQueenCastle;

  bool bKingCastle;
  bool bQueenCastle;

  char toMove;

} gameState;

// Files (columns)
static const uint64_t FILE_A = 0x0101010101010101ULL;
static const uint64_t FILE_B = 0x0202020202020202ULL;
static const uint64_t FILE_C = 0x0404040404040404ULL;
static const uint64_t FILE_D = 0x0808080808080808ULL;
static const uint64_t FILE_E = 0x1010101010101010ULL;
static const uint64_t FILE_F = 0x2020202020202020ULL;
static const uint64_t FILE_G = 0x4040404040404040ULL;
static const uint64_t FILE_H = 0x8080808080808080ULL;

// Ranks (rows)
static const uint64_t RANK_1 = 0x00000000000000FFULL;
static const uint64_t RANK_2 = 0x000000000000FF00ULL;
static const uint64_t RANK_3 = 0x0000000000FF0000ULL;
static const uint64_t RANK_4 = 0x00000000FF000000ULL;
static const uint64_t RANK_5 = 0x000000FF00000000ULL;
static const uint64_t RANK_6 = 0x0000FF0000000000ULL;
static const uint64_t RANK_7 = 0x00FF000000000000ULL;
static const uint64_t RANK_8 = 0xFF00000000000000ULL;

#endif
