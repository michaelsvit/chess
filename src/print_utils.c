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
	char repr = (piece->color == WHITE) ? 'a' : 'A';
	switch(piece->type){
		case PAWN:
			repr += 'p'-'a';
		case ROOK:
			repr += 'r'-'a';
		case KNIGHT:
			repr += 'n'-'a';
		case BISHOP:
			repr += 'b'-'a';
		case QUEEN:
			repr += 'q'-'a';
		case KING:
			repr += 'k'-'a';
	}
	return repr;
}
