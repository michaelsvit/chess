#include "game_engine.h"

Game *create_game(){
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
	game->current_player = Player.PLAYER1;
	return game;
}

void destroy_game(Game *game){
	if(!game) return;
	spArrayListDestroy(game->white_pieces);
	spArrayListDestroy(game->black_pieces);
}
