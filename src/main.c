#include "board.h"
#include "types.h"



int main() {
    
    chessBoard *board = malloc(sizeof(chessBoard));
    if (board == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
}
    gameState *game = malloc(sizeof(gameState));
    if (game == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
    }
    
    gameStart(board,game);
    printBitBoard(board);
    
    int from, to; 
    do { 
        printBitBoard(board);
        
        printf("Peice to move: ");
        scanf("%d", &from);
        if (from < 0 || from > 63) {
            break;
        }

        printf("Move to: ");
        scanf("%d", &to);

        move(board, game, from, to);

    } while(gameover(board) && from >= 0 && from <= 63 && to >= 0 && to <= 63);

    
    return 0;


}