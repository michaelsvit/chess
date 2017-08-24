#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

/*
 * Printing utility functions.
 */

#include "game_engine.h"

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

#endif /* ifndef PRINT_UTILS_H */
