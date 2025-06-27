#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "type.h"

// Constants for file and rank masks
#define FILE_A 0x0101010101010101ULL
#define FILE_H 0x8080808080808080ULL
#define RANK_1 0x00000000000000FFULL
#define RANK_2 0x000000000000FF00ULL
#define RANK_7 0x00FF000000000000ULL
#define RANK_8 0xFF00000000000000ULL


typedef struct {
    ChessPosition from;
    ChessPosition to;
    char piece;
    char captured;
    char promotion;
    bool isEnPassant;
    bool isCastling;
} ChessMove;

int positionToBit(ChessPosition pos) {
    return pos.row * 8 + pos.col;
}

ChessPosition bitToPosition(int bit) {
    ChessPosition pos = { bit / 8, bit % 8 };
    return pos;
}

void bitboardToPositions(uint64_t bitboard, ChessPosition* out, int* count) {
    *count = 0;
    for (int i = 0; i < 64; i++) {
        if (bitboard & (1ULL << i)) {
            out[(*count)++] = bitToPosition(i);
        }
    }
}

uint64_t rookMovesBB(ChessPosition position, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    int bit = positionToBit(position);
    int r = position.row;
    int c = position.col;

    // Horizontal
    for (int i = c - 1; i >= 0; i--) {
        int idx = r * 8 + i;
        attacks |= (1ULL << idx);
        if (occupied & (1ULL << idx)) break;
    }
    for (int i = c + 1; i < 8; i++) {
        int idx = r * 8 + i;
        attacks |= (1ULL << idx);
        if (occupied & (1ULL << idx)) break;
    }

    // Vertical
    for (int i = r - 1; i >= 0; i--) {
        int idx = i * 8 + c;
        attacks |= (1ULL << idx);
        if (occupied & (1ULL << idx)) break;
    }
    for (int i = r + 1; i < 8; i++) {
        int idx = i * 8 + c;
        attacks |= (1ULL << idx);
        if (occupied & (1ULL << idx)) break;
    }

    return attacks;
}

uint64_t bishopMovesBB(ChessPosition position, uint64_t occupied) {
    uint64_t attacks = 0ULL;
    int r = position.row;
    int c = position.col;

    int dr[] = { 1, 1, -1, -1 };
    int dc[] = { 1, -1, 1, -1 };

    for (int d = 0; d < 4; d++) {
        int nr = r + dr[d];
        int nc = c + dc[d];
        while (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            int idx = nr * 8 + nc;
            attacks |= (1ULL << idx);
            if (occupied & (1ULL << idx)) break;
            nr += dr[d];
            nc += dc[d];
        }
    }

    return attacks;
}

uint64_t queenMovesBB(ChessPosition pos, uint64_t occ) {
    return rookMovesBB(pos, occ) | bishopMovesBB(pos, occ);
}

uint64_t knightMovesBB(ChessPosition position) {
    int bit = positionToBit(position);
    uint64_t knight = 1ULL << bit;

    uint64_t l1 = (knight >> 1) & 0x7f7f7f7f7f7f7f7fULL;
    uint64_t l2 = (knight >> 2) & 0x3f3f3f3f3f3f3f3fULL;
    uint64_t r1 = (knight << 1) & 0xfefefefefefefefeULL;
    uint64_t r2 = (knight << 2) & 0xfcfcfcfcfcfcfcfcULL;

    uint64_t h1 = l1 | r1;
    uint64_t h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

uint64_t kingMovesBB(ChessPosition pos) {
    uint64_t king = 1ULL << positionToBit(pos);
    uint64_t notA = ~FILE_A;
    uint64_t notH = ~FILE_H;

    uint64_t attacks = 0;
    attacks |= (king & notA) >> 1;
    attacks |= (king & notH) << 1;
    attacks |= (king & notA) >> 9;
    attacks |= king >> 8;
    attacks |= (king & notH) >> 7;
    attacks |= (king & notA) << 7;
    attacks |= king << 8;
    attacks |= (king & notH) << 9;

    return attacks;
}

uint64_t whitePawnMovesBB(ChessPosition pos, uint64_t empty, uint64_t enemy) {
    uint64_t moves = 0ULL;
    int row = pos.row;
    int col = pos.col;
    int bit = positionToBit(pos);

    uint64_t from = 1ULL << bit;

    // Forward move (1 square)
    uint64_t one_step = (from << 8) & empty;
    moves |= one_step;

    // Double move from rank 2
    if (row == 1 && one_step) {
        uint64_t two_step = (from << 16) & empty & (empty << 8);
        moves |= two_step;
    }

    // Captures
    if (col > 0) moves |= (from << 7) & enemy; // capture left
    if (col < 7) moves |= (from << 9) & enemy; // capture right

    return moves;
}


// promotion, castling, en passant logic should be added 
