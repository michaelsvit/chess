#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

/*
 * Printing utility functions.
 */

#include "game_engine.h"

#define POS_REPR_MAX_LENGTH 5

/*
 * Print game board in console mode.
 * @precond    	 assumes game != NULL
 * @param game 	 game instance
 */
void print_board(Game *game);

/*
 * Get character representing given game piece.
 * @param piece 	 game piece to represent
 * @return      	 character representing given piece
 */
char get_piece_repr(GamePiece *piece);

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
