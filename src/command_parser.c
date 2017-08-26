#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "command_parser.h"

/******************************** Interface functions ******************************/

SettingCommand *parse_setting_command(const char *command_str){
	/* Create copy of given string to prevent segfault when running tests on string literal */
	char *str_copy = (char *)malloc(strlen(command_str)+1);
	strcpy(str_copy, command_str);

	char *cmd_name, *cmd_arg;

	cmd_name = strtok(str_copy, DELIMITER);
	if(!cmd_name){
		free(str_copy);
		return NULL;
	}

	cmd_arg = strtok(NULL, DELIMITER);
	SettingCommand *cmd = get_settings_command(cmd_name, cmd_arg);

	/* check no extra arguments are given */
	cmd_arg = strtok(NULL, DELIMITER);
	if (cmd && cmd_arg != NULL) {
		cmd->type = INVALID_SETTING_COMMAND;
	}

	free(str_copy);
	return cmd;
}

GameCommand *parse_game_command(const char *command_str){
	/* Create copy of given string to prevent segfault when running tests on string literal */
	char *str_copy = (char *)malloc(strlen(command_str)+1);
	strcpy(str_copy, command_str);

	char *cmd_name;

	cmd_name = strtok(str_copy, DELIMITER);
	if(!cmd_name){
		free(str_copy);
		return NULL;
	}

	GameCommand *cmd = get_game_command(cmd_name);

	/* check no extra arguments are given */
	if (cmd && strtok(NULL, DELIMITER)) {
		cmd->type = INVALID_GAME_COMMAND;
	}

	free(str_copy);
	return cmd;
}

/******************************** Auxiliary functions ******************************/

SettingCommand *get_settings_command(char *cmd_name, char *cmd_arg){
	SettingCommand *cmd = (SettingCommand *)malloc(sizeof(SettingCommand));
	if(!cmd) return NULL;
	cmd->arg = NULL;

	if (strcmp(cmd_name, "game_mode") == 0) {
		cmd->type = GAME_MODE;
		if(is_valid_int(cmd_arg)){
			int *arg = (int *)malloc(sizeof(int));
			*arg = atoi(cmd_arg);
			cmd->arg = arg;
		}
	} else if (strcmp(cmd_name, "difficulty") == 0) {
		cmd->type = DIFFICULTY;
		if(is_valid_int(cmd_arg)){
			int *arg = (int *)malloc(sizeof(int));
			*arg = atoi(cmd_arg);
			cmd->arg = arg;
		}
	} else if (strcmp(cmd_name, "user_color") == 0) {
		cmd->type = USER_COLOR;
		if(is_valid_int(cmd_arg)){
			int *arg = (int *)malloc(sizeof(int));
			*arg = atoi(cmd_arg);
			cmd->arg = arg;
		}
	} else if (strcmp(cmd_name, "load") == 0) {
		cmd->type = LOAD;
		cmd->arg = malloc(strlen(cmd_arg)+1);
		strcpy((char *)cmd->arg, cmd_arg);
	} else if (strcmp(cmd_name, "default") == 0) {
		cmd->type = DEFAULT;
	} else if (strcmp(cmd_name, "print_setting") == 0) {
		cmd->type = PRINT_SETTING;
	} else if (strcmp(cmd_name, "quit") == 0) {
		cmd->type = SETTING_QUIT;
	} else if (strcmp(cmd_name, "start") == 0) {
		cmd->type = START;
	} else {
		cmd->type = INVALID_SETTING_COMMAND;
	}
	return cmd;
}

GameCommand *get_game_command(char *cmd_name){
	GameCommand *cmd = (GameCommand *)malloc(sizeof(GameCommand));
	if(!cmd) return NULL;
	cmd->arg = NULL;

	if(strcmp(cmd_name, "move") == 0){
		cmd->type = MOVE;
		cmd->arg = parse_move_args();
	} else if (strcmp(cmd_name, "get_moves") == 0) {
		cmd->type = GET_MOVES;
		cmd->arg = parse_position();
	} else if (strcmp(cmd_name, "save") == 0) {
		cmd->type = SAVE;
		char *arg = strtok(NULL, DELIMITER);
		cmd->arg = malloc(strlen(arg)+1);
		strcpy((char *)cmd->arg, arg);
	} else if (strcmp(cmd_name, "undo") == 0) {
		cmd->type = UNDO;
	} else if (strcmp(cmd_name, "reset") == 0) {
		cmd->type = RESET;
	} else if (strcmp(cmd_name, "quit") == 0) {
		cmd->type = GAME_QUIT;
	} else {
		cmd->type = INVALID_GAME_COMMAND;
	}
	return cmd;
}

int is_valid_int(const char *str){
	if(!str) return 0;

	for(int i = 0; str[i] != '\0'; i++){
		if(!((str[i] >= '0' && str[i] <= '9') || (i == 0 && (str[i] == '-' || str[i] == '+')))){
			return 0;
		}
	}
	return 1;
}

int *parse_move_args(){
	int *src = parse_position();
	if(!src) return NULL;
	/* Consume "to" word from command */
	char *word = strtok(NULL, DELIMITER);
	if(strcmp(word, "to") != 0) return NULL;
	int *dst = parse_position();
	if(!dst){
		free(src);
		return NULL;
	}

	int *args = (int *)malloc(sizeof(int) * 4);
	args[0] = src[0];
	args[1] = src[1];
	args[2] = dst[0];
	args[3] = dst[1];

	free(src);
	free(dst);
	return args;
}

int *parse_position(){
	char *arg = strtok(NULL, DELIMITER);
	if(!arg) return NULL;

	if(strlen(arg) != 5 || arg[0] != '<' || arg[2] != ',' || arg[4] != '>') return NULL;
	if(!(arg[1] >= '1' && arg[1] <= '8') || !(arg[3] >= 'A' && arg[3] <= 'H')) return NULL;

	int *pos = (int *)malloc(sizeof(int) * 2);
	if(!pos) return NULL;

	pos[0] = arg[1] - '0';
	pos[1] = arg[3] - 'A';
	return pos;
}
