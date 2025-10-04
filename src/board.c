#include "board.h"
#include <stdlib.h> // For abs

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


/**
    * take the peice from Fenstring notion and make it print out the chess peices in ascii
    */
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
        * print the bitboard to be read in the console 
        */
void printBitBoard(chessBoard* board) {
    // Clear the board       
    // printf("\e[1;1H\e[2J");
    
    char fen[50];
    bitToFen(board,fen);
    
    

    int rank = 8;
    printf("\n   a b c d e f g h\n");
    printf("  +-----------------+\n");
    printf("%d | ", rank--);



    // prints out the actual board 
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

void gameStart(chessBoard* board, gameState* game) {
        
    board->wPawn = 0x000000000000FF00ULL;
    board->wBishop = 0x0000000000000024ULL;
    board->wRook = 0x0000000000000081ULL;
    board->wKnight = 0x0000000000000042ULL;
    board->wQueen = 0x0000000000000008ULL;
    board->wKing = 0x0000000000000010ULL;
    board->bPawn = 0x00FF000000000000ULL;
    board->bBishop = 0x2400000000000000ULL;
    board->bRook = 0x8100000000000000ULL;
    board->bKnight = 0x4200000000000000ULL;
    board->bQueen = 0x0800000000000000ULL;
    board->bKing = 0x1000000000000000ULL;
    

    game->bKingCastle = true;
    game->bQueenCastle =true;

    game->wKingCastle = true;
    game->wQueenCastle = true;

    game->toMove = 'w';
    game->enPassantSquare = -1;


    board->whitePeices = board->wPawn | board->wKnight | board->wBishop | board->wRook | board->wQueen | board->wKing;
    board->blackPeices = board->bPawn | board->bKnight | board->bBishop | board->bRook | board->bQueen | board->bKing; 
    board->empty = ~(board->whitePeices | board->blackPeices);
}

void move(chessBoard* board, gameState* game, int from, int to) {
    uint64_t from_mask = 1ULL << from;
    uint64_t to_mask = 1ULL << to;

    uint64_t* pieces[] = {
        &board->wPawn, &board->wKnight, &board->wBishop, &board->wRook, &board->wQueen, &board->wKing,
        &board->bPawn, &board->bKnight, &board->bBishop, &board->bRook, &board->bQueen, &board->bKing
    };

    // Determine the piece that is moving
    int movingPieceIndex = -1;
    for (int i = 0; i < 12; i++) {
        if (*pieces[i] & from_mask) {
            movingPieceIndex = i;
            break;
        }
    }

    // --- 1. Handle Castling and Castling Rights Update ---
    if (movingPieceIndex != -1) {
        // King move (White King index: 5, Black King: 11)
        if (movingPieceIndex == 5 || movingPieceIndex == 11) { 
            // Revoke castling rights upon King move
            if (movingPieceIndex == 5) {
                game->wKingCastle = false;
                game->wQueenCastle = false;
            } else {
                game->bKingCastle = false;
                game->bQueenCastle = false;
            }

            // Check for castling move (King moves 2 squares horizontally)
            if (abs(from % 8 - to % 8) == 2) {
                // Determine Rook's move (Kingside or Queenside)
                if (from % 8 < to % 8) { // Kingside
                    int rookFrom = (movingPieceIndex == 5) ? 7 : 63;
                    int rookTo = (movingPieceIndex == 5) ? 5 : 61;
                    uint64_t* rookBB = (movingPieceIndex == 5) ? &board->wRook : &board->bRook;
                    *rookBB &= ~BIT(rookFrom);
                    *rookBB |= BIT(rookTo);
                } else { // Queenside
                    int rookFrom = (movingPieceIndex == 5) ? 0 : 56;
                    int rookTo = (movingPieceIndex == 5) ? 3 : 59;
                    uint64_t* rookBB = (movingPieceIndex == 5) ? &board->wRook : &board->bRook;
                    *rookBB &= ~BIT(rookFrom);
                    *rookBB |= BIT(rookTo);
                }
            }
        }
        
        // Rook move/capture (White Rook index: 3, Black Rook: 9)
        else if (movingPieceIndex == 3 || movingPieceIndex == 9) {
            // Check if the rook is on its starting square and revoke castling rights
            if (from == 7) game->wKingCastle = false;   // h1
            if (from == 0) game->wQueenCastle = false;  // a1
            if (from == 63) game->bKingCastle = false;  // h8
            if (from == 56) game->bQueenCastle = false; // a8
        }
    }


    // --- 2. Handle En Passant Capture ---
    bool isEnPassant = false;
    // Check if it's a pawn moving to the enPassantSquare
    if ((movingPieceIndex == 0 || movingPieceIndex == 6) && (to == game->enPassantSquare)) {
        isEnPassant = true;
        
        // The captured pawn is on the square *behind* the en passant square.
        int capturedPawnSquare = (movingPieceIndex == 0) ? (to - 8) : (to + 8); 
        uint64_t capturedPawnMask = 1ULL << capturedPawnSquare;

        // Remove the captured pawn.
        if (movingPieceIndex == 0) { // White captured Black's pawn
            board->bPawn &= ~capturedPawnMask;
        } else { // Black captured White's pawn
            board->wPawn &= ~capturedPawnMask;
        }
    }
    
    // --- 3. Standard move/capture and En Passant square update ---
    
    // Remove any piece on the destination square (standard capture)
    if (!isEnPassant) {
        for (int i = 0; i < 12; i++) {
            if (*pieces[i] & to_mask) {
                // If a rook is captured, revoke castling rights on that side
                if (i == 3 && to == 7) game->wKingCastle = false;   // wRook h1
                if (i == 3 && to == 0) game->wQueenCastle = false;  // wRook a1
                if (i == 9 && to == 63) game->bKingCastle = false;  // bRook h8
                if (i == 9 && to == 56) game->bQueenCastle = false; // bRook a8
                
                *pieces[i] &= ~to_mask;
                break; // Only one piece per square
            }
        }
    }

    // Move the piece from 'from' to 'to'
    for (int i = 0; i < 12; i++) {
        if (*pieces[i] & from_mask) {
            *pieces[i] &= ~from_mask; 
            *pieces[i] |= to_mask;
            break; 
        }
    }
    
    // --- 4. Handle Pawn Promotion (Default to Queen) ---
    if (movingPieceIndex == 0 && (to >= 56 && to <= 63)) { // White pawn to rank 8
        board->wPawn &= ~to_mask; 
        board->wQueen |= to_mask; 
    } else if (movingPieceIndex == 6 && (to >= 0 && to <= 7)) { // Black pawn to rank 1
        board->bPawn &= ~to_mask; 
        board->bQueen |= to_mask; 
    }

    // --- 5. Update En Passant Target Square ---
    game->enPassantSquare = -1; // Reset default
    // Check for white double pawn push (rank 2 to 4)
    if (movingPieceIndex == 0 && (from >= 8 && from <= 15) && (to >= 24 && to <= 31)) {
        game->enPassantSquare = to - 8; 
    } 
    // Check for black double pawn push (rank 7 to 5)
    else if (movingPieceIndex == 6 && (from >= 48 && from <= 55) && (to >= 32 && to <= 39)) {
        game->enPassantSquare = to + 8; 
    }


    // Recalculate combined bitboards
    board->whitePeices = board->wPawn | board->wKnight | board->wBishop | board->wRook | board->wQueen | board->wKing;
    board->blackPeices = board->bPawn | board->bKnight | board->bBishop | board->bRook | board->bQueen | board->bKing;
    board->empty = ~(board->whitePeices | board->blackPeices);
 
    // Flip the turn
    game->toMove = (game->toMove == 'w') ? 'b' : 'w';
}


// This is just for debugging
void print_board(uint64_t bitboard) {
    for (int rank = 7; rank >=   0; rank--) {
        for (int file = 0; file < 8; file++) {
            int index = rank * 8 + file;
            int bit = (bitboard >> index) & 1;
            printf("%d ", bit);
        }
        printf("\n");
    }
}