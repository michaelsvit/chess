#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "print_utils.h"

void print_player_color(Game *game){
	printf("%s ", (game->player_color[game->current_player] == WHITE) ? "white" : "black");
}

void print_game_error(Game *game, EngineMessage msg, GameCommand *cmd){
	switch(cmd->type){
		case MOVE:
			if(msg == INVALID_ARGUMENT){
				printf("Invalid position on the board\n");
			} else if (msg == ILLEGAL_MOVE) {
				char *str;
				int *arg = (int *)cmd->arg;
				/* Get game piece at source position */
				GamePiece *piece = game->board[arg[1]][arg[0]];
				if(!piece || piece->color != game->player_color[game->current_player]){
					str = "The specified position does not contain your piece\n";
				} else {
					str = "Illegal move\n";
				}
				printf("%s", str);
			}
			break;
		case GET_MOVES:
			if(msg == ILLEGAL_MOVE){
				char *str = (game->player_color[game->current_player] == WHITE) ?
					"white" : "black";
				printf("The specified position does not contain %s player piece\n", str);
			} else if (msg == INVALID_ARGUMENT) {
				printf("Invalid position on the board\n");
			}
			break;
		case SAVE:
			break;
		case UNDO:
			if(msg == INVALID_COMMAND){
				printf("Undo command not available in 2 players mode\n");
			} else if (msg == EMPTY_HISTORY) {
				printf("Empty history, move cannot be undone\n");
			}
			break;
		default:
			break;
	}
}

void print_settings_error(SettingCommand *cmd){
	switch(cmd->type){
		case GAME_MODE:
			printf("Wrong game mode\n");
			break;
		case DIFFICULTY:
			if(cmd->valid_arg && *((int *)cmd->arg) == 5){
				printf("Expert level not supported, please choose a value between 1 to 4:\n");
			} else {
				printf("Wrong difficulty level. The value should be between 1 to 5\n");
			}
			break;
		case LOAD:
			printf("Error: File doesn’t exist or cannot be opened\n");
			break;
		default:
			break;
	}
}

void print_generic_message(EngineMessage msg){
	switch(msg){
		case MALLOC_FAILURE:
			printf("ERROR: Memory allocation failure\n");
			break;
		case INVALID_COMMAND:
			printf("ERROR: Invalid command\n");
			break;
		case RESTART:
			printf("Restarting...\n");
			break;
		case QUIT:
			printf("Exiting...\n");
		default:
			break;
	}
}

void print_undo_two_moves(Game *game, GameMove *moves[]){
	GameMove *move = moves[0];
	Color color = !game->player_color[game->current_player];
	printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
			(color == WHITE) ? "white" : "black",
			move->src_y+1, move->src_x+'A',
			move->dst_y+1, move->dst_x+'A');
	move = moves[1];
	color = !color;
	printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",
			(color == WHITE) ? "white" : "black",
			move->src_y+1, move->src_x+'A',
			move->dst_y+1, move->dst_x+'A');
	free(moves[0]);
	free(moves[1]);
}

void print_check(Color color){
	printf("Check: %s King is threatened!\n", (color == WHITE) ? "white" : "black");
}

void print_game_over(Game *game){
	if(game->check){
		char *str = (game->player_color[game->current_player] == WHITE) ? "black" : "white";
		printf("Checkmate! %s player wins the game\n", str);
	} else {
		if (game->mode == ONE_PLAYER) {
			printf("The game ends in a tie\n");
		} else {
			printf("The game is tied\n");
		}
	}
}

void print_game_mode(int mode){
	printf("Game mode is set to %s\n", (mode == ONE_PLAYER) ? "1 player" : "2 players");
}

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
			repr += 'm'-'a';
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
		printf("DIFFICULTY_LVL: %d\n", settings->difficulty);
		printf("USER_CLR: %s\n", (settings->player1_color == WHITE) ? "WHITE" : "BLACK");
	} else {
		printf("GAME_MODE: 2\n");
	}
}

int print_possible_moves(Game *game, SPArrayList *moves){
	/* Create new list and use insertion sort */
	int count = spArrayListSize(moves);
	SPArrayList *list = spArrayListCreate(sizeof(GameMove), count);
	if(!list) return 0;
	/* Use insertion sort */
	spArrayListAddFirst(list, spArrayListGetFirst(moves));
	for (int i = 1; i < count; ++i) {
		GameMove *move = spArrayListGetAt(moves, i);
		int j = 0;
		GameMove *temp = (GameMove *)spArrayListGetAt(list, j);
		while(j < count-1 && temp
				&& compare_positions(move->dst_x, move->dst_y, temp->dst_x, temp->dst_y)>0){
			j++;
			temp = (GameMove *)spArrayListGetAt(list, j);
		}
		spArrayListAddAt(list, move, j);
	}
	spArrayListClear(moves);

	/* Create list of string representations of move destinations */
	SPArrayList *strings = spArrayListCreate(POS_REPR_MAX_LENGTH + 1, count);
	if(!strings){
		spArrayListDestroy(list);
		return 0;
	}
	for (int i = 0; i < count; ++i) {
		char *repr = get_destination_repr(game, spArrayListGetAt(list, i));
		if(!repr){
			spArrayListDestroy(list);
			spArrayListDestroy(strings);
			return 0;
		}
		spArrayListAddAt(strings, repr, i);
	}
	/* Print generated representations */
	for (int i = 0; i < count; ++i) {
		printf("%s\n", (char *)spArrayListGetAt(strings, i));
	}
	spArrayListDestroy(list);
	spArrayListDestroy(strings);
	return 1;
}

char *get_destination_repr(Game *game, GameMove *move){
	GamePiece *dst_piece = game->board[move->dst_y][move->dst_x];
	int capture = dst_piece && (dst_piece->color == game->player_color[!game->current_player]);
	GamePiece *src_piece = game->board[move->src_y][move->src_x];
	int dst_threat = is_piece_threatened_after_move(game, src_piece, move);
	int row = move->dst_y+1;
	char col = move->dst_x+'A';
	char *str = (char *)malloc(POS_REPR_MAX_LENGTH+1);
	sprintf(str, "<%d,%c>", row, col);
	if(dst_threat){
		int len = strlen(str);
		str[len] = '*';
		str[len+1] = '\0';
	}
	if(capture){
		int len = strlen(str);
		str[len] = '^';
		str[len+1] = '\0';
	}
	return str;
}

int compare_positions(int a_x, int a_y, int b_x, int b_y){
	if(a_y > b_y) return 1;
	if(a_y == b_y){
		if(a_x > b_x) return 1;
		if(a_x == b_x){
			return 0;
		} else {
			return -1;
		}
	} else {
		return -1;
	}
}
