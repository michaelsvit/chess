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
	GameMove comp_move;

	if (game_screen->game) {
		destroy_game(game_screen->game);
	}

	game_screen->game = create_game(settings);
	if (!game_screen->game) {
		return MALLOC_FAILURE;
	}
	if (game_screen->game->mode == ONE_PLAYER && game_screen->game->current_player == PLAYER2) {
		msg = minimax_suggest_move(game_screen->game, game_screen->game->difficulty, &comp_move);
		if (msg != SUCCESS) {
			return msg;
		}
		msg = move_game_piece(game_screen->game, comp_move.src_x, comp_move.src_y, comp_move.dst_x, comp_move.dst_y);
	}

	return msg;
}

EngineMessage start_load_game(int slot_num, GameScreen *game_screen) {
	game_screen->game = load_saved_game(slot_num);
	if (game_screen->game == NULL) {
		return LOAD_ERROR;
	}
	return SUCCESS;
}

EngineMessage draw_game_screen(SDL_Renderer *renderer, GameScreen *game_screen) {
	EngineMessage err;
	err = draw_chess_board(renderer, game_screen->chess_board, game_screen->game);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, game_screen->restart_button);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, game_screen->save_button);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, game_screen->load_button);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, game_screen->new_game_button);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, game_screen->main_menu_button);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, game_screen->quit_button);
	if (err != SUCCESS) {
		return err;
	}

	if (game_screen->game->mode == ONE_PLAYER && spArrayListSize(game_screen->game->move_history) >= 2) {
		err = draw_button(renderer, game_screen->undo_button);
		if (err != SUCCESS) {
			return err;
		}
	}

	return SUCCESS;
}

EngineMessage game_screen_event_handler(SDL_Event *event, GameScreen *game_screen, GameScreenEvent *game_screen_event) {
	EngineMessage msg = SUCCESS;
	GameMove *move;
	GameMove comp_move;
	ChessBoardEvent chess_board_event;
	ButtonEvent button_event;

	game_screen_event->type = GAME_SCREEN_NO_EVENT;

	msg = chess_board_event_handler(event, game_screen->chess_board, &chess_board_event, game_screen->game);
	if (msg != SUCCESS) {
		return msg;
	}
	if (chess_board_event.type == PIECE_MOVED) {
		msg = move_game_piece(game_screen->game, chess_board_event.data.move.prev_piece_col, chess_board_event.data.move.prev_piece_row, 
		                                         chess_board_event.data.move.new_piece_col, chess_board_event.data.move.new_piece_row);
		if (msg != SUCCESS && msg != ILLEGAL_MOVE) {
			return msg;
		}
	}

	msg = button_event_handler(event, game_screen->restart_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type ==  BUTTON_PUSHED && game_screen->game) {
		msg = restart_game(game_screen->game);
		if (msg != SUCCESS) {
			return msg;
		}
	}

	msg = button_event_handler(event, game_screen->save_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		msg = new_saved_game(game_screen->game);
		if (msg != SUCCESS && msg != SUCCESS_NO_PRINT) {
			return msg;
		}
	}

	msg = button_event_handler(event, game_screen->load_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		game_screen_event->type = GAME_SCREEN_MOVE_TO_LOAD_SCREEN;
	}

	msg = button_event_handler(event, game_screen->new_game_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		game_screen_event->type = GAME_SCREEN_MOVE_TO_SETTINGS_SCREEN;
	}

	msg = button_event_handler(event, game_screen->main_menu_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		game_screen_event->type = GAME_SCREEN_MOVE_TO_MAIN_MENU;
	}

	msg = button_event_handler(event, game_screen->quit_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		game_screen_event->type = GAME_SCREEN_QUIT;
	}

	if (game_screen->game->mode == ONE_PLAYER && spArrayListSize(game_screen->game->move_history) >= 2) {
		msg = button_event_handler(event, game_screen->undo_button, &button_event);
		if (msg != SUCCESS) {
			return msg;
		}
		if (button_event.type == BUTTON_PUSHED && game_screen->game) {
			msg = undo_move(game_screen->game, &move);
			if (msg != SUCCESS) {
				return msg;
			}
			free(move);

			msg = undo_move(game_screen->game, &move);
			if (msg != SUCCESS) {
				return msg;
			}
			free(move);
		}
	}

	if (game_screen->game && game_screen->game->mode == ONE_PLAYER && game_screen->game->current_player == PLAYER2) {
		msg = minimax_suggest_move(game_screen->game, game_screen->game->difficulty, &comp_move);
		if (msg != SUCCESS) {
			return msg;
		}
		msg = move_game_piece(game_screen->game, comp_move.src_x, comp_move.src_y, comp_move.dst_x, comp_move.dst_y);
		if (msg != SUCCESS) {
			return msg;
		}
	}

	return msg;
}
