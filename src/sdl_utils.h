#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"
#include "type.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define SQUARE_SIZE 80

typedef enum {
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
} PieceType;

// External declarations for global SDL objects
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* pieceTextures[12];
extern TTF_Font* font;

// Function declarations
void initializeSDL();
void loadPieceTextures();
void cleanupSDL();
void renderBoard(chessBoard* board, int selectedSquare);
void renderStatus(gameState* game);
void renderGameOver(const char* message);

#endif // SDL_UTILS_H