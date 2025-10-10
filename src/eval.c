#include "eval.h"
#include "type.h"
#include <stdio.h>
#include <stdint.h>

// --- Piece Square Table Definitions (From user request) ---

const int PAWN_EVAL_WHITE[8][8] = {
    // Rank 1 (R0 - White's home rank)
    {0, 0, 0, 0, 0, 0, 0, 0},
    // Rank 2 (R1)
    {5, 10, 10, -20, -20, 10, 10, 5},
    // Rank 3 (R2)
    {5, -5, -10, 0, 0, -10, -5, 5},
    // Rank 4 (R3)
    {0, 0, 0, 20, 20, 0, 0, 0},
    // Rank 5 (R4)
    {5, 5, 10, 25, 25, 10, 5, 5},
    // Rank 6 (R5)
    {10, 10, 20, 30, 30, 20, 10, 10},
    // Rank 7 (R6)
    {50, 50, 50, 50, 50, 50, 50, 50},
    // Rank 8 (R7 - White's promotion rank)
    {0, 0, 0, 0, 0, 0, 0, 0}
};

const int PAWN_EVAL_BLACK[8][8] = {
    // Rank 1 (R0) - Black's starting rank (White's R8)
    {0, 0, 0, 0, 0, 0, 0, 0},
    // Rank 2 (R1)
    {50, 50, 50, 50, 50, 50, 50, 50},
    // Rank 3 (R2)
    {10, 10, 20, 30, 30, 20, 10, 10},
    // Rank 4 (R3)
    {5, 5, 10, 25, 25, 10, 5, 5},
    // Rank 5 (R4)
    {0, 0, 0, 20, 20, 0, 0, 0},
    // Rank 6 (R5)
    {5, -5, -10, 0, 0, -10, -5, 5},
    // Rank 7 (R6)
    {5, 10, 10, -20, -20, 10, 10, 5},
    // Rank 8 (R7) - Black's promotion rank (White's R1)
    {0, 0, 0, 0, 0, 0, 0, 0}
};

const int KNIGHT_EVAL[8][8] = {
    {-50, -40, -30, -30, -30, -30, -40, -50},
    {-40, -20, 0, 5, 5, 0, -20, -40},
    {-30, 5, 10, 15, 15, 10, 5, -30},
    {-30, 0, 15, 20, 20, 15, 0, -30},
    {-30, 5, 15, 20, 20, 15, 5, -30},
    {-30, 0, 10, 15, 15, 10, 0, -30},
    {-40, -20, 0, 0, 0, 0, -20, -40},
    {-50, -40, -30, -30, -30, -30, -40, -50}
};

const int BISHOP_EVAL_WHITE[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10, 5, 0, 0, 0, 0, 5, -10},
    {-10, 10, 10, 10, 10, 10, 10, -10},
    {-10, 0, 10, 10, 10, 10, 0, -10},
    {-10, 5, 5, 10, 10, 5, 5, -10},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-10, 0, 0, 0, 0, 0, 0, -10},
    {-20, -10, -10, -10, -10, -10, -10, -20}
};

const int BISHOP_EVAL_BLACK[8][8] = {
    {-20, -10, -10, -10, -10, -10, -10, -20},
    {-10, 0, 0, 0, 0, 0, 0, -10},
    {-10, 0, 5, 10, 10, 5, 0, -10},
    {-10, 5, 5, 10, 10, 5, 5, -10},
    {-10, 0, 10, 10, 10, 10, 0, -10},
    {-10, 10, 10, 10, 10, 10, 10, -10},
    {-10, 5, 0, 0, 0, 0, 5, -10},
    {-20, -10, -10, -10, -10, -10, -10, -20}
};

const int ROOK_EVAL_WHITE[8][8] = {
    {0, 0, 0, 5, 5, 0, 0, 0},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {5, 10, 10, 10, 10, 10, 10, 5},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

const int ROOK_EVAL_BLACK[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {5, 10, 10, 10, 10, 10, 10, 5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5},
    {0, 0, 0, 5, 5, 0, 0, 0}
};

const int QUEEN_EVAL[8][8] = {
    {-20, -10, -10, -5, -5, -10, -10, -20},
    {-10, 0, 5, 0, 0, 0, 0, -10},
    {-10, 5, 5, 5, 5, 5, 0, -10},
    {0, 0, 5, 5, 5, 5, 0, -5},
    {-5, 0, 5, 5, 5, 5, 0, -5},
    {-10, 0, 5, 5, 5, 5, 0, -10},
    {-10, 0, 0, 0, 0, 0, 0, -10},
    {-20, -10, -10, -5, -5, -10, -10, -20}
};

const int KING_EVAL_WHITE[8][8] = {
    {20, 30, 10, 0, 0, 10, 30, 20},
    {20, 20, 0, 0, 0, 0, 20, 20},
    {-10, -20, -20, -20, -20, -20, -20, -10},
    {-20, -30, -30, -40, -40, -30, -30, -20},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30}
};

