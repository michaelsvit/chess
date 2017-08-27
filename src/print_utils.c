#include <stdio.h>
#include "print_utils.h"

void print_board(Game *game){
	for (int i = BOARD_SIZE-1; i >= 0; i--) {
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
			break;
		case ROOK:
			repr += 'r'-'a';
			break;
		case KNIGHT:
			repr += 'n'-'a';
			break;
		case BISHOP:
			repr += 'b'-'a';
			break;
		case QUEEN:
			repr += 'q'-'a';
			break;
		case KING:
			repr += 'k'-'a';
			break;
	}
	return repr;
}

void print_settings(GameSettings *settings){
	printf("SETTINGS:\n");
	if(settings->mode == ONE_PLAYER){
		printf("GAME_MODE: 1\n");
		printf("DIFFICULTY_LVL: %d", settings->difficulty);
		printf("USER_CLR: %s", (settings->player1_color == WHITE) ? "WHITE" : "BLACK");
	} else {
		printf("GAME_MODE: 2\n");
	}
}
