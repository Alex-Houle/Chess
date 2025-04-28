/**
 * this is header file for the structs and enums used in the chess program
 * it contains the definition of the chessBoard struct, gameState struct, and the enum for piece types
 */

#ifndef TYPES_H
#define TYPES_H

#define FILE_A 0x0101010101010101ULL
#define FILE_B 0x0202020202020202ULL
#define FILE_C 0x0404040404040404ULL
#define FILE_D 0x0808080808080808ULL
#define FILE_E 0x1010101010101010ULL
#define FILE_F 0x2020202020202020ULL
#define FILE_G 0x4040404040404040ULL
#define FILE_H 0x8080808080808080ULL

#define RANK_1 0x00000000000000FFULL
#define RANK_2 0x000000000000FF00ULL
#define RANK_3 0x0000000000FF0000ULL
#define RANK_4 0x00000000FF000000ULL
#define RANK_5 0x000000FF00000000ULL
#define RANK_6 0x0000FF0000000000ULL
#define RANK_7 0x00FF000000000000ULL
#define RANK_8 0xFF00000000000000ULL

typedef struct {
    uint64_t wPawn;
    uint64_t wKnight;
    uint64_t wBishop;
    uint64_t wRook;
    uint64_t wQueen;
    uint64_t wKing;
    
    uint64_t bPawn;
    uint64_t bKnight;
    uint64_t bBishop;
    uint64_t bRook;
    uint64_t bQueen;
    uint64_t bKing;
} chessBoard;

typedef struct {
    bool wKingCastle;
    bool wQueenCastle;

    bool bKingCastle;
    bool bQueenCastle;

    char toMove;

    uint64_t EmptySquares;
} gameState;

typedef enum {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NO_SQUARE
} Square;               


typedef struct{ 
    char piece;
    int from;
    int to;
} move_t;

#endif // TYPES_H
