#include "board.h"
#include "type.h"
#include "rules.h"

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

    int from, to;
    do {
        printBitBoard(board);

        printf("Piece to move: ");
        scanf("%d", &from);

        printf("Move to: ");
        scanf("%d", &to);

        uint64_t from_bb = BIT(from);
        uint64_t to_bb = BIT(to);
        uint64_t moves_bb = 0;

        // Determine whose turn it is and what piece is at `from`
        if (game->toMove == 'w') {
            if (board->wPawn & from_bb) {
                moves_bb = whitePawnMovesBB((ChessPosition){from / 8, from % 8},
                                            board->empty,
                                            board->blackPeices);
            } else if (board->wKnight & from_bb) {
                moves_bb = knightMovesBB((ChessPosition){from / 8, from % 8});
            } else if (board->wBishop & from_bb) {
                moves_bb = bishopMovesBB((ChessPosition){from / 8, from % 8}, board->blackPeices | board->whitePeices);
            } else if (board->wRook & from_bb) {
                moves_bb = rookMovesBB((ChessPosition){from / 8, from % 8}, board->blackPeices | board->whitePeices);
            } else if (board->wQueen & from_bb) {
                moves_bb = queenMovesBB((ChessPosition){from / 8, from % 8}, board->blackPeices | board->whitePeices);
            } else if (board->wKing & from_bb) {
                moves_bb = kingMovesBB((ChessPosition){from / 8, from % 8});
            }
        } else if (game->toMove == 'b') {
            if (board->bPawn & from_bb) {
                // you'd need to write blackPawnMovesBB() similarly to white
                // moves_bb = blackPawnMovesBB(...);
            } else if (board->bKnight & from_bb) {
                moves_bb = knightMovesBB((ChessPosition){from / 8, from % 8});
            } else if (board->bBishop & from_bb) {
                moves_bb = bishopMovesBB((ChessPosition){from / 8, from % 8}, board->blackPeices | board->whitePeices);
            } else if (board->bRook & from_bb) {
                moves_bb = rookMovesBB((ChessPosition){from / 8, from % 8}, board->blackPeices | board->whitePeices);
            } else if (board->bQueen & from_bb) {
                moves_bb = queenMovesBB((ChessPosition){from / 8, from % 8}, board->blackPeices | board->whitePeices);
            } else if (board->bKing & from_bb) {
                moves_bb = kingMovesBB((ChessPosition){from / 8, from % 8});
            }
        }

        if (moves_bb & to_bb) {
            move(board, game, from, to);
        } else {
            printf("Illegal move!\n");
        }

    } while (gameover(board) && from >= 0 && from <= 63 && to >= 0 && to <= 63);

    free(board);
    free(game);
    return 0;
}
