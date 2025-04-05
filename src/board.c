#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
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


const char* get_unicode_piece(char piece) {
    switch (piece) {
        // White pieces
        case 'K': return "♔";
        case 'Q': return "♕";
        case 'R': return "♖";
        case 'B': return "♗";
        case 'N': return "♘";
        case 'P': return "♙";
        // Black pieces
        case 'k': return "♚";
        case 'q': return "♛";
        case 'r': return "♜";
        case 'b': return "♝";
        case 'n': return "♞";
        case 'p': return "♟";
        default: return " ";
        }
    }

    /**
     * print the bitbaord to be read in the consele 
     */
void printBitBoard(const char *fen) {
    int rank = 8;
    printf("\n   a b c d e f g h\n");
    printf("  +-----------------+\n");
    printf("%d | ", rank--);

    for (const char *c = fen; *c; ++c) {
        if (*c == '/') {
            printf("|\n%d | ", rank--);
        } else if (isdigit(*c)) {
            int empty = *c - '0';
            for (int i = 0; i < empty; i++) {
                printf("· ");
            }
        } else {
            printf("%s ", get_unicode_piece(*c));
        }
    }
    printf("|\n  +-----------------+\n\n");
}