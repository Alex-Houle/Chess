// src/main.c
#include "board.h"
#include "type.h"
#include "rules.h"
#include "sdl_utils.h"
#include <stdbool.h>
#include <stdlib.h> 
#include "search.h" // Changed from <search.h> to "search.h"

#define AI_SEARCH_DEPTH 3 // Define a search depth for the AI

int main() {
    chessBoard *board = malloc(sizeof(chessBoard));
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    gameState *game = malloc(sizeof(gameState));
    if (game == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    gameStart(board, game);
    
    initializeSDL();
    loadPieceTextures();
    
    SDL_Event e;
    int selectedSquare = -1;
    bool isGameOver = false;
    
    while (!isGameOver) {
        // --- AI Turn Logic (Black) ---
        if (game->toMove == 'b' && !isGameOver) {
            printf("AI (Black) is thinking at depth %d...\n", AI_SEARCH_DEPTH);
            Move ai_move = findBestMove(board, game, AI_SEARCH_DEPTH);

            if (ai_move.fromSquare != -1) {
                // Execute the AI's move
                move(board, game, ai_move.fromSquare, ai_move.toSquare);
                printf("AI moved: %d to %d\n", ai_move.fromSquare, ai_move.toSquare);
                
                // Check for game end conditions after AI move
                if (!hasLegalMoves(board, game)) {
                    if (isKingInCheck(board, game->toMove)) {
                        printf("Checkmate! %s wins!\n", game->toMove == 'w' ? "Black" : "White");
                        isGameOver = true;
                    } else {
                        printf("Stalemate! Game is a draw.\n");
                        isGameOver = true;
                    }
                }
            } else {
                 // This should only happen if findBestMove fails to find a legal move
                if (!hasLegalMoves(board, game)) {
                    printf("Game Over: No legal moves for Black (should have been caught earlier).\n");
                } else {
                    printf("Error: AI returned no move despite legal moves existing.\n");
                }
                isGameOver = true; // Terminate if AI fails to move
            }
        }
        
        // --- Human Turn Logic (White) ---
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                isGameOver = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Only process human moves if it's White's turn
                if (e.button.button == SDL_BUTTON_LEFT && game->toMove == 'w' && !isGameOver) {
                    int x = e.button.x / SQUARE_SIZE;
                    // Y-axis is inverted for rendering, so we flip it for bitboard indexing
                    int y = 7 - (e.button.y / SQUARE_SIZE); 
                    int clickedSquare = y * 8 + x;
                    
                    printf("Clicked square: %d (row: %d, col: %d)\n", 
                        clickedSquare, 
                        clickedSquare/8, 
                        clickedSquare%8);

                    uint64_t clicked_bb = BIT(clickedSquare);
                    uint64_t occupied = board->whitePeices | board->blackPeices;
                    
                    if (selectedSquare == -1) {
                        // ... (Logic for selecting a white piece remains the same)
                        if (board->whitePeices & clicked_bb) {
                            selectedSquare = clickedSquare;
                        }
                    } else {
                        // A piece is already selected, try to move it to the clicked square
                        uint64_t from_bb = BIT(selectedSquare);
                        uint64_t to_bb = BIT(clickedSquare);
                        uint64_t moves_bb = 0;
                        ChessPosition from_pos = {selectedSquare / 8, selectedSquare % 8};

                        // Generate moves based on the selected piece (only for White, since we are in the 'w' turn block)
                        if (board->wPawn & from_bb) {
                            moves_bb = whitePawnMovesBB(from_pos, board->empty, board->blackPeices, game->enPassantSquare);
                        } else if (board->wKnight & from_bb) {
                            moves_bb = knightMovesBB(from_pos);
                        } else if (board->wBishop & from_bb) {
                            moves_bb = bishopMovesBB(from_pos, occupied);
                        } else if (board->wRook & from_bb) {
                            moves_bb = rookMovesBB(from_pos, occupied);
                        } else if (board->wQueen & from_bb) {
                            moves_bb = queenMovesBB(from_pos, occupied);
                        } else if (board->wKing & from_bb) {
                            moves_bb = kingMovesBB(from_pos);
                            
                            // Manually add castling moves to the moves_bb if the king is selected
                            if (selectedSquare == 4) { // E1
                                // Kingside (E1 to G1)
                                if (game->wKingCastle && (BIT(7) & board->wRook) && (BIT(5) & board->empty) && (BIT(6) & board->empty)) {
                                    if (!isKingInCheck(board, 'w') && !isSquareAttacked(board, 5, 'b') && !isSquareAttacked(board, 6, 'b')) {
                                        moves_bb |= BIT(6);
                                    }
                                }
                                // Queenside (E1 to C1)
                                if (game->wQueenCastle && (BIT(0) & board->wRook) && (BIT(3) & board->empty) && (BIT(2) & board->empty) && (BIT(1) & board->empty)) {
                                    if (!isKingInCheck(board, 'w') && !isSquareAttacked(board, 3, 'b') && !isSquareAttacked(board, 2, 'b')) {
                                        moves_bb |= BIT(2);
                                    }
                                }
                            }
                        }
                        // Filter moves to prevent captures of friendly pieces
                        moves_bb &= ~board->whitePeices;


                        // Check if the clicked square is a valid move
                        if (moves_bb & to_bb) {
                             // Temporarily make the move on a copy of the board to check for check
                            chessBoard temp_board = *board;
                            gameState temp_game = *game;
                            move(&temp_board, &temp_game, selectedSquare, clickedSquare);

                            if (!isKingInCheck(&temp_board, game->toMove)) {
                                move(board, game, selectedSquare, clickedSquare);
                                
                                // After a move, check for game end conditions
                                if (!hasLegalMoves(board, game)) {
                                    if (isKingInCheck(board, game->toMove)) {
                                        printf("Checkmate! %s wins!\n", game->toMove == 'w' ? "Black" : "White");
                                        isGameOver = true;
                                    } else {
                                        printf("Stalemate! Game is a draw.\n");
                                        isGameOver = true;
                                    }
                                }
                            } else {
                                printf("Illegal move! King would be in check.\n");
                            }
                        } else {
                            printf("Illegal move!\n");
                        }
                        
                        // Deselect the piece or select the new piece if it belongs to the human (White)
                        if ((board->whitePeices & clicked_bb)) {
                             selectedSquare = clickedSquare;
                        } else {
                             selectedSquare = -1;
                        }
                    }
                }
            }
        }
        
        renderBoard(board, selectedSquare);
        renderStatus(game);
        SDL_Delay(16);
    }
    
    char* finalMessage = "Game Over!";
    if (isKingInCheck(board, game->toMove) && !hasLegalMoves(board, game)) {
        finalMessage = (game->toMove == 'w') ? "Checkmate! Black Wins!" : "Checkmate! White Wins!";
    } else if (!isKingInCheck(board, game->toMove) && !hasLegalMoves(board, game)) {
        finalMessage = "Stalemate! Draw.";
    }
    
    while(true) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                cleanupSDL();
                free(board);
                free(game);
                return 0;
            }
        }
        renderBoard(board, -1);
        renderGameOver(finalMessage);
        SDL_Delay(16);
    }

    cleanupSDL();
    free(board);
    free(game);
    return 0;
}
