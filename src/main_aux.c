#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main_aux.h"

int init_game_variables(char **user_input, GameSettings **settings, Indicators **indicators){
	*user_input = (char *)malloc(INPUT_SIZE);
	*settings = create_settings();
	*indicators = create_indicators();
	if(!*user_input || !*settings || !*indicators){
		free(user_input);
		free(settings);
		free(indicators);
		return 0;
	}
	return 1;
}

Indicators *create_indicators(){
	Indicators *indicators = (Indicators *)malloc(sizeof(Indicators));
	if(!indicators) return NULL;
	indicators->quit = 0;
	indicators->print_game_prompt = 1;
	indicators->print_settings_prompt = 1;
	indicators->state = SETTINGS;
	return indicators;
}

void get_user_input(const char* prompt, char* buf, int len) {
	if(prompt) printf("%s", prompt);
	fgets(buf, len, stdin);
	if (buf[strlen(buf) - 1] == '\n') {
		buf[strlen(buf) - 1] = '\0';
	}
}

EngineMessage execute_game_command(Game *game, GameCommand *cmd){
	int *args = (int *)cmd->arg;
	switch (cmd->type) {
		case MOVE:
			return move_game_piece(game, args[0], args[1], args[2], args[3]);
		case GET_MOVES:
			{
				if(args[0] < 0 || args[0]>= BOARD_SIZE || args[1] < 0 || args[1] >= BOARD_SIZE)
					return INVALID_ARGUMENT;
				SPArrayList *moves;
				EngineMessage msg = get_possible_moves(
						&moves, game,
						game->board[args[1]][args[0]]);
				if(msg == SUCCESS) print_possible_moves(game, moves);
				spArrayListDestroy(moves);
				return msg;
			}
		case UNDO:
			{
				GameMove *moves[2];
				EngineMessage msg = undo_move(game, &moves[0]);
				if(msg != SUCCESS) return msg;
				msg = undo_move(game, &moves[1]);
				if(msg != SUCCESS){
					free(moves[0]);
					return msg;
				}
				print_undo_two_moves(game, moves);
				return SUCCESS;
			}
		case SAVE:
			/* TODO: Implement */
			break;
		case RESET:
			return RESTART;
		case GAME_QUIT:
			return QUIT;
		case INVALID_GAME_COMMAND:
			return INVALID_COMMAND;
	}
	return SUCCESS;
}

void handle_game_message(
		Game **game,
		EngineMessage msg,
		GameCommand *cmd,
		GameSettings **settings,
		Indicators *indicators){
	if(msg == INVALID_ARGUMENT || msg == ILLEGAL_MOVE || msg == EMPTY_HISTORY){
		print_game_invalid_arg(*game, msg, cmd);
	} else {
		print_generic_message(msg);
		handle_message(msg, game, settings, indicators);
	}
}

EngineMessage execute_setting_command(GameSettings *settings, SettingCommand *cmd){
	int *args = (int *)cmd->arg;
	switch (cmd->type){
		case GAME_MODE:
			if(cmd->valid_arg && (*args == ONE_PLAYER || *args == TWO_PLAYER)){
				settings->mode = *args;
				print_game_mode(*args);
				break;
			} else {
				return INVALID_ARGUMENT;
			}
		case DIFFICULTY:
			if(cmd->valid_arg && settings->mode == ONE_PLAYER){
				if(*args >= 1 && *args <= 4){
					settings->difficulty = *args;
					break;
				} else {
					return INVALID_ARGUMENT;
				}
			}
			return INVALID_COMMAND;
		case USER_COLOR:
			if(cmd->valid_arg && settings->mode == ONE_PLAYER){
				if(*args == 1 || *args == 0){
					settings->player1_color = *args;
					break;
				} else {
					return INVALID_ARGUMENT;
				}
			}
			return INVALID_COMMAND;
		case LOAD:
			/* TODO: Implement */
			break;
		case DEFAULT:
			set_default_settings(settings);
			break;
		case PRINT_SETTING:
			print_settings(settings);
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

void handle_settings_message(
		Game **game,
		EngineMessage msg,
		SettingCommand *cmd,
		GameSettings **settings,
		Indicators *indicators){
	if(msg == INVALID_ARGUMENT){
		print_settings_invalid_arg(cmd);
	} else {
		print_generic_message(msg);
		handle_message(msg, game, settings, indicators);
	}
}

void handle_message(EngineMessage msg, Game **game, GameSettings **settings, Indicators *indicators){
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
			indicators->state = GAME;
			*game = create_game(*settings);
			free(*settings);
			if(!*game){
				print_error(MEMORY);
				indicators->quit = 1;
			}
			return;
		case RESTART:
			*settings = create_settings();
			if(!*settings){
				print_error(MEMORY);
				indicators->quit = 1;
			}
			indicators->state = SETTINGS;
			indicators->print_settings_prompt = 1;
			return;
		case QUIT:
			/* Print quit message */
			(indicators->state == GAME) ? destroy_game(*game) : free(*settings);
			indicators->quit = 1;
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
