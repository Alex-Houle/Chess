#include "search.h"
#include <limits.h>
#include <stdio.h> // For debugging, though generally avoid in search

// A simplified NegaMax function for move search
int negaMax(chessBoard* board, gameState* game, int depth, int alpha, int beta) {
    if (depth == 0) {
        // The evaluatePosition function is from White's perspective. 
        // We flip the score if it's Black's turn to maximize their benefit.
        int score = evaluatePosition(board);
        return (game->toMove == 'w') ? score : -score;
    }

    // Check for game end conditions (mate or draw)
    if (!hasLegalMoves(board, game)) {
        if (isKingInCheck(board, game->toMove)) {
            // Checkmate: return a very low score (adjusted for depth to prefer shorter mates)
            return INT_MIN + 1 + depth; 
        } else {
            // Stalemate: return a draw score
            return 0; 
        }
    }

    int max_eval = INT_MIN + 1;

    uint64_t playerPieces;
    char enemyColor = (game->toMove == 'w') ? 'b' : 'w';
    
    // Pointers to the current player's piece bitboards
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
        if ((playerPieces >> i) & 1) { // Found a piece belonging to the current player
            from_square = i;
            uint64_t moves_bb = 0;
            ChessPosition from_pos = bitToPosition(from_square);
            uint64_t occupied = board->whitePeices | board->blackPeices;
            
            // -------------------- Pseudolegal Move Generation --------------------
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
                
                // Add Castling Moves (Search needs to evaluate these too)
                if (game->toMove == 'w' && from_square == 4) { // White King on E1
                    if (game->wKingCastle && (BIT(7) & board->wRook) && (BIT(5) & board->empty) && (BIT(6) & board->empty)) {
                        if (!isSquareAttacked(board, 4, enemyColor) && !isSquareAttacked(board, 5, enemyColor) && !isSquareAttacked(board, 6, enemyColor)) {
                            moves_bb |= BIT(6); // G1
                        }
                    }
                    if (game->wQueenCastle && (BIT(0) & board->wRook) && (BIT(3) & board->empty) && (BIT(2) & board->empty) && (BIT(1) & board->empty)) {
                        if (!isSquareAttacked(board, 4, enemyColor) && !isSquareAttacked(board, 3, enemyColor) && !isSquareAttacked(board, 2, enemyColor)) {
                            moves_bb |= BIT(2); // C1
                        }
                    }
                } else if (game->toMove == 'b' && from_square == 60) { // Black King on E8
                    if (game->bKingCastle && (BIT(63) & board->bRook) && (BIT(61) & board->empty) && (BIT(62) & board->empty)) {
                        if (!isSquareAttacked(board, 60, enemyColor) && !isSquareAttacked(board, 61, enemyColor) && !isSquareAttacked(board, 62, enemyColor)) {
                            moves_bb |= BIT(62); // G8
                        }
                    }
                    if (game->bQueenCastle && (BIT(56) & board->bRook) && (BIT(59) & board->empty) && (BIT(58) & board->empty) && (BIT(57) & board->empty)) {
                        if (!isSquareAttacked(board, 60, enemyColor) && !isSquareAttacked(board, 59, enemyColor) && !isSquareAttacked(board, 58, enemyColor)) {
                            moves_bb |= BIT(58); // C8
                        }
                    }
                }
            }
            
            // Filter out moves that capture friendly pieces (captures on empty squares handled by move function)
            moves_bb &= ~playerPieces;
            
            uint64_t temp_moves_bb = moves_bb;
            while (temp_moves_bb != 0) {
                int to_square = __builtin_ctzll(temp_moves_bb);
                temp_moves_bb &= temp_moves_bb - 1; // Clear the least significant bit
                
                // --- Check Legality (King safety) and Recurse ---
                chessBoard temp_board = *board;
                gameState temp_game = *game;
                
                // Execute the move on the temporary board
                move(&temp_board, &temp_game, from_square, to_square);

                if (!isKingInCheck(&temp_board, game->toMove)) { // Legal move
                    int eval = -negaMax(&temp_board, &temp_game, depth - 1, -beta, -alpha);
                    
                    if (eval > max_eval) {
                        max_eval = eval;
                    }
                    
                    if (max_eval > alpha) {
                        alpha = max_eval;
                    }
                    
                    if (alpha >= beta) {
                        return alpha; // Beta cutoff
                    }
                }
            }
        }
    }
    
    return max_eval;
}

