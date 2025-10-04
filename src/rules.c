#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // For abs
#include "type.h"
#include "rules.h"
#include "board.h"


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

    uint64_t attacks = 0ULL;

    // 1 up, 2 left/right
    attacks |= (knight << 10) & ~FILE_G & ~FILE_H; 
    attacks |= (knight << 6) & ~FILE_A & ~FILE_B;

    // 2 up, 1 left/right
    attacks |= (knight << 17) & ~FILE_H;
    attacks |= (knight << 15) & ~FILE_A;

    // 1 down, 2 left/right
    attacks |= (knight >> 6) & ~FILE_A & ~FILE_B;
    attacks |= (knight >> 10) & ~FILE_G & ~FILE_H;

    // 2 down, 1 left/right
    attacks |= (knight >> 15) & ~FILE_H;
    attacks |= (knight >> 17) & ~FILE_A;
    
    return attacks;
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

uint64_t whitePawnMovesBB(ChessPosition pos, uint64_t empty, uint64_t enemy, int enPassantSquare) {
    uint64_t moves = 0ULL;
    int row = pos.row;
    int col = pos.col;
    int bit = positionToBit(pos);

    uint64_t from = 1ULL << bit;

    // Forward move (1 square)
    uint64_t one_step = (from << 8) & empty;
    moves |= one_step;

    // Double move from rank 2
    if (row == 1) { // Rank 2
        uint64_t two_step_test = from << 8;
        if ((two_step_test & empty)) { // Check one step is empty
            uint64_t two_step = (from << 16) & empty;
            moves |= two_step;
        }
    }

    // Captures
    if (col > 0) moves |= (from << 7) & enemy; // capture left (to the W king's perspective)
    if (col < 7) moves |= (from << 9) & enemy; // capture right

    // En passant logic
    if (enPassantSquare != -1) {
        uint64_t ep_mask = 1ULL << enPassantSquare;
        // Check diagonal attacks against the en passant target square
        if (col > 0) moves |= (from << 7) & ep_mask; // ep capture left
        if (col < 7) moves |= (from << 9) & ep_mask; // ep capture right
    }

    return moves;
}

uint64_t blackPawnMovesBB(ChessPosition pos, uint64_t empty, uint64_t enemy, int enPassantSquare) {
    uint64_t moves = 0ULL;
    int row = pos.row;
    int col = pos.col;
    int bit = positionToBit(pos);

    uint64_t from = 1ULL << bit;

    uint64_t one_step = (from >> 8) & empty;
    moves |= one_step;

    if (row == 6) { // Rank 7
        uint64_t two_step_test = from >> 8;
        if ((two_step_test & empty)) { // Check one step is empty
            uint64_t two_step = (from >> 16) & empty;
            moves |= two_step;
        }
    }

    if (col > 0) moves |= (from >> 9) & enemy; // capture left
    if (col < 7) moves |= (from >> 7) & enemy; // capture right

    // En passant logic
    if (enPassantSquare != -1) {
        uint64_t ep_mask = 1ULL << enPassantSquare;
        // Check diagonal attacks against the en passant target square
        if (col > 0) moves |= (from >> 9) & ep_mask; // ep capture left
        if (col < 7) moves |= (from >> 7) & ep_mask; // ep capture right
    }

    return moves;
}

// Helper function to check if a square is attacked by a given color
bool isSquareAttacked(chessBoard* board, int square, char byColor) {
    uint64_t occupied = board->whitePeices | board->blackPeices;
    ChessPosition pos = bitToPosition(square);
    uint64_t target_sq = 1ULL << square;

    uint64_t attackerPawn, attackerKnight, attackerRookQueen, attackerBishopQueen, attackerKing;
    
    // Determine which side's pieces are the attackers
    if (byColor == 'w') {
        attackerPawn = board->wPawn;
        attackerKnight = board->wKnight;
        attackerRookQueen = board->wRook | board->wQueen;
        attackerBishopQueen = board->wBishop | board->wQueen;
        attackerKing = board->wKing;
    } else {
        attackerPawn = board->bPawn;
        attackerKnight = board->bKnight;
        attackerRookQueen = board->bRook | board->bQueen;
        attackerBishopQueen = board->bBishop | board->bQueen;
        attackerKing = board->bKing;
    }

    // Pawn attacks: Check if a pawn on the attacking rank could attack this square
    if (byColor == 'w') { 
        if ((target_sq >> 9) & attackerPawn) return true; 
        if ((target_sq >> 7) & attackerPawn) return true; 
    } else { 
        if ((target_sq << 9) & attackerPawn) return true; 
        if ((target_sq << 7) & attackerPawn) return true; 
    }

    // Knight attacks
    if (knightMovesBB(pos) & attackerKnight) return true;

    // Sliding pieces use the actual occupied board
    if (rookMovesBB(pos, occupied) & attackerRookQueen) return true;
    if (bishopMovesBB(pos, occupied) & attackerBishopQueen) return true;

    // King attacks
    if (kingMovesBB(pos) & attackerKing) return true;

    return false;
}

