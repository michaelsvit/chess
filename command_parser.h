#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H value

/*
 * Module for parsing user commands in either settings or game state.
 *
 * Summary of interface:
 * parse_settings_command 	 parse given user command in settings state
 * parse_game_command     	 parse given user command in game state
 */

#define DELIMITER  " \t\r\n"

typedef enum setting_command_type{
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

typedef struct setting_command{
	SettingCommandType type;
	void *arg;
} SettingCommand;

typedef enum game_command_type{
	INVALID_GAME_COMMAND,
	MOVE,
	GET_MOVES,
	SAVE,
	UNDO,
	RESET,
	GAME_QUIT
} GameCommandType;

typedef struct game_command{
	GameCommandType type;
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
 * Get game command according to given name and argument.
 * @param cmd_name  	 name of user command
 * @return          	 pointer to parsed command on success, NULL otherwise
 */
GameCommand *get_game_command(char *cmd_name);

/*
 * Check if given string is a valid integer.
 * @param str 	 string to be checked
 * @return    	 true iff str represents a valid integer
 */
int is_valid_int(const char *str);

/*
 * Parse arguments for a move command.
 * @return 	 {src_x, src_y, dst_x, dst_y} on success, NULL otherwise
 */
int *parse_move_args();

/*
 * Parse position from argument string of the form <pos_y,pos_x> where x=column, y=row.
 * @return 	 {pos_x, pos_y} on success, NULL otherwise
 */
int *parse_position();

#endif /* ifndef COMMAND_PARSER_H */
