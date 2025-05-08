<LeftMouse>#include "rules.h"
#include "type.h"
#include <stdbool.h>
#include <stdlib.h>

static bool isSquareOnBoard(int square) {
    return square >= 0 && square < 64;
}

static int getPieceType(chessBoard* board, int square) {
    if (!isSquareOnBoard(square)) return EMPTY;
    
    uint64_t bit = 1ULL << square;
    
    if (board->whitePawns & bit) return WHITE_PAWN;
    if (board->whiteKnights & bit) return WHITE_KNIGHT;
    if (board->whiteBishops & bit) return WHITE_BISHOP;
    if (board->whiteRooks & bit) return WHITE_ROOK;
    if (board->whiteQueens & bit) return WHITE_QUEEN;
    if (board->whiteKing & bit) return WHITE_KING;
    
    if (board->blackPawns & bit) return BLACK_PAWN;
    if (board->blackKnights & bit) return BLACK_KNIGHT;
    if (board->blackBishops & bit) return BLACK_BISHOP;
    if (board->blackRooks & bit) return BLACK_ROOK;
    if (board->blackQueens & bit) return BLACK_QUEEN;
    if (board->blackKing & bit) return BLACK_KING;
    
    return EMPTY;
}

static void generatePawnMoves(chessBoard* board, int square, MoveList* moveList, bool isWhite) {
    int direction = isWhite ? 8 : -8;
    int startRank = isWhite ? 1 : 6;
    int promotionRank = isWhite ? 7 : 0;
    int currentRank = square / 8;
    
    int target = square + direction;
    if (isSquareOnBoard(target) && getPieceType(board, target) == EMPTY) {
        if (currentRank == promotionRank - 1) {
            addMove(moveList, createMove(square, target, PROMOTION_QUEEN));
            addMove(moveList, createMove(square, target, PROMOTION_ROOK));
            addMove(moveList, createMove(square, target, PROMOTION_BISHOP));
            addMove(moveList, createMove(square, target, PROMOTION_KNIGHT));
        } else {
            addMove(moveList, createMove(square, target, NORMAL));
            
            if (currentRank == startRank) {
                target = square + 2 * direction;
                if (getPieceType(board, target) == EMPTY) {
                    addMove(moveList, createMove(square, target, PAWN_DOUBLE));
                }
            }
        }
    }
    
    int captureDirs[] = {direction + 1, direction - 1};
    for (int i = 0; i < 2; i++) {
        target = square + captureDirs[i];
        if (isSquareOnBoard(target)) {
            if (abs((target % 8) - (square % 8)) == 1) {
                int targetPiece = getPieceType(board, target);
                if ((isWhite && targetPiece >= BLACK_PAWN && targetPiece <= BLACK_KING) ||
                    (!isWhite && targetPiece >= WHITE_PAWN && targetPiece <= WHITE_KING)) {
                    if (currentRank == promotionRank - 1) {
                        // Capture with promotion
                        addMove(moveList, createMove(square, target, PROMOTION_CAPTURE_QUEEN));
                        // ... add other promotion types
                    } else {
                        addMove(moveList, createMove(square, target, CAPTURE));
                    }
                }
                // TODO: Add en passant logic
            }
        }
    }
}

void generateLegalMoves(chessBoard* board, gameState* state, MoveList* moveList) {
    moveList->count = 0;
    
    for (int square = 0; square < 64; square++) {
        int piece = getPieceType(board, square);
        if (piece == EMPTY) continue;
        
        bool isWhite = piece <= WHITE_KING;
        
        switch (piece) {
            case WHITE_PAWN:
            case BLACK_PAWN:
                generatePawnMoves(board, square, moveList, isWhite);
                break;
            case WHITE_KNIGHT:
            case BLACK_KNIGHT:
                // TODO: Implement knight moves
                break;
            // TODO: Implement other pieces
            default:
                break;
        }
    }
    
    // TODO: Add castling moves
    // TODO: Filter out moves that leave king in check
}

bool isMoveLegal(chessBoard* board, gameState* state, Move move) {
    MoveList moveList;
    generateLegalMoves(board, state, &moveList);
    
    for (int i = 0; i < moveList.count; i++) {
        if (moveList.moves[i].from == move.from &&
            moveList.moves[i].to == move.to &&
            moveList.moves[i].flag == move.flag) {
            return true;
        }
    }
    return false;
}      
