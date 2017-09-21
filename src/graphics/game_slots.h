#ifndef GAME_SLOTS_H
#define GAME_SLOTS_H

#include "../game_engine.h"

#define MAX_SAVED_GAME 5

int num_saved_games();

Game* load_saved_game(int slot);

EngineMessage new_saved_game(Game *game);

#endif