Move findBestMove(chessBoard* board, gameState* game, int depth) {
    Move bestMove = {-1, -1};
    int maxScore = INT_MIN + 1; // Initialize to smallest possible score

    uint64_t playerPieces;
    char enemyColor = (game->toMove == 'w') ? 'b' : 'w';
    
    // Pointers to the current player's piece bitboards
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
        if ((playerPieces >> i) & 1) { // Found a piece belonging to the current player
            from_square = i;
            uint64_t moves_bb = 0;
            ChessPosition from_pos = bitToPosition(from_square);
            uint64_t occupied = board->whitePeices | board->blackPeices;
            
            // -------------------- Pseudolegal Move Generation --------------------
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
                
                // Add Castling Moves (Legality checks for search are done inside the move loop)
                if (game->toMove == 'w' && from_square == 4) { // White King on E1
                    if (game->wKingCastle && (BIT(7) & board->wRook) && (BIT(5) & board->empty) && (BIT(6) & board->empty)) {
                        if (!isSquareAttacked(board, 4, enemyColor) && !isSquareAttacked(board, 5, enemyColor) && !isSquareAttacked(board, 6, enemyColor)) {
                            moves_bb |= BIT(6); // G1
                        }
                    }
                    if (game->wQueenCastle && (BIT(0) & board->wRook) && (BIT(3) & board->empty) && (BIT(2) & board->empty) && (BIT(1) & board->empty)) {
                        if (!isSquareAttacked(board, 4, enemyColor) && !isSquareAttacked(board, 3, enemyColor) && !isSquareAttacked(board, 2, enemyColor)) {
                            moves_bb |= BIT(2); // C1
                        }
                    }
                } else if (game->toMove == 'b' && from_square == 60) { // Black King on E8
                    if (game->bKingCastle && (BIT(63) & board->bRook) && (BIT(61) & board->empty) && (BIT(62) & board->empty)) {
                        if (!isSquareAttacked(board, 60, enemyColor) && !isSquareAttacked(board, 61, enemyColor) && !isSquareAttacked(board, 62, enemyColor)) {
                            moves_bb |= BIT(62); // G8
                        }
                    }
                    if (game->bQueenCastle && (BIT(56) & board->bRook) && (BIT(59) & board->empty) && (BIT(58) & board->empty) && (BIT(57) & board->empty)) {
                        if (!isSquareAttacked(board, 60, enemyColor) && !isSquareAttacked(board, 59, enemyColor) && !isSquareAttacked(board, 58, enemyColor)) {
                            moves_bb |= BIT(58); // C8
                        }
                    }
                }
            }
            
            moves_bb &= ~playerPieces;
            
            uint64_t temp_moves_bb = moves_bb;
            while (temp_moves_bb != 0) {
                int to_square = __builtin_ctzll(temp_moves_bb);
                temp_moves_bb &= temp_moves_bb - 1; // Clear the least significant bit

                // --- Check Legality (King safety) and Score Move ---
                chessBoard temp_board = *board;
                gameState temp_game = *game;
                
                // Execute the move on the temporary board
                move(&temp_board, &temp_game, from_square, to_square);

                if (!isKingInCheck(&temp_board, game->toMove)) { // Legal move
                    // We use INT_MIN and INT_MAX for the first level search (full window)
                    int score = -negaMax(&temp_board, &temp_game, depth - 1, INT_MIN, INT_MAX);
                    
                    if (score > maxScore) {
                        maxScore = score;
                        bestMove.fromSquare = from_square;
                        bestMove.toSquare = to_square;
                    }
                }
            }
        }
    }
    
    return bestMove; 
}
