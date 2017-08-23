#include <stdlib.h>
#include <stdio.h>
#include "main_aux.h"

int main(/* int argc, char *argv[] */){
	Game *game = create_game(DEFAULT_MODE, DEFAULT_DIFFICULTY, DEFAULT_PLAYER1_COLOR);
	if(!game){
		/* TODO: Print error */
		return 0;
	}

	char *user_input = (char *)malloc(INPUT_SIZE);
	if(!user_input){
		destroy_game(game);
		/* TODO: Print error */
		return 0;
	}
	int quit = 0;
	State state = SETTINGS;
	do {
		if(scanf("%s", user_input) != 1){
			/* TODO: Print error */
			break;
		}
		EngineMessage msg;
		if(state == GAME){
			GameCommand *cmd = parse_game_command(user_input);
			if(!cmd){
				/* TODO: Print error */
				break;
			}
			msg = execute_game_command(&game, cmd);
			free(cmd);
		} else {
			SettingCommand *cmd = parse_setting_command(user_input);
			if(!cmd){
				/* TODO: Print error */
				break;
			}
			msg = execute_setting_command(&game, cmd);
			free(cmd);
		}
		if(msg != SUCCESS){
			if(msg != MALLOC_FAILURE){
				/* print_error(msg); */
			} else {
				/* TODO: Print error */
				break;
			}
		}
	} while (!quit);

	free(user_input);
	destroy_game(game);
	return 0;
}
