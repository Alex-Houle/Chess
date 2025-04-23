/**
 * this is header file for the structs and enums used in the chess program
 * it contains the definition of the chessBoard struct, gameState struct, and the enum for piece types
 */

 // Struct representing the chess board using bitboards
 #ifndef TYPES_H
#define TYPES_H
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

#endif // TYPES_H
