#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "game_engine.h"

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax alpha-betha pruning algorithm up to a
 * specified length given by maxDepth. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param currentGame 	 game instance
 * @param maxDepth    	 maximum recursion depth
 * @return            	 pointer to move suggested by minimax on success, NULL otherwise
 */
GameMove *minimax_suggest_move(Game* currentGame, unsigned int maxDepth);

/*
 * Calculate total value of all pieces of given piece color.
 * @param game  	 game instance
 * @param color 	 piece color to summarize values of
 * @return      	 calculated value
 */
int color_score(Game* game, Color color);

/*
 * Calculate score of given game instance's board.
 * @param game 	 game instance
 * @return     	 calculated score
 */
int score(Game* game);

/*
 * Get MiniMax value of the board when game is over.
 * @param current_player_color 	 color of current player in the game
 * @param check_mate           	 determines if game ended with a check-mate
 * 								 (otherwise it's a tie)
 * @return                     	 value of game board at this state
 */
int get_game_over_value(Color current_player_color, int check_mate);

/*
 * Check minimax subtree starting at this node.
 * @param game             	 game instance
 * @param cur_player_color 	 color of current player in the game
 * @param suggested_move   	 pointer to move struct to be returned to main function
 * @param max_depth        	 maximum depth of recursion to perform
 * @param alpha            	 alpha value (pruning)
 * @param beta             	 beta value (pruning)
 * @return
 * MALLOC_FAILURE 	 memory error occurred, stopping recursion
 * SUCCESS        	 otherwise
 */
EngineMessage check_minimax_subtree(
		Game *game,
		Color cur_player_color,
		GameMove *suggested_move,
		int max_depth,
		int *alpha,
		int *beta);

/*
 * Recursively generate MiniMax tree nodes to find a move that yields highest-value score.
 * @param game           	 game instance
 * @param max_depth      	 maximum depth of recursion to perform
 * @param suggested_move 	 pointer to move struct to be returned to main function
 * @param son_score      	 pointer to variable to be assigned value of current node
 * @param alpha          	 alpha value (pruning)
 * @param beta           	 beta value (pruning)
 * @return
 * MALLOC_FAILURE 	 memory error occurred, stopping recursion
 * SUCCESS        	 otherwise
 */
EngineMessage minimax_node(
		Game* game,
		unsigned int max_depth,
		GameMove* suggested_move,
		int *son_score,
		int alpha,
		int beta);

#endif
