// src/main.c
#include "board.h"
#include "type.h"
#include "rules.h"
#include "sdl_utils.h"
#include <stdbool.h>

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
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                isGameOver = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int x = e.button.x / SQUARE_SIZE;
                    int y = 7 - (e.button.y / SQUARE_SIZE);
                    int clickedSquare = y * 8 + x;
                    
                    printf("Clicked square: %d (row: %d, col: %d)\n", 
                        clickedSquare, 
                        clickedSquare/8, 
                        clickedSquare%8);

                    uint64_t clicked_bb = BIT(clickedSquare);
                    uint64_t occupied = board->whitePeices | board->blackPeices;
                    
                    if (selectedSquare == -1) {
                        // No piece selected, check if the clicked square has a piece of the current player's color
                        if ((game->toMove == 'w' && (board->whitePeices & clicked_bb)) ||
                            (game->toMove == 'b' && (board->blackPeices & clicked_bb))) {
                            selectedSquare = clickedSquare;
                        }
                    } else {
                        // A piece is already selected, try to move it to the clicked square
                        uint64_t from_bb = BIT(selectedSquare);
                        uint64_t to_bb = BIT(clickedSquare);
                        uint64_t moves_bb = 0;
                        ChessPosition from_pos = {selectedSquare / 8, selectedSquare % 8};

                        // Generate moves based on the selected piece
                        if (game->toMove == 'w') {
                            if (board->wPawn & from_bb) {
                                moves_bb = whitePawnMovesBB(from_pos, board->empty, board->blackPeices);
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
                            }
                            // Filter moves to prevent captures of friendly pieces
                            moves_bb &= ~board->whitePeices;
                        } else { // Black's turn
                            if (board->bPawn & from_bb) {
                                moves_bb = blackPawnMovesBB(from_pos, board->empty, board->whitePeices);
                            } else if (board->bKnight & from_bb) {
                                moves_bb = knightMovesBB(from_pos);
                            } else if (board->bBishop & from_bb) {
                                moves_bb = bishopMovesBB(from_pos, occupied);
                            } else if (board->bRook & from_bb) {
                                moves_bb = rookMovesBB(from_pos, occupied);
                            } else if (board->bQueen & from_bb) {
                                moves_bb = queenMovesBB(from_pos, occupied);
                            } else if (board->bKing & from_bb) {
                                moves_bb = kingMovesBB(from_pos);
                            }
                            // Filter moves to prevent captures of friendly pieces
                            moves_bb &= ~board->blackPeices;
                        }

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
                        
                        // Deselect the piece
                        selectedSquare = -1;
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
        finalMessage = "Checkmate!";
    } else if (!isKingInCheck(board, game->toMove) && !hasLegalMoves(board, game)) {
        finalMessage = "Stalemate!";
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