#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "command_parser.h"

/******************************** Interface functions ******************************/

SettingCommand *parse_setting_command(const char *command_str){
	/* Create copy of given string to prevent segfault when running tests on string literal */
	char *str_copy = (char *)malloc(strlen(command_str)+1);
	if(!str_copy) return NULL;
	strcpy(str_copy, command_str);

	char *cmd_name, *cmd_arg;

	cmd_name = strtok(str_copy, DELIMITER);
	cmd_arg = strtok(NULL, DELIMITER);
	SettingCommand *cmd = get_settings_command(cmd_name, cmd_arg);

	/* check no extra arguments are given */
	cmd_arg = strtok(NULL, DELIMITER);
	if (cmd && cmd->valid_arg && cmd_arg != NULL) {
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
	GameCommand *cmd = get_game_command(cmd_name);

	/* check no extra arguments are given */
	if (cmd && cmd->valid_arg && strtok(NULL, DELIMITER)) {
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
	cmd->valid_arg = 0;
	if(!cmd_name){
		cmd->type = INVALID_SETTING_COMMAND;
		return cmd;
	}

	if (strcmp(cmd_name, "game_mode") == 0) {
		if(!get_arg_command(cmd, cmd_arg, GAME_MODE)) return NULL;
	} else if (strcmp(cmd_name, "difficulty") == 0) {
		if(!get_arg_command(cmd, cmd_arg, DIFFICULTY)) return NULL;
	} else if (strcmp(cmd_name, "user_color") == 0) {
		if(!get_arg_command(cmd, cmd_arg, USER_COLOR)) return NULL;
	} else if (strcmp(cmd_name, "load") == 0) {
		if(!get_load_command(cmd, cmd_arg)){
			free(cmd);
			return NULL;
		}
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

int get_arg_command(SettingCommand *cmd, char *cmd_arg, SettingCommandType type){
	if(!cmd_arg){
		cmd->type = INVALID_SETTING_COMMAND;
		return 1;
	}
	cmd->type = type;
	if(is_valid_int(cmd_arg)){
		int *arg = (int *)malloc(sizeof(int));
		if(!arg){
			free(cmd);
			return 0;
		}
		*arg = atoi(cmd_arg);
		cmd->arg = arg;
		cmd->valid_arg = 1;
	}
	return 1;
}

int get_load_command(SettingCommand *cmd, char *cmd_arg){
	if(cmd_arg){
		cmd->type = LOAD;
		cmd->arg = malloc(strlen(cmd_arg)+1);
		if(!cmd->arg){
			return 0;
		}
		strcpy((char *)cmd->arg, cmd_arg);
		cmd->valid_arg = 1;
	} else {
		cmd->type = INVALID_SETTING_COMMAND;
	}
	return 1;
}

GameCommand *get_game_command(char *cmd_name){
	GameCommand *cmd = (GameCommand *)malloc(sizeof(GameCommand));
	if(!cmd) return NULL;
	cmd->arg = NULL;
	cmd->valid_arg = 0;
	if(!cmd_name){
		cmd->type = INVALID_GAME_COMMAND;
		return cmd;
	}

	if(strcmp(cmd_name, "move") == 0){
		if(!get_move_command(cmd)){
			free(cmd);
			return NULL;
		}
	} else if (strcmp(cmd_name, "get_moves") == 0) {
		if(!get_get_moves_command(cmd)){
			free(cmd);
			return NULL;
		}
	} else if (strcmp(cmd_name, "save") == 0) {
		if(!get_save_command(cmd)){
			free(cmd);
			return NULL;
		}
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

int get_move_command(GameCommand *cmd){
	cmd->type = MOVE;
	int *src;
	int status = get_position(&src, cmd);
	if(status != 1) return status;

	/* Consume "to" word from command */
	char *middle_word = strtok(NULL, DELIMITER);
	if(!middle_word || strcmp(middle_word, "to") != 0){
		free(src);
		cmd->type = INVALID_GAME_COMMAND;
		return 1;
	}

	int *dst;
	status = get_position(&dst, cmd);
	if(status != 1){
		free(src);
		return status;
	}

	int *args = (int *)malloc(sizeof(int) * 4);
	if(!args){
		free(src);
		free(dst);
		return 0;
	}
	copy_positions_to_array(args, src, dst);

	free(src);
	free(dst);
	cmd->arg = args;
	return 1;
}

int get_position(int **pos, GameCommand *cmd){
	*pos = parse_position(&cmd->valid_arg);
	if(!cmd->valid_arg){
		if(!*pos){
			return 0;
		} else {
			cmd->type = INVALID_GAME_COMMAND;
			free(*pos);
			return -1;
		}
	} else {
		if(!*pos){
			cmd->valid_arg = 0;
			return -1;
		}
	}
	return 1;
}

void copy_positions_to_array(int *args, int *src, int *dst){
	args[0] = src[0];
	args[1] = src[1];
	args[2] = dst[0];
	args[3] = dst[1];
}

int get_get_moves_command(GameCommand *cmd){
	cmd->type = GET_MOVES;
	cmd->arg = parse_position(&cmd->valid_arg);
	if(!cmd->arg){
		if(!cmd->valid_arg){
			return 0;
		} else {
			cmd->valid_arg = 0;
		}
	} else {
		if(!cmd->valid_arg){
			cmd->type = INVALID_GAME_COMMAND;
		}
	}
	return 1;
}

int get_save_command(GameCommand *cmd){
	cmd->type = SAVE;
	char *arg = strtok(NULL, DELIMITER);
	if(!arg){
		cmd->type = INVALID_GAME_COMMAND;
	} else {
		cmd->arg = malloc(strlen(arg)+1);
		if(!cmd->arg){
			return 0;
		}
		strcpy((char *)cmd->arg, arg);
		cmd->valid_arg = 1;
	}
	return 1;
}

int is_valid_int(const char *str){
	for(int i = 0; str[i] != '\0'; i++){
		if(!((str[i] >= '0' && str[i] <= '9') || (i == 0 && (str[i] == '-' || str[i] == '+')))){
			return 0;
		}
	}
	return 1;
}

int *parse_position(int *valid_arg){
	int *pos = (int *)malloc(sizeof(int) * 2);
	if(!pos) return NULL;

	char *arg = strtok(NULL, DELIMITER);
	if(!arg) return pos;

	if(arg[0] != '<' || !strchr(arg, ',') || arg[strlen(arg)-1] != '>') return pos;
	*valid_arg = 1;
	if(strlen(arg) != 5
			|| !(arg[1] >= '1' && arg[1] <= '8')
			|| !(arg[3] >= 'A' && arg[3] <= 'H')){
		free(pos);
		return NULL;
	}

	pos[1] = arg[1] - '0' - 1;
	pos[0] = arg[3] - 'A';
	return pos;
}
