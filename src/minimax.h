#ifndef MINIMAX_H
#define MINIMAX_H
#include "game_engine.h"

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax alpha-betha pruning algorithm up to a
 * specified depth given by max_depth. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param game - The current game state
 * @param max_depth - The maximum depth of the algorithm
 * @param suggestem_move - On success, will be filled with the suggested move calculated by the algorithm.
 */
EngineMessage minimax_suggest_move(Game* game, unsigned int max_depth, GameMove *suggested_move);

#endif /* ifndef MINIMAX_H */

