#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "board.h"


/**
 * Take a fen stirng and converts the fenString into bitboards
 */
void fenToBit(chessBoard *board, char *fenString) {
    // Initialize the board to zero
    board->wBishop = 0;
    board->wRook = 0;
    board->wKnight = 0;
    board->wQueen = 0;
    board->wKing = 0;
    board->wPawn = 0;
    board->bBishop = 0;
    board->bRook = 0;
    board->bKnight = 0;
    board->bQueen = 0;
    board->bKing = 0;
    board->bPawn = 0;

    char peice = ' ';
    short place  = 0; 
    for (int i = 0; i < 64; i++, place++) {
        peice = fenString[i];
        if (peice == '/') {
            place--; // Adjust place for row separator
            continue; 
        } else if (peice >= '0' && peice <= '9') {
            place += peice - '0' - 1; // Skip empty squares
            continue;
        } else if (peice == '\0') {
            break; // End of the string
        }
        
        switch (peice) {
            case 'B':
                board->wBishop |= (1ULL << (63 - place));
                place++;
                break;
            case 'R':
                board->wRook |= (1ULL << (63 - place));
                place++;
                break;
            case 'N':
                board->wKnight |= (1ULL << (63 - place));
                place++;
                break;
            case 'Q':
                board->wQueen |= (1ULL << (63 - place));
                place++;
                break;
            case 'K':
                board->wKing |= (1ULL << (63 - place));
                place++;
                break;
            case 'P':
                board->wPawn |= (1ULL << (63 - place));
                place++;
                break;
            case 'b':
                board->bBishop |= (1ULL << (63 - place));
                place++;
                break;
            case 'r':
                board->bRook |= (1ULL << (63 - place));
                place++;
                break;
            case 'n':
                board->bKnight |= (1ULL << (63 - place));
                place++;
                break;
            case 'q':
                board->bQueen |= (1ULL << (63 - place));
                place++;
                break;
            case 'k':
                board->bKing |= (1ULL << (63 - place));
                place++;
                break;
            case 'p':
                board->bPawn |= (1ULL << (63 - place));
                place++;
                break;
            default:
                continue; // Ignore empty squares or invalid characters
        }
    }

}

/**
 * Take a bitboard and converts it into a fen string
 * The fen string is stored in the fenString variable
 */
void bitToFen(chessBoard board, char *fenString) {
    board.bBishop ++;
    fenString[1] = 'a';
}

/**
 * print the bitbaord to be read in the consele 
 */
void printBitBoard(chessBoard board) {
    printf("White Pieces:\n");
    printf("Bishop: %016" PRIx64 "\n", board.wBishop);
    printf("Rook: %016" PRIx64 "\n", board.wRook);
    printf("Queen: %016" PRIx64 "\n", board.wQueen);
    printf("King: %016" PRIx64 "\n", board.wKing);
    printf("Knight: %016" PRIx64 "\n", board.wKnight);
    printf("Pawn: %016" PRIx64 "\n", board.wPawn);

    printf("Black Pieces:\n");
    printf("Bishop: %016" PRIx64 "\n", board.bBishop);
    printf("Rook: %016" PRIx64 "\n", board.bRook);
    printf("Queen: %016" PRIx64 "\n", board.bQueen);
    printf("King: %016" PRIx64 "\n", board.bKing);
    printf("Knight: %016" PRIx64 "\n", board.bKnight);
    printf("Pawn: %016" PRIx64 "\n", board.bPawn);
   
    

   
    printf("\n"); 
}