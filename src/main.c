#include <stdlib.h>
#include <stdio.h>
#include "main_aux.h"

int main(/* int argc, char *argv[] */){
	char *user_input = (char *)malloc(INPUT_SIZE);
	GameSettings *settings = create_settings();
	if(!settings || !user_input){
		free(settings);
		free(user_input);
		print_error(MEMORY);
		return 0;
	}
	int quit = 0;
	int settings_prompt_printed = 0;
	State state = SETTINGS;
	Game *game;
	do {
		EngineMessage msg;
		if(state == GAME){
			print_board(game);
			print_player_color(game);
			get_user_input(GAME_PROMPT, user_input, INPUT_SIZE);
			GameCommand *cmd = parse_game_command(user_input);
			if(!cmd){
				print_error(MEMORY);
				break;
			}
			msg = execute_game_command(game, cmd);
			if(msg == INVALID_ARGUMENT || msg == ILLEGAL_MOVE)
				print_game_invalid_arg(game, msg, cmd);
			free(cmd->arg);
			free(cmd);
		} else {
			char *prompt = settings_prompt_printed ? SETTINGS_PROMPT : NULL;
			get_user_input(prompt, user_input, INPUT_SIZE);
			SettingCommand *cmd = parse_setting_command(user_input);
			if(!cmd){
				free(settings);
				print_error(MEMORY);
				break;
			}
			msg = execute_setting_command(settings, cmd);
			if(msg == INVALID_ARGUMENT) print_settings_invalid_arg(cmd);
			free(cmd->arg);
			free(cmd);
		}
		if(msg != SUCCESS && msg != INVALID_ARGUMENT)
			handle_message(msg, &game, &settings, &state, &quit);
	} while (!quit);

	free(user_input);
	return 0;
}
