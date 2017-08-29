#include <stdlib.h>
#include <stdio.h>
#include "main_aux.h"

int main(/* int argc, char *argv[] */){
	char *user_input;
	GameSettings *settings;
	Indicators *indicators;
	if(!init_game_variables(&user_input, &settings, &indicators)){
		print_error(MEMORY);
		print_generic_message(QUIT);
		return 0;
	}
	Game *game;
	do {
		EngineMessage msg;
		if(indicators->state == GAME){
			if(indicators->print_game_prompt) print_board(game);
			print_player_color(game);
			get_user_input(GAME_PROMPT, user_input, INPUT_SIZE);
			GameCommand *cmd = parse_game_command(user_input);
			if(!cmd){
				print_error(MEMORY);
				break;
			}
			msg = execute_game_command(game, cmd);
			if(msg != SUCCESS){
				indicators->print_game_prompt = 0;
				handle_game_message(&game, msg, cmd, &settings, indicators);
			} else {
				indicators->print_game_prompt = 1;
			}
			free(cmd->arg);
			free(cmd);
		} else {
			char *prompt = NULL;
			if(indicators->print_settings_prompt){
				prompt = SETTINGS_PROMPT;
				indicators->print_settings_prompt = 0;
			}
			get_user_input(prompt, user_input, INPUT_SIZE);
			SettingCommand *cmd = parse_setting_command(user_input);
			if(!cmd){
				free(settings);
				print_error(MEMORY);
				break;
			}
			msg = execute_setting_command(settings, cmd);
			if(msg != SUCCESS)
				handle_settings_message(&game, msg, cmd, &settings, indicators);
			free(cmd->arg);
			free(cmd);
		}
	} while (!indicators->quit);

	free(user_input);
	return 0;
}
