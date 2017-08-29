#ifndef MAIN_AUX_H
#define MAIN_AUX_H

/*
 * Auxiliary functions for main function.
 */

#include "command_parser.h"
#include "game_engine.h"
#include "print_utils.h"

#define INPUT_SIZE 1024

#define DEFAULT_MODE 1
#define DEFAULT_DIFFICULTY 2
#define DEFAULT_PLAYER1_COLOR 1

typedef enum {
	SETTINGS,
	GAME
} State;

typedef struct {
	int quit;
	int print_settings_prompt;
	int print_game_prompt;
	State state;
} Indicators;

/*
 * Initialize indicators required for game management.
 * @param user_input 	 buffer for user input
 * @param settings   	 settings struct to initialize
 * @param indicators 	 indicators struct to initialize
 */
int init_game_variables(char **user_input, GameSettings **settings, Indicators **indicators);

/*
 * Create and initialize an indicators struct for main function management.
 * @return	pointer to indicators struct on success, NULL otherwise
 */
Indicators *create_indicators();

/*
 * Get command input from user.
 * @param prompt 	 prompt to display to user
 * @param buf    	 buffer to hold read content
 * @param len    	 maximum input length
 */
void get_user_input(const char* prompt, char* buf, int len);

/*
 * Execute given game command.
 * @param game 	 game instance
 * @param cmd  	 command given by user in game state
 * @return
 * MALLOC_FAILURE   	 memory failure occurred
 * INVALID_COMMAND  	 command given by user is invalid
 * INVALID_ARGUMENT 	 required argument given to command is invalid
 * ILLEGAL_MOVE     	 user issued an illegal chess move
 * EMPTY_HISTORY    	 user issued undo command when move history is empty
 * SUCCESS          	 otherwise
 */
EngineMessage execute_game_command(Game *game, GameCommand *cmd);

/*
 * Handler for engine messages returned in game state.
 * @param game     	 game instance
 * @param msg      	 returned message
 * @param cmd      	 command that was issued by user
 * @param settings 	 pointer to main function's settings variable in case of restart
 * @param state    	 game state
 * @param quit     	 flag indicating to quit the game
 */
void handle_game_message(Game **game, EngineMessage msg, GameCommand *cmd, GameSettings **settings, Indicators *indicators);

/*
 * Execute given setting command.
 * @param game     	 game instance
 * @param settings 	 current set of settings to be used by game
 * @param cmd      	 command given by user in settings state
 * @return
 * MALLOC_FAILURE   	 memory failure occurred
 * INVALID_COMMAND  	 command given by user is invalid
 * INVALID_ARGUMENT 	 required argument given to command is invalid
 * SUCCESS          	 otherwise
 */
EngineMessage execute_setting_command(GameSettings *settings, SettingCommand *cmd);

/*
 * Handler for engine messages returned in settings state.
 * @param game     	 game instance
 * @param msg      	 returned message
 * @param cmd      	 command that was issued by user
 * @param settings 	 pointer to main function's settings variable in case of restart
 * @param state    	 game state
 * @param quit     	 flag indicating to quit the game
 */
void handle_settings_message(Game **game, EngineMessage msg, SettingCommand *cmd, GameSettings **settings, Indicators *indicators);

/*
 * Handle game engine messages.
 * @param msg      	 message to be handled
 * @param settings 	 pointer to main function's settings struct
 * @param state    	 pointer to variable holding game state
 * @param quit     	 pointer to game quit flag
 */
void handle_message(EngineMessage msg, Game **game, GameSettings **settings, Indicators *indicators);

/*
 * Create default game settings.
 * @return 	 pointer to created struct on success, NULL otherwise
 */
GameSettings *create_settings();

/*
 * Reset given settings struct to default settings.
 * @param 	 pointer to an existing settings struct
 */
void set_default_settings(GameSettings *settings);

#endif /* ifndef MAIN_AUX_H */
