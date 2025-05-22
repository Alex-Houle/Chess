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
    }
    
    gameStart(board,game);
    

    int from, to; 
    do { 
        printBitBoard(board);
        
        printf("Peice to move: ");
        scanf("%d", &from);

        printf("Move to: ");
        scanf("%d", &to);

        move(board, game, from, to);
        moves(board, game);

    } while(gameover(board) && from >= 0 && from <= 63 && to >= 0 && to <= 63);

    
    return 0;


}
