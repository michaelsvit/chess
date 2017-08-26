#include <stdlib.h>
#include <stdio.h>
#include "main_aux.h"

int main(/* int argc, char *argv[] */){
	char *user_input = (char *)malloc(INPUT_SIZE);
	int quit = 0;
	State state = SETTINGS;
	Game *game;
	GameSettings *settings = create_settings();
	do {
		EngineMessage msg;
		if(state == GAME){
			print_board(game);
			get_user_input("Enter game command: ", user_input, INPUT_SIZE);
			GameCommand *cmd = parse_game_command(user_input);
			if(!cmd){
				/* TODO: Print error */
				break;
			}
			msg = execute_game_command(game, cmd);
			free(cmd);
		} else {
			get_user_input("Enter settings command: ", user_input, INPUT_SIZE);
			SettingCommand *cmd = parse_setting_command(user_input);
			if(!cmd){
				/* TODO: Print error */
				break;
			}
			msg = execute_setting_command(settings, cmd);
			free(cmd);
		}
		if(msg != SUCCESS) handle_message(msg, &game, &settings, &state, &quit);
	} while (!quit);

	free(user_input);
	return 0;
}
