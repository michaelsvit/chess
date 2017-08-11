#include <stdlib.h>
#include "game_engine.h"

Game *create_game(Mode mode, int difficulty, Color player1_color){
	if(difficulty < 1 || difficulty > 4) return NULL;
	Game *game = malloc(sizeof(Game));
	if(!game) return NULL;
	game->white_pieces = spArrayListCreate(ARRAY_SIZE);
	if(!game->white_pieces){
		free(game);
		return NULL;
	}
	game->black_pieces = spArrayListCreate(ARRAY_SIZE);
	if(!game->black_pieces){
		spArrayListDestroy(game->white_pieces);
		free(game);
		return NULL;
	}
	game->current_player = PLAYER1;
	game->mode = mode;
	game->difficulty = difficulty;
	game->player_color[PLAYER1] = player1_color;
	game->player_color[PLAYER2] = !player1_color;
	return game;
}

void destroy_game(Game *game){
	if(!game) return;
	spArrayListDestroy(game->white_pieces);
	spArrayListDestroy(game->black_pieces);
}
