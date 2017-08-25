#ifndef MAIN_AUX_H
#define MAIN_AUX_H

/*
 * Auxiliary functions for main function.
 */

#include "command_parser.h"
#include "game_engine.h"
#include "print_utils.h"

#define INPUT_SIZE 1024

#define DEFAULT_MODE 2
#define DEFAULT_DIFFICULTY 2
#define DEFAULT_PLAYER1_COLOR 1

typedef enum {
	SETTINGS,
	GAME
} State;

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
EngineMessage execute_game_command(Game **game, GameCommand *cmd);

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
 * Handle game engine messages.
 * @param msg      	 message to be handled
 * @param settings 	 pointer to main function's settings struct
 * @param state    	 pointer to variable holding game state
 * @param quit     	 pointer to game quit flag
 */
void handle_message(EngineMessage msg, Game **game, GameSettings **settings, State *state, int *quit);

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
