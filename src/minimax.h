#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "game_engine.h"

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax alpha-betha pruning algorithm up to a
 * specified length given by maxDepth. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param currentGame - The current game state
 * @param maxDepth - The maximum depth of the algorithm
 * @return
 * NULL if either currentGame is NULL or maxDepth <= 0.
 * On success the function returns GameMove
 * which is the best move for the current player.
 */
GameMove minimax_suggest_move(Game* currentGame, unsigned int maxDepth);

#endif
