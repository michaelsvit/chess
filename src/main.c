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
	int print_game_user_prompt = 1; /* Used to determine if last command was invalid */
	State state = SETTINGS;
	Game *game;
	do {
		EngineMessage msg;
		if(state == GAME){
			if(print_game_user_prompt) print_board(game);
			print_player_color(game);
			get_user_input(GAME_PROMPT, user_input, INPUT_SIZE);
			GameCommand *cmd = parse_game_command(user_input);
			if(!cmd){
				print_error(MEMORY);
				break;
			}
			msg = execute_game_command(game, cmd);
			if(msg != SUCCESS){
				print_game_user_prompt = 0;
				handle_game_message(&game, msg, cmd, &settings, &state, &quit);
			} else {
				print_game_user_prompt = 1;
			}
			free(cmd->arg);
			free(cmd);
		} else {
			char *prompt = NULL;
			if(print_settings_prompt){
				prompt = SETTINGS_PROMPT;
				print_settings_prompt = 0;
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
				handle_settings_message(&game, msg, cmd, &settings, &state, &quit);
			free(cmd->arg);
			free(cmd);
		}
	} while (!quit);

	free(user_input);
	return 0;
}
