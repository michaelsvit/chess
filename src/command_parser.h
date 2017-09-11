#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

/*
 * Module for parsing user commands in either settings or game state.
 *
 * Summary of interface:
 * parse_settings_command 	 parse given user command in settings state
 * parse_game_command     	 parse given user command in game state
 */

#define DELIMITER  " \t\r\n"

typedef enum {
	INVALID_SETTING_COMMAND,
	GAME_MODE,
	DIFFICULTY,
	USER_COLOR,
	LOAD,
	DEFAULT,
	PRINT_SETTING,
	SETTING_QUIT,
	START
} SettingCommandType;

typedef struct {
	SettingCommandType type;
	int valid_arg;
	void *arg;
} SettingCommand;

typedef enum {
	INVALID_GAME_COMMAND,
	MOVE,
	GET_MOVES,
	SAVE,
	UNDO,
	RESET,
	GAME_QUIT
} GameCommandType;

typedef struct {
	GameCommandType type;
	int valid_arg;
	void *arg;
} GameCommand;

/*
 * Parse command given by user in settings state.
 * @param command_str 	 string of user input
 * @return            	 pointer to parsed command on success, NULL otherwise
 */
SettingCommand *parse_setting_command(const char *command_str);

/*
 * Parse command given by user in game state.
 * @param command_str 	 string of user input
 * @return            	 pointer to parsed command on success, NULL otherwise
 */
GameCommand *parse_game_command(const char *command_str);

/*******************************************************************************************/
/**************** Auxiliary functions - should not be called outside this module ***********/
/*******************************************************************************************/

/*
 * Get setting command according to given name and argument.
 * @param cmd_name 	 name of user command
 * @param cmd_arg  	 argument of user command
 * @return         	 pointer to parsed command on success, NULL otherwise
 */
SettingCommand *get_settings_command(char *cmd_name, char *cmd_arg);

/*
 * Initialize a command that requires a single integer argument.
 * @param cmd     	 command to be initialized
 * @param cmd_arg 	 argument to command
 * @param type    	 type of the command
 * @return        	 true on success, false on memory failure
 */
int get_arg_command(SettingCommand *cmd, char *cmd_arg, SettingCommandType type);

/*
 * Initialize a load command with given argument.
 * @param cmd     	 command to be initialized
 * @param cmd_arg 	 path (relative or absolute) to file to load from
 * @return        	 true on success, false on memory failure
 */
int get_load_command(SettingCommand *cmd, char *cmd_arg);

/*
 * Get game command according to given name and argument.
 * @param cmd_name  	 name of user command
 * @return          	 pointer to parsed command on success, NULL otherwise
 */
GameCommand *get_game_command(char *cmd_name);

/*
 * Parse a move command given by user.
 * @param cmd 	 command to be returned
 */
int get_move_command(GameCommand *cmd);

/*
 * Get one of the required positions for a move command from user input.
 * @param pos 	 array to hold position
 * @param cmd 	 command to be returned
 * @return    	 1 on success, 0 on malloc failure, -1 on success but invalid command or argument
 */
int get_position(int **pos, GameCommand *cmd);

/*
 * Copy parsed source and destination positions into returned argument array.
 * @param args 	 argument array to be returned with command
 * @param src  	 source position {src_x, src_y}
 * @param dst  	 destination position {dst_x, dst_y}
 */
void copy_positions_to_array(int *args, int *src, int *dst);

/*
 * Parse a get_moves command given by user.
 * @param cmd 	 command to be returned
 */
int get_get_moves_command(GameCommand *cmd);

/*
 * Get a save command given by user.
 * @param cmd 	 command to be returned
 */
int get_save_command(GameCommand *cmd);

/*
 * Check if given string is a valid integer.
 * @param str 	 string to be checked
 * @return    	 true iff str represents a valid integer
 */
int is_valid_int(const char *str);

/*
 * Parse arguments for a move command.
 * @param valid_arg 	 command property to be set true or false according to argument validity
 * @return          	 {src_x, src_y, dst_x, dst_y} on success, NULL otherwise
 */
int *parse_move_args(int *valid_arg);

/*
 * Parse position from argument string of the form <pos_y,pos_x> where x=column, y=row.
 * @param valid_arg 	 command property to be set true or false according to argument validity
 * @return          	 {pos_x, pos_y} on success, NULL otherwise
 */
int *parse_position(int *valid_arg);

#endif /* ifndef COMMAND_PARSER_H */