bool isKingInCheck(chessBoard* board, char playerColor) {
    uint64_t kingPosition;
    char enemyColor = (playerColor == 'w') ? 'b' : 'w';

    if (playerColor == 'w') {
        kingPosition = board->wKing;
    } else {
        kingPosition = board->bKing;
    }

    int kingSquare = __builtin_ctzll(kingPosition); // Find the index of the king's bit

    return isSquareAttacked(board, kingSquare, enemyColor);
}

bool hasLegalMoves(chessBoard* board, gameState* game) {
    uint64_t playerPieces;
    char enemyColor = (game->toMove == 'w') ? 'b' : 'w';
    uint64_t* kingBB = (game->toMove == 'w') ? &board->wKing : &board->bKing;
    uint64_t* pawnBB = (game->toMove == 'w') ? &board->wPawn : &board->bPawn;
    uint64_t* knightBB = (game->toMove == 'w') ? &board->wKnight : &board->bKnight;
    uint64_t* bishopBB = (game->toMove == 'w') ? &board->wBishop : &board->bBishop;
    uint64_t* rookBB = (game->toMove == 'w') ? &board->wRook : &board->bRook;
    uint64_t* queenBB = (game->toMove == 'w') ? &board->wQueen : &board->bQueen;


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
            ChessPosition from_pos = bitToPosition(from_square);
            uint64_t occupied = board->whitePeices | board->blackPeices;

            if (*pawnBB & BIT(from_square)) {
                if (game->toMove == 'w')
                    moves_bb = whitePawnMovesBB(from_pos, board->empty, board->blackPeices, game->enPassantSquare);
                else 
                    moves_bb = blackPawnMovesBB(from_pos, board->empty, board->whitePeices, game->enPassantSquare);
            } else if (*knightBB & BIT(from_square)) {
                moves_bb = knightMovesBB(from_pos);
            } else if (*bishopBB & BIT(from_square)) {
                moves_bb = bishopMovesBB(from_pos, occupied);
            } else if (*rookBB & BIT(from_square)) {
                moves_bb = rookMovesBB(from_pos, occupied);
            } else if (*queenBB & BIT(from_square)) {
                moves_bb = queenMovesBB(from_pos, occupied);
            } else if (*kingBB & BIT(from_square)) {
                moves_bb = kingMovesBB(from_pos);
                
                // --- Add Castling Moves (Legality Check) ---
                if (game->toMove == 'w' && from_square == 4) { // White King on E1
                    // Kingside (E1 to G1, target 6)
                    if (game->wKingCastle && (BIT(7) & board->wRook) && (BIT(5) & board->empty) && (BIT(6) & board->empty)) {
                        if (!isSquareAttacked(board, 4, enemyColor) && !isSquareAttacked(board, 5, enemyColor) && !isSquareAttacked(board, 6, enemyColor)) {
                            moves_bb |= BIT(6);
                        }
                    }
                    // Queenside (E1 to C1, target 2)
                    if (game->wQueenCastle && (BIT(0) & board->wRook) && (BIT(3) & board->empty) && (BIT(2) & board->empty) && (BIT(1) & board->empty)) {
                        if (!isSquareAttacked(board, 4, enemyColor) && !isSquareAttacked(board, 3, enemyColor) && !isSquareAttacked(board, 2, enemyColor)) {
                            moves_bb |= BIT(2);
                        }
                    }
                } else if (game->toMove == 'b' && from_square == 60) { // Black King on E8
                    // Kingside (E8 to G8, target 62)
                    if (game->bKingCastle && (BIT(63) & board->bRook) && (BIT(61) & board->empty) && (BIT(62) & board->empty)) {
                        if (!isSquareAttacked(board, 60, enemyColor) && !isSquareAttacked(board, 61, enemyColor) && !isSquareAttacked(board, 62, enemyColor)) {
                            moves_bb |= BIT(62);
                        }
                    }
                    // Queenside (E8 to C8, target 58)
                    if (game->bQueenCastle && (BIT(56) & board->bRook) && (BIT(59) & board->empty) && (BIT(58) & board->empty) && (BIT(57) & board->empty)) {
                        if (!isSquareAttacked(board, 60, enemyColor) && !isSquareAttacked(board, 59, enemyColor) && !isSquareAttacked(board, 58, enemyColor)) {
                            moves_bb |= BIT(58);
                        }
                    }
                }
            }
            
            moves_bb &= ~playerPieces;
            
            uint64_t temp_moves_bb = moves_bb;
            while (temp_moves_bb != 0) {
                int to_square = __builtin_ctzll(temp_moves_bb);
                temp_moves_bb &= temp_moves_bb - 1;

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