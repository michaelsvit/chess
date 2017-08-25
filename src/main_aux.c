#include <stdlib.h>
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
			break;
		}
		case UNDO:
			return undo_move(*game);
		case SAVE:
			/* TODO: Implement */
			break;
		case RESET:
			destroy_game(*game);
			return RESTART;
		case GAME_QUIT:
			destroy_game(*game);
			return QUIT;
		case INVALID_GAME_COMMAND:
			return INVALID_COMMAND;
	}
	return SUCCESS;
}

EngineMessage execute_setting_command(GameSettings *settings, SettingCommand *cmd){
	int *args = (int *)cmd->arg;
	switch (cmd->type){
		case GAME_MODE:
			settings->mode = *args;
			break;
		case DIFFICULTY:
			/* TODO: If in 2 players mode then this is an error */
			settings->difficulty = *args;
			break;
		case USER_COLOR:
			/* TODO: If in 2 players mode then this is an error */
			settings->player1_color = *args;
		case LOAD:
			/* TODO: Implement */
			break;
		case DEFAULT:
			set_default_settings(settings);
			break;
		case PRINT_SETTING:
			/* TODO: Implement */
			break;
		case SETTING_QUIT:
			return QUIT;
		case START:
			return START_GAME;
		case INVALID_SETTING_COMMAND:
			return INVALID_COMMAND;
	}
	return SUCCESS;
}

void handle_message(EngineMessage msg, GameSettings **settings, State *state, int *quit){
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
			free(*settings);
			*state = GAME;
			return;
		case RESTART:
			*settings = create_settings();
			if(!*settings){
				*quit = 1;
				return;
			}
			*state = SETTINGS;
			return;
		case QUIT:
			/* Print quit message */
			if(*state == SETTINGS) free(*settings);
			*quit = 1;
			return;
	}
}

GameSettings *create_settings(){
	GameSettings *settings = (GameSettings *)malloc(sizeof(GameSettings));
	if(!settings) return NULL;
	set_default_settings(settings);
	return settings;
}

void set_default_settings(GameSettings *settings){
	settings->mode = DEFAULT_MODE;
	settings->difficulty = DEFAULT_DIFFICULTY;
	settings->player1_color = DEFAULT_PLAYER1_COLOR;
}
