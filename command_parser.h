#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H value

/*
 * Module for parsing user commands in either settings or game state.
 *
 * Summary of interface:
 * parse_settings_command 	 parse given user command in settings state
 * parse_game_command     	 parse given user command in game state
 */

typedef enum setting_command_type{
	INVALID_COMMAND,
	GAME_MODE,
	DIFFICULTY,
	USER_COLOR,
	LOAD,
	DEFAULT,
	PRINT_SETTING,
	QUIT,
	START
} SettingCommandType;

typedef struct setting_command{
	SettingCommandType type;
	void *arg;
} SettingCommand;

/*
 * Parse command given by user in settings state.
 * @param command_str 	 string of user input
 * @return            	 pointer to parsed command on success, NULL otherwise
 */
SettingCommand *parse_setting_command(const char *command_str);

/*******************************************************************************************/
/**************** Auxiliary functions - should not be called outside this module ***********/
/*******************************************************************************************/

/*
 * Get command according to given name and argument.
 * @param cmd_name 	 name of user command
 * @param cmd_arg  	 argument of user command
 * @return         	 pointer to parsed command on success, NULL otherwise
 */
SettingCommand *get_settings_command(char *cmd_name, char *cmd_arg);

/*
 * Check if given string is a valid integer.
 * @param str 	 string to be checked
 * @return    	 true iff str represents a valid integer
 */
int is_valid_int(const char *str);

#endif /* ifndef COMMAND_PARSER_H */