const int KING_EVAL_BLACK[8][8] = {
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-30, -40, -40, -50, -50, -40, -40, -30},
    {-20, -30, -30, -40, -40, -30, -30, -20},
    {-10, -20, -20, -20, -20, -20, -20, -10},
    {20, 20, 0, 0, 0, 0, 20, 20},
    {20, 30, 10, 0, 0, 10, 30, 20}
};


/**
 * Calculates the static evaluation of the current position.
 * The score is returned from White's perspective (positive is good for White).
 * Evaluation = (White Material + White Positional Score) - (Black Material + Black Positional Score)
 * * @param board The current state of the chess board.
 * @return The evaluation score (in centipawns).
 */
int evaluatePosition(const chessBoard* board) {
    int evaluation = 0;
    
    // --- Material Value (White) ---
    evaluation += __builtin_popcountll(board->wPawn) * VALUE_PAWN;
    evaluation += __builtin_popcountll(board->wKnight) * VALUE_KNIGHT;
    evaluation += __builtin_popcountll(board->wBishop) * VALUE_BISHOP;
    evaluation += __builtin_popcountll(board->wRook) * VALUE_ROOK;
    evaluation += __builtin_popcountll(board->wQueen) * VALUE_QUEEN;
    evaluation += __builtin_popcountll(board->wKing) * VALUE_KING;
    
    // --- Material Value (Black) ---
    evaluation -= __builtin_popcountll(board->bPawn) * VALUE_PAWN;
    evaluation -= __builtin_popcountll(board->bKnight) * VALUE_KNIGHT;
    evaluation -= __builtin_popcountll(board->bBishop) * VALUE_BISHOP;
    evaluation -= __builtin_popcountll(board->bRook) * VALUE_ROOK;
    evaluation -= __builtin_popcountll(board->bQueen) * VALUE_QUEEN;
    evaluation -= __builtin_popcountll(board->bKing) * VALUE_KING;
    
    
    // --- Positional Value (Piece Square Tables) ---
    
    // Use an index loop from a1 (0) to h8 (63)
    for (int square = 0; square < 64; square++) {
        uint64_t mask = 1ULL << square;
        int rank = square / 8; // Row index
        int file = square % 8; // Column index
        
        // White Pieces (Add positional score)
        if (board->whitePeices & mask) {
            if (board->wPawn & mask) {
                evaluation += PAWN_EVAL_WHITE[rank][file];
            } else if (board->wKnight & mask) {
                evaluation += KNIGHT_EVAL[rank][file];
            } else if (board->wBishop & mask) {
                evaluation += BISHOP_EVAL_WHITE[rank][file];
            } else if (board->wRook & mask) {
                evaluation += ROOK_EVAL_WHITE[rank][file];
            } else if (board->wQueen & mask) {
                evaluation += QUEEN_EVAL[rank][file];
            } else if (board->wKing & mask) {
                evaluation += KING_EVAL_WHITE[rank][file];
            }
        }
        
        // Black Pieces (Subtract positional score)
        else if (board->blackPeices & mask) {
            if (board->bPawn & mask) {
                evaluation -= PAWN_EVAL_BLACK[rank][file];
            } else if (board->bKnight & mask) {
                evaluation -= KNIGHT_EVAL[rank][file];
            } else if (board->bBishop & mask) {
                evaluation -= BISHOP_EVAL_BLACK[rank][file];
            } else if (board->bRook & mask) {
                evaluation -= ROOK_EVAL_BLACK[rank][file];
            } else if (board->bQueen & mask) {
                evaluation -= QUEEN_EVAL[rank][file];
            } else if (board->bKing & mask) {
                evaluation -= KING_EVAL_BLACK[rank][file];
            }
        }
    }
    

    return evaluation;
}
