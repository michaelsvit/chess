#include <stdlib.h>
#include <string.h>
#include "command_parser.h"

/******************************** Interface functions ******************************/

SettingCommand *parse_setting_command(const char *command_str){
	/* Create copy of given string to prevent segfault when running tests on string literal */
	char *str_copy = (char *)malloc(strlen(command_str)+1);
	strcpy(str_copy, command_str);

	char *cmd_name, *cmd_arg;
	const char *delimeter = " \t\r\n";

	cmd_name = strtok(str_copy, delimeter);
	if(!cmd_name){
		free(str_copy);
		return NULL;
	}

	cmd_arg = strtok(NULL, delimeter);
	SettingCommand *cmd = get_settings_command(cmd_name, cmd_arg);

	/* check no extra arguments are given */
	cmd_arg = strtok(NULL, delimeter);
	if (cmd && cmd_arg != NULL) {
		cmd->type = INVALID_COMMAND;
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
		cmd->type = QUIT;
	} else if (strcmp(cmd_name, "start") == 0) {
		cmd->type = START;
	} else {
		cmd->type = INVALID_COMMAND;
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
