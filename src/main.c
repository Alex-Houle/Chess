// src/main.c
#include "board.h"
#include "type.h"
#include "rules.h"
#include "sdl_utils.h"

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
    int running = 1;
    
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int x = e.button.x / SQUARE_SIZE;
                    int y = 7 - (e.button.y / SQUARE_SIZE);
                    int clickedSquare = y * 8 + x;
                    // Add this debug print in your mouse click handler:
                    printf("Clicked square: %d (row: %d, col: %d)\n", 
                        clickedSquare, 
                        clickedSquare/8, 
                        clickedSquare%8);
                    if (selectedSquare == -1) {
                        selectedSquare = clickedSquare;
                    } else {
                        uint64_t from_bb = BIT(selectedSquare);
                        uint64_t to_bb = BIT(clickedSquare);
                        uint64_t moves_bb = 0;

                        if (game->toMove == 'w') {
                            if (board->wPawn & from_bb) {
                                moves_bb = whitePawnMovesBB((ChessPosition){selectedSquare / 8, selectedSquare % 8},
                                                            board->empty,
                                                            board->blackPeices);
                            }
                            // ... rest of your move generation logic
                        }

                        if (moves_bb & to_bb) {
                            move(board, game, selectedSquare, clickedSquare);
                        } else {
                            printf("Illegal move!\n");
                        }
                        
                        selectedSquare = -1;
                    }
                }
            }
        }
        
        renderBoard(board, selectedSquare);
        renderStatus(game);
        SDL_Delay(16);
    }
    
    cleanupSDL();
    free(board);
    free(game);
    return 0;
}