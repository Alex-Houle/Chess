#include "board.h"
#include "types.h"
#include "rules.h"


Square parse_square(const char *input) {
    if (strlen(input) != 2) return NO_SQUARE;

    char file = toupper(input[0]);
    char rank = input[1];

    if (file < 'A' || file > 'H') return NO_SQUARE;
    if (rank < '1' || rank > '8') return NO_SQUARE;

    int file_idx = file - 'A';   // 0-7
    int rank_idx = rank - '1';   // 0-7

    return (Square)(rank_idx * 8 + file_idx);
}

int main() {
    chessBoard *board = malloc(sizeof(chessBoard));
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed for board\n");
        return 1;
    }

    gameState *game = malloc(sizeof(gameState));
    if (game == NULL) {
        fprintf(stderr, "Memory allocation failed for game state\n");
        free(board);
        return 1;
    }

    gameStart(board, game);
    printf("Starting position:\n");
    char from_input[3], to_input[3];
    while (!gameover(board)) {
        printBitBoard(board);

        printf("Piece to move (e.g. E2): ");
        scanf("%2s", from_input);
        printf("Move to (e.g. E4): ");
        scanf("%2s", to_input);

        Square from = parse_square(from_input);
        Square to = parse_square(to_input);

        if (from == NO_SQUARE || to == NO_SQUARE) {
            printf("Invalid square. Use format like E2 or H7.\n");
            continue;
        }
        generate_moves(board, game);
        move(board, game, from, to);
    }

    free(board);
    free(game);
    return 0;
}

