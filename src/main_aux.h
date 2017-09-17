#ifndef MAIN_AUX_H
#define MAIN_AUX_H

/*
 * Auxiliary functions for main function.
 */

#include "command_parser.h"
#include "game_engine.h"
#include "minimax.h"
#include "print_utils.h"
#include "xml_serializer.h"

#define INPUT_SIZE 1024

#define DEFAULT_MODE 1
#define DEFAULT_DIFFICULTY 2
#define DEFAULT_PLAYER1_COLOR 1

typedef enum {
	SETTINGS,
	GAME
} RunState;

typedef struct {
	int quit;
	int print_settings_prompt;
	int print_game_prompt;
	int game_loaded;
	RunState run_state;
} Indicators;

/* Struct to hold all of the main function's state variables */
typedef struct {
	char *user_input;
	GameSettings *settings;
	Game *game;
	Indicators *indicators;
} ProgramState;

/*
 * Create and initialize all required variables for program management.
 * @return 	 pointer to struct holding variables on success, NULL otherwise
 */
ProgramState *create_program_state();

/*
 * Free all allocated program management memory.
 * @param state 	 program state struct
 */
void destroy_program_state(ProgramState *state);

/*
 * Initialize program state required for game management.
 * @param prog_state 	 struct holding all program state variables
 */
int init_program_state(ProgramState *prog_state);

/*
 * Create and initialize an indicators struct for main function management.
 * @return 	 pointer to indicators struct on success, NULL otherwise
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
 * Fetch and execute next game command from user.
 * @param state 	 program state struct
 */
int fetch_and_exe_game(ProgramState *state);

/*
 * Fetch and execute next (move) game command from AI player.
 * @param state 	 program state struct
 */
int fetch_and_exe_ai(ProgramState *state);

/*
 * Fetch and execute next game command from human player.
 * @param state 	 program state struct
 */
int fetch_and_exe_user(ProgramState *state);

/*
 * Fetch and execute next settings command from user.
 * @param state 	 program state struct
 */
int fetch_and_exe_settings(ProgramState *state);

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
 * @param state    	 program state struct
 * @param msg      	 returned message
 * @param cmd      	 command that was issued by user
 */
void handle_game_message(ProgramState *state, EngineMessage msg, GameCommand *cmd);

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
 * @param state 	 program state struct
 * @param msg   	 returned message
 * @param cmd   	 command that was issued by user
 */
void handle_settings_message(ProgramState *state, EngineMessage msg, SettingCommand *cmd);

/*
 * Handle game engine messages.
 * @param state 	 program state struct
 * @param msg   	 message to be handled
 */
void handle_message(ProgramState *state, EngineMessage msg);

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
