#include "game_screen.h"
#include "../minimax.h"

EngineMessage create_game_screen(GameScreen **game_screen, SDL_Renderer *renderer) {
	EngineMessage msg;

	GameScreen *new_game_screen = (GameScreen*)calloc(1, sizeof(GameScreen));
	if (!new_game_screen) {
		return MALLOC_FAILURE;
	}

	SDL_Rect board_area = {.x = 200, .y = 0, .w = 800, .h = 800};
	msg = create_chess_board(&new_game_screen->chess_board, &board_area,renderer);
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect restart_area = {.x = 25, .y = 14, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->restart_button, &restart_area, renderer, "./images/restart.bmp", "./images/restart_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect save_area = {.x = 25, .y = 126, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->save_button, &save_area, renderer, "./images/save.bmp", "./images/save_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect load_area = {.x = 25, .y = 238, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->load_button, &load_area, renderer, "./images/load.bmp", "./images/load_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect undo_area = {.x = 25, .y = 350, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->undo_button, &undo_area, renderer, "./images/undo.bmp", "./images/undo_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect new_game_area = {.x = 25, .y = 462, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->new_game_button, &new_game_area, renderer, "./images/new_game.bmp", "./images/new_game_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect main_menu_area = {.x = 25, .y = 574, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->main_menu_button, &main_menu_area, renderer, "./images/main_menu.bmp", "./images/main_menu_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect quit_area = {.x = 25, .y = 686, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->quit_button, &quit_area, renderer, "./images/quit.bmp", "./images/quit_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	*game_screen = new_game_screen;
	return SUCCESS;
}

void destroy_game_screen(GameScreen *game_screen) {
	if (game_screen->chess_board) {
		destroy_chess_board(game_screen->chess_board);
	}
	if (game_screen->restart_button) {
		destroy_button(game_screen->restart_button);
	}
	if (game_screen->save_button) {
		destroy_button(game_screen->save_button);
	}
	if (game_screen->load_button) {
		destroy_button(game_screen->load_button);
	}
	if (game_screen->undo_button) {
		destroy_button(game_screen->undo_button);
	}
	if (game_screen->new_game_button) {
		destroy_button(game_screen->new_game_button);
	}
	if (game_screen->main_menu_button) {
		destroy_button(game_screen->main_menu_button);
	}
	if (game_screen->quit_button) {
		destroy_button(game_screen->quit_button);
	}
	free(game_screen);
}

EngineMessage start_new_game(GameSettings *settings, GameScreen *game_screen) {
	EngineMessage msg = SUCCESS;

	if (game_screen->game) {
		destroy_game(game_screen->game);
	}

	game_screen->game = create_game(settings);
	if (!game_screen->game) {
		return MALLOC_FAILURE;
	}
	if (game_screen->game->mode == ONE_PLAYER && game_screen->game->current_player == PLAYER2) {
		GameMove comp_move = minimax_suggest_move(game_screen->game, game_screen->game->difficulty);
		msg = move_game_piece(game_screen->game, comp_move.src_x, comp_move.src_y, comp_move.dst_x, comp_move.dst_y);
	}

	return msg;
}

EngineMessage draw_game_screen(SDL_Renderer *renderer, GameScreen *game_screen) {
	draw_chess_board(renderer, game_screen->chess_board, game_screen->game);
	draw_button(renderer, game_screen->restart_button);
	draw_button(renderer, game_screen->save_button);
	draw_button(renderer, game_screen->load_button);
	draw_button(renderer, game_screen->undo_button);
	draw_button(renderer, game_screen->new_game_button);
	draw_button(renderer, game_screen->main_menu_button);
	draw_button(renderer, game_screen->quit_button);

	return SUCCESS;
}

EngineMessage game_screen_event_handler(SDL_Event *event, GameScreen *game_screen, GameEvent *game_event) {
	game_event->type = NO_EVENT;
	EngineMessage msg = SUCCESS;
	GameEvent chess_board_event;
	GameMove *move;
	GameMove comp_move;
	
	chess_board_event_handler(event, game_screen->chess_board, &chess_board_event);
	if (chess_board_event.type == PIECE_MOVED) {
		msg = move_game_piece(game_screen->game, chess_board_event.data.move.prev_piece_col, chess_board_event.data.move.prev_piece_row, 
		                                         chess_board_event.data.move.new_piece_col, chess_board_event.data.move.new_piece_row);
	}

	if (button_event_handler(event, game_screen->restart_button) && game_screen->game) {
		msg = restart_game(game_screen->game);
	}

	if (button_event_handler(event, game_screen->save_button)) {
		game_event->type = SAVE_GAME;
	}

	if (button_event_handler(event, game_screen->load_button)) {
		game_event->type = MOVE_TO_LOAD_SCREEN;
	}

	if (button_event_handler(event, game_screen->undo_button) && game_screen->game) {
		msg = undo_move(game_screen->game, &move);
	}

	if (button_event_handler(event, game_screen->new_game_button)) {
		game_event->type = MOVE_TO_SETTINGS_SCREEN;
	}

	if (button_event_handler(event, game_screen->main_menu_button)) {
		game_event->type = MOVE_TO_MAIN_MENU;
	}

	if (button_event_handler(event, game_screen->quit_button)) {
		game_event->type = QUIT_GAME;
	}

	if (game_screen->game->mode == ONE_PLAYER && game_screen->game->current_player == PLAYER2) {
		comp_move = minimax_suggest_move(game_screen->game, game_screen->game->difficulty);
		msg = move_game_piece(game_screen->game, comp_move.src_x, comp_move.src_y, comp_move.dst_x, comp_move.dst_y);
	}

	return msg;
}
