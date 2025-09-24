#include "sdl_utils.h"
#include "board.h"
#include "type.h"
#include <stdio.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* pieceTextures[12];
TTF_Font* font = NULL;

void initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    font = TTF_OpenFont("asset/font.ttf", 48); // User must provide a font file
    if (font == NULL) {
        fprintf(stderr, "Failed to load font! TTF_Error: %s\n", TTF_GetError());
    }
    
    window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                             WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    printf("SDL Initialized successfully. Window and Renderer created.\n");
}

void loadPieceTextures() {
    const char* textureFiles[] = {
        "asset/Chess_plt60.png",   // WHITE_PAWN
        "asset/Chess_nlt60.png",   // WHITE_KNIGHT
        "asset/Chess_blt60.png",   // WHITE_BISHOP
        "asset/Chess_rlt60.png",   // WHITE_ROOK
        "asset/Chess_qlt60.png",   // WHITE_QUEEN
        "asset/Chess_klt60.png",   // WHITE_KING
        "asset/Chess_pdt60.png",   // BLACK_PAWN
        "asset/Chess_ndt60.png",   // BLACK_KNIGHT
        "asset/Chess_bdt60.png",   // BLACK_BISHOP
        "asset/Chess_rdt60.png",   // BLACK_ROOK
        "asset/Chess_qdt60.png",   // BLACK_QUEEN
        "asset/Chess_kdt60.png"    // BLACK_KING
    };
    
    for (int i = 0; i < 12; i++) {
        SDL_Surface* surface = IMG_Load(textureFiles[i]);
        if (surface == NULL) {
            fprintf(stderr, "Unable to load image %s! SDL Error: %s\n", textureFiles[i], IMG_GetError());
            continue;
        }
        
        pieceTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        if (pieceTextures[i] == NULL) {
            fprintf(stderr, "Unable to create texture from %s! SDL Error: %s\n", textureFiles[i], SDL_GetError());
        } else {
            SDL_SetTextureBlendMode(pieceTextures[i], SDL_BLENDMODE_BLEND);
        }
    }
}

void cleanupSDL() {
    for (int i = 0; i < 12; i++) {
        if (pieceTextures[i] != NULL) {
            SDL_DestroyTexture(pieceTextures[i]);
        }
    }
    
    if (font != NULL) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void renderBoard(chessBoard* board, int selectedSquare) {
    SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xFF);
    SDL_RenderClear(renderer);
    
        for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int square = row * 8 + col;
            SDL_Rect rect = {col * SQUARE_SIZE, (7 - row) * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            
            if (square == selectedSquare) {
                SDL_SetRenderDrawColor(renderer, 0x88, 0x88, 0xFF, 0x80);
                SDL_RenderFillRect(renderer, &rect);
            }
            
            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 0xF0, 0xD9, 0xB5, 0xFF);
            } else {
                SDL_SetRenderDrawColor(renderer, 0xB5, 0x88, 0x63, 0xFF);
            }
            SDL_RenderFillRect(renderer, &rect);
            
            uint64_t mask = 1ULL << square;
            int pieceType = -1;
            
            if (board->wPawn & mask) pieceType = WHITE_PAWN;
            else if (board->wKnight & mask) pieceType = WHITE_KNIGHT;
            else if (board->wBishop & mask) pieceType = WHITE_BISHOP;
            else if (board->wRook & mask) pieceType = WHITE_ROOK;
            else if (board->wQueen & mask) pieceType = WHITE_QUEEN;
            else if (board->wKing & mask) pieceType = WHITE_KING;

            else if (board->bPawn & mask) pieceType = BLACK_PAWN;
            else if (board->bKnight & mask) pieceType = BLACK_KNIGHT;
            else if (board->bBishop & mask) pieceType = BLACK_BISHOP;
            else if (board->bRook & mask) pieceType = BLACK_ROOK;
            else if (board->bQueen & mask) pieceType = BLACK_QUEEN;
            else if (board->bKing & mask) pieceType = BLACK_KING;
            
            if (pieceType != -1 && pieceTextures[pieceType] != NULL) {
                SDL_RenderCopy(renderer, pieceTextures[pieceType], NULL, &rect);
            }
        }
    }
    
    SDL_RenderPresent(renderer);
}

void renderStatus(gameState* game) {
    //printf("%s to move\n", game->toMove == 'w' ? "White" : "Black");
}

void renderGameOver(const char* message) {
    if (font == NULL) return;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, message, white);
    if (surface == NULL) {
        fprintf(stderr, "Failed to render text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        fprintf(stderr, "Failed to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect textRect;
    textRect.w = surface->w;
    textRect.h = surface->h;
    textRect.x = (WINDOW_WIDTH - textRect.w) / 2;
    textRect.y = (WINDOW_HEIGHT - textRect.h) / 2;

    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}