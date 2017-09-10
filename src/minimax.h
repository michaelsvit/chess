#ifndef MINIMAX_H
#define MINIMAX_H value
#include "game_engine.h"

/*
 * Use minimax algorithm to determine computer move.
 * @param game 	 game instance
 * @return     	 pointer to suggested move on success, NULL otherwise
 */
GameMove *minimax_suggest_move(Game* game);

/*
 * Recursively and dynamicly generate minimax tree nodes to determine best move to perform.
 * @param game           	 game instance
 * @param max_depth      	 maximum recursion depth
 * @param suggested_move 	 pointer to game move struct to be returned to main function
 * @param alpha          	 maximal value guaranteed to AI player
 * @param beta           	 minimal value guaranteed to human player
 * @param error          	 flag to keep track of errors that occurred
 * @return               	 minimax value of current node
 */
int minimax_node(Game *game, int max_depth, GameMove **suggested_move, int alpha, int beta, int *error);

/*
 * Handle memory errors. Free all allocated resources and set error flag.
 * @param game  	 game copy instance
 * @param moves 	 possible moves list
 * @param error 	 error flag
 */
int quit_error(Game *game, SPArrayList *moves, int *error);

/*
 * Calculate minimax value of given game board.
 * @param game      	 game instance
 * @param game_over 	 flag indicating if given game is in a final state
 * @return          	 calculated value
 */
int board_value(Game *game, int game_over);

/*
 * Summarize values of all pieces belonging to given player.
 * @param game   	 game instance
 * @param player 	 player to calculate value for
 * @return       	 calculated value
 */
int player_pieces_value(Game *game, Player player);
#endif /* ifndef MINIMAX_H */
