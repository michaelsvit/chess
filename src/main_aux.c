#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main_aux.h"

ProgramState *create_program_state(){
	ProgramState *state = (ProgramState *)malloc(sizeof(ProgramState));
	if(!state) return NULL;
	init_program_state(state);
	return state;
}

void destroy_program_state(ProgramState *state){
	(state->indicators->run_state == GAME) ?
		destroy_game(state->game) : free(state->settings);
	free(state->indicators);
	free(state->user_input);
	free(state);
}

int init_program_state(ProgramState *prog_state){
	prog_state->user_input = (char *)malloc(INPUT_SIZE);
	prog_state->settings = create_settings();
	prog_state->indicators = create_indicators();
	if(!prog_state->user_input || !prog_state->settings || !prog_state->indicators){
		free(prog_state->user_input);
		free(prog_state->settings);
		free(prog_state->indicators);
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
	indicators->run_state = SETTINGS;
	return indicators;
}

void get_user_input(const char* prompt, char* buf, int len) {
	if(prompt) printf("%s", prompt);
	fgets(buf, len, stdin);
	if (buf[strlen(buf) - 1] == '\n') {
		buf[strlen(buf) - 1] = '\0';
	}
}

int fetch_and_exe_game(ProgramState *state){
	if(state->indicators->print_game_prompt) print_board(state->game);
	print_player_color(state->game);
	get_user_input(GAME_PROMPT, state->user_input, INPUT_SIZE);
	GameCommand *cmd = parse_game_command(state->user_input);
	if(!cmd){
		destroy_game(state->game);
		return 0;
	}
	EngineMessage msg = execute_game_command(state->game, cmd);
	if(msg != SUCCESS){
		state->indicators->print_game_prompt = 0;
		handle_game_message(state, msg, cmd);
	} else {
		state->indicators->print_game_prompt = 1;
	}
	free(cmd->arg);
	free(cmd);
	return 1;
}

int fetch_and_exe_settings(ProgramState *state){
	char *prompt = NULL;
	if(state->indicators->print_settings_prompt){
		prompt = SETTINGS_PROMPT;
		state->indicators->print_settings_prompt = 0;
	}
	get_user_input(prompt, state->user_input, INPUT_SIZE);
	SettingCommand *cmd = parse_setting_command(state->user_input);
	if(!cmd){
		free(state->settings);
		return 0;
	}
	EngineMessage msg = execute_setting_command(state->settings, cmd);
	if(msg != SUCCESS)
		handle_settings_message(state, msg, cmd);
	free(cmd->arg);
	free(cmd);
	return 1;
}

EngineMessage execute_game_command(Game *game, GameCommand *cmd){
	int *args = (int *)cmd->arg;
	switch (cmd->type) {
		case MOVE:
			if(cmd->valid_arg){
				return move_game_piece(game, args[0], args[1], args[2], args[3]);
			} else {
				return INVALID_ARGUMENT;
			}
		case GET_MOVES:
			{
				if(!cmd->valid_arg)
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

void handle_game_message(ProgramState *state, EngineMessage msg, GameCommand *cmd){
	if(msg == INVALID_ARGUMENT || msg == ILLEGAL_MOVE || msg == EMPTY_HISTORY){
		print_game_invalid_arg(state->game, msg, cmd);
	} else {
		handle_message(state, msg);
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
			if(settings->mode == ONE_PLAYER){
				if(cmd->valid_arg && *args >= 1 && *args <= 4){
					settings->difficulty = *args;
					break;
				} else {
					return INVALID_ARGUMENT;
				}
			}
			return INVALID_COMMAND;
		case USER_COLOR:
			if(settings->mode == ONE_PLAYER){
				if(cmd->valid_arg && (*args == 1 || *args == 0)){
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

void handle_settings_message(ProgramState *state, EngineMessage msg, SettingCommand *cmd){
	if(msg == INVALID_ARGUMENT){
		print_settings_invalid_arg(cmd);
	} else {
		handle_message(state, msg);
	}
}

void handle_message(ProgramState *state, EngineMessage msg){
	switch(msg){
		case MALLOC_FAILURE:
			state->indicators->quit = 1;
			print_generic_message(msg);
			return;
		case INVALID_COMMAND:
			print_generic_message(msg);
			return;
		case START_GAME:
			state->indicators->run_state = GAME;
			state->game = create_game(state->settings);
			free(state->settings);
			if(!state->game){
				print_generic_message(MALLOC_FAILURE);
				state->indicators->quit = 1;
			}
			return;
		case RESTART:
			state->settings = create_settings();
			if(!state->settings){
				print_generic_message(MALLOC_FAILURE);
				state->indicators->quit = 1;
			}
			state->indicators->run_state = SETTINGS;
			state->indicators->print_settings_prompt = 1;
			print_generic_message(msg);
			return;
		case QUIT:
			state->indicators->quit = 1;
			return;
		default:
			break;
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
