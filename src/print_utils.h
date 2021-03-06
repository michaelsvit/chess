#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

/*
 * Printing utility functions.
 */

#include "game_engine.h"
#include "command_parser.h"

#define GAME_PROMPT "player - enter your move:\n"
#define SETTINGS_PROMPT "Specify game setting or type 'start' to begin a game with the current setting:\n"

#define POS_REPR_MAX_LENGTH 7

/*
 * Print color of current player.
 * @param game 	 game instance
 */
void print_player_color(Game *game);

/*
 * Print invalid argument error for given game command.
 * @param game 	 game instance
 * @param msg  	 returned engine message
 * @param cmd  	 command issued by user
 */
void print_game_error(Game *game, EngineMessage msg, GameCommand *cmd);

/*
 * Print message describing move executed by AI player.
 * @param type 	 type of game piece that was moved
 * @param move 	 move executed by AI
 */
void print_computer_move(PieceType type, GameMove *move);

/*
 * Convert piece type into a representing string.
 * @param type 	 type of game piece to represent
 * @return     	 string representing given piece
 */
char *get_type_repr(PieceType type);

/*
 * Convert coordinates into a string representing given position on board.
 * @param pos_x 	 column of given position
 * @param pos_y 	 row of given position
 */
void print_position_repr(int pos_x, int pos_y);

/*
 * Print message to user according to given engine message.
 * @param msg 	 message to be printed
 */
void print_generic_message(EngineMessage msg);

/*
 * Print invalid argument error for given settings command.
 * @param cmd 	 command issued by user
 */
void print_settings_error(SettingCommand *cmd);

/*
 * Print two moves that were undone by user.
 * @param game  	 game instance
 * @param moves 	 moves that were undone
 */
void print_undo_two_moves(Game *game, GameMove *moves[]);

/*
 * Print message informing user his king is threatened.
 * @param color          	 color of current player
 * @param mode           	 game mode
 * @param current_player 	 current game player
 */
void print_check(Color color, Mode mode, Player current_player);

/*
 * Print message informing user game is over and announce winner (or tie).
 * @param game 	 game instance
 */
void print_game_over(Game *game);

/*
 * Print which mode the game has been set to.
 * @param mode 	 current game mode
 */
void print_game_mode(Mode mode);

/*
 * Print game board in console mode.
 * @precond    	 assumes game != NULL
 * @param game 	 game instance
 */
void print_board(Game *game);

/*
 * Print given settings.
 * @param settings 	 settings of current game to be printed
 */
void print_settings(GameSettings *settings);

/*
 * Print moves from given list in sorted order, and add special characters
 * as defined in project requirements.
 * @param game  	 game instance
 * @param moves 	 list of possible moves to print
 * @return      	 true on success, false on memory failure
 */
int print_possible_moves(Game *game, SPArrayList *moves);

/*******************************************************************************************/
/**************** Auxiliary functions - should not be called outside this module ***********/
/*******************************************************************************************/

/*
 * Get character representing given game piece.
 * @param piece 	 game piece to represent
 * @return      	 character representing given piece
 */
char get_piece_repr(GamePiece *piece);

/*
 * Get string representing given move's destination with special characters (if needed).
 * @param game 	 game instance
 * @param move 	 game move
 * @return     	 string representing destination on success, NULL on memory failure
 */
char *get_destination_repr(Game *game, GameMove *move);

/*
 * Compare two given game positions.
 * @param a_x	column of position a
 * @param a_y	row of position a
 * @param b_x	column of position b
 * @param b_y	row of position b
 * @return  	 1 if a>b, 0 if a==b, -1 if a<b (order given in project requirements)
 */
int compare_positions(int a_x, int a_y, int b_x, int b_y);

#endif /* ifndef PRINT_UTILS_H */
