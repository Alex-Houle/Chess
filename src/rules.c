#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "type.h"
#include "rules.h"
#include "board.h"

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
    int r = position.row;
    int c = position.col;

    // Up
    for (int i = r + 1; i < 8; i++) {
        int idx = i * 8 + c;
        uint64_t mask = 1ULL << idx;
        attacks |= mask;
        if (occupied & mask) break;
    }
    // Down
    for (int i = r - 1; i >= 0; i--) {
        int idx = i * 8 + c;
        uint64_t mask = 1ULL << idx;
        attacks |= mask;
        if (occupied & mask) break;
    }
    // Right
    for (int i = c + 1; i < 8; i++) {
        int idx = r * 8 + i;
        uint64_t mask = 1ULL << idx;
        attacks |= mask;
        if (occupied & mask) break;
    }
    // Left
    for (int i = c - 1; i >= 0; i--) {
        int idx = r * 8 + i;
        uint64_t mask = 1ULL << idx;
        attacks |= mask;
        if (occupied & mask) break;
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
            uint64_t mask = 1ULL << idx;
            attacks |= mask;
            if (occupied & mask) break;
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

    // En passant logic needs information from previous move, which isn't in this function's scope.
    // It should be handled in the 'move' function when checking for a valid move.

    return moves;
}

uint64_t blackPawnMovesBB(ChessPosition pos, uint64_t empty, uint64_t enemy) {
    uint64_t moves = 0ULL;
    int row = pos.row;
    int col = pos.col;
    int bit = positionToBit(pos);

    uint64_t from = 1ULL << bit;

    uint64_t one_step = (from >> 8) & empty;
    moves |= one_step;

    if (row == 6 && one_step) {
        uint64_t two_step = (from >> 16) & empty & (empty >> 8);
        moves |= two_step;
    }

    if (col > 0) moves |= (from >> 9) & enemy;
    if (col < 7) moves |= (from >> 7) & enemy;

    // En passant logic needs information from previous move, which isn't in this function's scope.
    // It should be handled in the 'move' function when checking for a valid move.

    return moves;
}

bool isKingInCheck(chessBoard* board, char playerColor) {
    uint64_t kingPosition;

    if (playerColor == 'w') {
        kingPosition = board->wKing;
    } else {
        kingPosition = board->bKing;
    }

    int kingSquare = __builtin_ctzll(kingPosition); // Find the index of the king's bit

    // Check for attacks from opponent's pawns
    if (playerColor == 'w') {
        if ((BIT(kingSquare) >> 9) & board->bPawn && (kingSquare % 8 > 0)) return true;
        if ((BIT(kingSquare) >> 7) & board->bPawn && (kingSquare % 8 < 7)) return true;
    } else {
        if ((BIT(kingSquare) << 9) & board->wPawn && (kingSquare % 8 < 7)) return true;
        if ((BIT(kingSquare) << 7) & board->wPawn && (kingSquare % 8 > 0)) return true;
    }
    
    // Check for attacks from opponent's knights
    if (knightMovesBB(bitToPosition(kingSquare)) & ((playerColor == 'w') ? board->bKnight : board->wKnight)) {
        return true;
    }

    uint64_t occupied = board->whitePeices | board->blackPeices;
    // Check for attacks from opponent's rooks or queens
    if (rookMovesBB(bitToPosition(kingSquare), occupied) & ((playerColor == 'w') ? (board->bRook | board->bQueen) : (board->wRook | board->wQueen))) {
        return true;
    }

    // Check for attacks from opponent's bishops or queens
    if (bishopMovesBB(bitToPosition(kingSquare), occupied) & ((playerColor == 'w') ? (board->bBishop | board->bQueen) : (board->wBishop | board->wQueen))) {
        return true;
    }

    // Check for attacks from opponent's king
    if (kingMovesBB(bitToPosition(kingSquare)) & ((playerColor == 'w') ? board->bKing : board->wKing)) {
        return true;
    }

    return false;
}

bool hasLegalMoves(chessBoard* board, gameState* game) {
    uint64_t playerPieces;
    if (game->toMove == 'w') {
        playerPieces = board->whitePeices;
    } else {
        playerPieces = board->blackPeices;
    }

    int from_square;
    for (int i = 0; i < 64; i++) {
        if ((playerPieces >> i) & 1) {
            from_square = i;
            uint64_t moves_bb = 0;
            ChessPosition from_pos = {from_square / 8, from_square % 8};
            uint64_t occupied = board->whitePeices | board->blackPeices;

            if (game->toMove == 'w') {
                if (board->wPawn & BIT(from_square)) {
                    moves_bb = whitePawnMovesBB(from_pos, board->empty, board->blackPeices);
                } else if (board->wKnight & BIT(from_square)) {
                    moves_bb = knightMovesBB(from_pos);
                } else if (board->wBishop & BIT(from_square)) {
                    moves_bb = bishopMovesBB(from_pos, occupied);
                } else if (board->wRook & BIT(from_square)) {
                    moves_bb = rookMovesBB(from_pos, occupied);
                } else if (board->wQueen & BIT(from_square)) {
                    moves_bb = queenMovesBB(from_pos, occupied);
                } else if (board->wKing & BIT(from_square)) {
                    moves_bb = kingMovesBB(from_pos);
                }
                moves_bb &= ~board->whitePeices;
            } else { // Black's turn
                if (board->bPawn & BIT(from_square)) {
                    moves_bb = blackPawnMovesBB(from_pos, board->empty, board->whitePeices);
                } else if (board->bKnight & BIT(from_square)) {
                    moves_bb = knightMovesBB(from_pos);
                } else if (board->bBishop & BIT(from_square)) {
                    moves_bb = bishopMovesBB(from_pos, occupied);
                } else if (board->bRook & BIT(from_square)) {
                    moves_bb = rookMovesBB(from_pos, occupied);
                } else if (board->bQueen & BIT(from_square)) {
                    moves_bb = queenMovesBB(from_pos, occupied);
                } else if (board->bKing & BIT(from_square)) {
                    moves_bb = kingMovesBB(from_pos);
                }
                moves_bb &= ~board->blackPeices;
            }
            
            while (moves_bb != 0) {
                int to_square = __builtin_ctzll(moves_bb);
                moves_bb &= moves_bb - 1;

                chessBoard temp_board = *board;
                gameState temp_game = *game;
                move(&temp_board, &temp_game, from_square, to_square);

                if (!isKingInCheck(&temp_board, game->toMove)) {
                    return true; 
                }
            }
        }
    }
    return false;
}