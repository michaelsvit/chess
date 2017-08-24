#include "main_aux.h"

EngineMessage execute_game_command(Game **game, GameCommand *cmd){
	int *args = (int *)cmd->arg;
	switch (cmd->type) {
		case MOVE:
			return move_game_piece(*game, args[0], args[1], args[2], args[3]);
		case GET_MOVES:{
			/* SPArrayList *moves = get_possible_moves(*game, (*game)->board[args[1]][args[0]]); */
			/* TODO: Handle error */
			/* TODO: Print moves */
			return SUCCESS;
		}
		case UNDO:
			return undo_move(*game);
		case SAVE:
			/* TODO: Implement */
			return SUCCESS;
		case RESET:
			destroy_game(*game);
			*game = create_game(DEFAULT_MODE, DEFAULT_DIFFICULTY, DEFAULT_PLAYER1_COLOR);
			return (*game) ? RESTART : MALLOC_FAILURE;
		case GAME_QUIT:
			destroy_game(*game);
			return QUIT;
		case INVALID_GAME_COMMAND:
			return INVALID_COMMAND;
	}
}

void handle_message(EngineMessage msg, State *state, int *quit){
	switch(msg){
		case SUCCESS:
			return;
		case MALLOC_FAILURE:

			return;
		case INVALID_COMMAND:

			return;
		case INVALID_ARGUMENT:

			return;
		case ILLEGAL_MOVE:

			return;
		case EMPTY_HISTORY:

			return;
		case START_GAME:
			*state = GAME;
			return;
		case RESTART:
			*state = SETTINGS;
			return;
		case QUIT:
			/* Print quit message */
			*quit = 1;
			return;
	}
}
