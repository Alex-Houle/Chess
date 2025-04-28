    #include "board.h"

    /**
     * Take a fen stirng and converts the fenString into bitboards
     */
    void fenToBit(chessBoard *board, char *fenString) {
        if (board == NULL || fenString == NULL) {
            printf("WHYYYYYYYYYYYYYYYY");
        }
    }

    /**
     * Get the piece at a specific index from the bitboard.
     * @param mask The bitboard mask.
     * @param index The index of the square (0-63).
     * @param piece The piece character.
     * @return The piece character if present, 0 otherwise.
     */
    char get_piece_at(uint64_t mask, int index, char piece) {
        return (mask >> index) & 1 ? piece : 0;
    }

    /**
     * Convert the bitboard to a FEN string representation.
     * @param board The chessboard.
     * @param output The output FEN string.
     */
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
        //printf("\e[1;1H\e[2J");
        char fen[50];
        bitToFen(board,fen);
        
        

        int rank = 8;
        printf("\n   a b c d e f g h\n");
        printf("  +-----------------+\n");
        printf("%d | ", rank--);



        // prints out the acutal board 
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



    /**
     * Check if the game is over (both kings are present).
     * @param board The chessboard.
     * @return 1 if the game is over, 0 otherwise.
     */
    int gameover(chessBoard *board) { 
        return 0;
    }

    /**
     * Initialize the chessboard with the starting position.
     * @param board The chessboard.
     * @param game The game state.
     */
    void gameStart(chessBoard* board, gameState* game) {
        board->wBishop = 0x0000000000000024ULL;
        board->wRook = 0x0000000000000081ULL;
        board->wKnight = 0x0000000000000042ULL;
        board->wQueen = 0x0000000000000008ULL;
        board->wKing = 0x0000000000000010ULL;
        board->wPawn = 0x000000000000FF00ULL;
        board->bBishop = 0x2400000000000000ULL;
        board->bRook = 0x8100000000000000ULL;
        board->bKnight = 0x4200000000000000ULL;
        board->bQueen = 0x0800000000000000ULL;
        board->bKing = 0x1000000000000000ULL;
        board->bPawn = 0x00FF000000000000ULL;

        game->bKingCastle = true;
        game->bQueenCastle =true;

        game->wKingCastle = true;
        game->wQueenCastle = true;

        game->toMove = 'w';


        game->EmptySquares = board->wPawn | board->wKnight | board->wBishop | board->wRook | board->wQueen | board->wKing |
                            board->bPawn | board->bKnight | board->bBishop | board->bRook | board->bQueen | board->bKing;   
    }


/**
 * Move a piece from one square to another on the chessboard.
 * @param board The chessboard.
 * @param game The game state.
 * @param from The starting square (0-63).
 * @param to The destination square (0-63).
 */
void move(chessBoard* board, gameState* game, int from, int to) {
    uint64_t from_mask = 1ULL << from;
    uint64_t to_mask = 1ULL << to;

    uint64_t* pieces[] = {
        &board->wPawn, &board->wKnight, &board->wBishop, &board->wRook, &board->wQueen, &board->wKing,
        &board->bPawn, &board->bKnight, &board->bBishop, &board->bRook, &board->bQueen, &board->bKing
    };

    for (int i = 0; i < 12; i++) {
        if (*pieces[i] & from_mask) {
            *pieces[i] &= ~from_mask; 
            *pieces[i] |= to_mask;    
        } else {
            *pieces[i] &= ~to_mask;   
        }
    }

    game->toMove = (game->toMove == 'w') ? 'b' : 'w';
}
