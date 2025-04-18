#include "board.h"

/**
 * Take a fen stirng and converts the fenString into bitboards
 */
void fenToBit(chessBoard *board, char *fenString) {
     if (board == NULL || fenString == NULL) {
        printf("WHYYYYYYYYYYYYYYYY");
     }
}

char get_piece_at(uint64_t mask, int index, char piece) {
    return (mask >> index) & 1 ? piece : 0;
}

void bitToFen(chessBoard* board, char* output) {
    char fen[100] = {0};
    int empty = 0;

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int index = rank * 8 + file;
            char piece = 0;
            if ((piece = get_piece_at(board->wPawn, index, 'P')) ||
                (piece = get_piece_at(board->wKnight, index, 'N')) ||
                (piece = get_piece_at(board->wBishop, index, 'B')) ||
                (piece = get_piece_at(board->wRook, index, 'R')) ||
                (piece = get_piece_at(board->wQueen, index, 'Q')) ||
                (piece = get_piece_at(board->wKing, index, 'K')) ||
                (piece = get_piece_at(board->bPawn, index, 'p')) ||
                (piece = get_piece_at(board->bKnight, index, 'n')) ||
                (piece = get_piece_at(board->bBishop, index, 'b')) ||
                (piece = get_piece_at(board->bRook, index, 'r')) ||
                (piece = get_piece_at(board->bQueen, index, 'q')) ||
                (piece = get_piece_at(board->bKing, index, 'k'))) {
                if (empty > 0) {
                    char num[2] = {(char)(empty + '0'), '\0'};
                    strcat(fen, num);
                    empty = 0;
                }
                strncat(fen, &piece, 1);
            } else {
                empty++;
            }
        }
        if (empty > 0) {
            char num[2] = {(char)(empty + '0'), '\0'};
            strcat(fen, num);
            empty = 0;
        }
        if (rank > 0) strcat(fen, "/");
    }

    strcpy(output, fen);
}


const char* get_unicode_piece(char piece) {
    switch (piece) {
        // White pieces
        case 'k': return "♔";
        case 'q': return "♕";
        case 'r': return "♖";
        case 'b': return "♗";
        case 'n': return "♘";
        case 'p': return "♙";
        // Black pieces
        case 'K': return "♚";
        case 'Q': return "♛";
        case 'R': return "♜";
        case 'B': return "♝";
        case 'N': return "♞";
        case 'P': return "♟";
        default: return " ";
        }
    }

    /**
     * print the bitbaord to be read in the consele 
     */
void printBitBoard(chessBoard* board) {
    char fen[50];
    bitToFen(board,fen);

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

int gameover(chessBoard *board) { 
    return 1;
}