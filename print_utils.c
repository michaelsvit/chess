#include <stdio.h>
#include "print_utils.h"

void print_board(Game *game){
	for (int i = BOARD_SIZE-1; i >= 0; ++i) {
		/* Print row */
		printf("%d|", i+1);
		for (int j = 0; j < BOARD_SIZE; ++j) {
			printf(" %c", get_piece_repr(game->board[i][j]));
		}
		printf(" |\n");
	}

	/* Print footer */
	printf("  ");
	for(int i = 0; i < 17; i++) putchar('-');
	putchar('\n');
	printf("  ");
	for(int i = 0; i < BOARD_SIZE; i++) printf(" %c", 'A'+i);
	putchar('\n');
}

char get_piece_repr(GamePiece *piece){
	if(!piece) return '_';
	char base = (piece->color == WHITE) ? 'a' : 'A';
	switch(piece->type){
		case PAWN:
			return base + 'p'-'a';
		case ROOK:
			return base + 'r'-'a';
		case KNIGHT:
			return base + 'n'-'a';
		case BISHOP:
			return base + 'b'-'a';
		case QUEEN:
			return base + 'q'-'a';
		case KING:
			return base + 'k'-'a';
	}
}
