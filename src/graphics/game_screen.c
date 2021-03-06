#include "game_screen.h"
#include "../minimax.h"

EngineMessage create_game_screen(GameScreen **game_screen, SDL_Renderer *renderer) {
	EngineMessage msg;

	GameScreen *new_game_screen = (GameScreen*)calloc(1, sizeof(GameScreen));
	if (!new_game_screen) {
		return MALLOC_FAILURE;
	}

	SDL_Rect board_area = {.x = 200, .y = 64, .w = 768, .h = 768};
	msg = create_chess_board(&new_game_screen->chess_board, &board_area, renderer);
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect new_game_area = {.x = 25, .y = 64, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->new_game_button, &new_game_area, renderer, "./images/new_game.bmp", "./images/new_game_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect save_area = {.x = 25, .y = 176, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->save_button, &save_area, renderer, "./images/save.bmp", "./images/save_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}
	msg = create_inactive_texture(&new_game_screen->inactive_save_button, &save_area, renderer, "./images/inactive_save.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect load_area = {.x = 25, .y = 288, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->load_button, &load_area, renderer, "./images/load.bmp", "./images/load_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect undo_area = {.x = 25, .y = 400, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->undo_button, &undo_area, renderer, "./images/undo.bmp", "./images/undo_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}
	msg = create_inactive_texture(&new_game_screen->inactive_undo_button, &undo_area, renderer, "./images/inactive_undo.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect restart_area = {.x = 25, .y = 512, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->restart_button, &restart_area, renderer, "./images/restart.bmp", "./images/restart_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect main_menu_area = {.x = 25, .y = 624, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->main_menu_button, &main_menu_area, renderer, "./images/main_menu.bmp", "./images/main_menu_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect quit_area = {.x = 25, .y = 736, .w = 150, .h = 100};
	msg = create_button(&new_game_screen->quit_button, &quit_area, renderer, "./images/quit.bmp", "./images/quit_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect message_area = {.x = 392, .y = 0, .w = 384, .h = 64};
	msg = create_inactive_texture(&new_game_screen->white_check, &message_area, renderer, "./images/check_white.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}
	msg = create_inactive_texture(&new_game_screen->white_checkmate, &message_area, renderer, "./images/checkmate_white.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}
	msg = create_inactive_texture(&new_game_screen->black_check, &message_area, renderer, "./images/check_black.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}
	msg = create_inactive_texture(&new_game_screen->black_checkmate, &message_area, renderer, "./images/checkmate_black.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	SDL_Rect small_message_area = {.x = 545, .y = 0, .w = 75, .h = 64};
	msg = create_inactive_texture(&new_game_screen->tie, &small_message_area, renderer, "./images/tie.bmp");
	if (msg != SUCCESS) {
		destroy_game_screen(new_game_screen);
		return msg;
	}

	new_game_screen->moves_since_save = 0;
	new_game_screen->is_game_over = 0;

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
	if (game_screen->inactive_undo_button) {
		destroy_inactive_texture(game_screen->inactive_undo_button);
	}
	if (game_screen->inactive_save_button) {
		destroy_inactive_texture(game_screen->inactive_save_button);
	}
	if (game_screen->white_check) {
		destroy_inactive_texture(game_screen->white_check);
	}
	if (game_screen->white_checkmate) {
		destroy_inactive_texture(game_screen->white_checkmate);
	}
	if (game_screen->black_check) {
		destroy_inactive_texture(game_screen->black_check);
	}
	if (game_screen->black_checkmate) {
		destroy_inactive_texture(game_screen->black_checkmate);
	}
	if (game_screen->tie) {
		destroy_inactive_texture(game_screen->tie);
	}
	if (game_screen->game) {
		destroy_game(game_screen->game);
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

	game_screen->moves_since_save = 0;
	game_screen->is_game_over = 0;

	if (game_screen->game->mode == ONE_PLAYER && game_screen->game->current_player == PLAYER2) {
		msg = minimax_suggest_move(game_screen->game, game_screen->game->difficulty, &comp_move);
		if (msg != SUCCESS) {
			return msg;
		}
		msg = move_game_piece(game_screen->game, comp_move.src_x, comp_move.src_y, comp_move.dst_x, comp_move.dst_y);
		if (msg != SUCCESS) {
			return msg;
		}

		game_screen->moves_since_save++;
	}

	return msg;
}

EngineMessage start_load_game(int slot_num, GameScreen *game_screen) {
	game_screen->game = load_saved_game(slot_num);
	if (game_screen->game == NULL) {
		return LOAD_ERROR;
	}
	game_screen->moves_since_save = 0;
	game_screen->is_game_over = is_game_over(game_screen->game);
	return SUCCESS;
}

int should_draw_undo_button(GameScreen* game_screen) {
	if (!game_screen->game) {
		return 0;
	}
	if (game_screen->game->mode != ONE_PLAYER) {
		return 0;
	}

	int undo_history_size = spArrayListSize(game_screen->game->move_history);
	if (undo_history_size == 0) {
		return 0;
	}
	if (undo_history_size == 1 && game_screen->game->current_player == PLAYER1) {
		return 0;
	}

	return 1;
}

EngineMessage draw_message(SDL_Renderer *renderer, GameScreen *game_screen) {
	EngineMessage err = SUCCESS;
	Color current_color = game_screen->game->player_color[game_screen->game->current_player];
	Texture *texture = NULL;

	if (game_screen->is_game_over && game_screen->game->check) {
		texture = (current_color == WHITE) ? game_screen->black_checkmate : game_screen->white_checkmate;
	} else if (game_screen->game->check) {
		texture = (current_color == WHITE) ? game_screen->black_check : game_screen->white_check;
	} else if (game_screen->is_game_over) {
		texture = game_screen->tie;
	}

	if (texture) {
		err = draw_inactive_texture(renderer, texture);
	}

	return err;
}

EngineMessage draw_game_screen(SDL_Renderer *renderer, GameScreen *game_screen) {
	EngineMessage err;

	err = draw_chess_board(renderer, game_screen->chess_board, game_screen->game);
	if (err != SUCCESS) {
		return err;
	}

	err = draw_message(renderer, game_screen);
	if (err != SUCCESS) {
		return err;
	}

	err = draw_button(renderer, game_screen->restart_button);
	if (err != SUCCESS) {
		return err;
	}

	if (game_screen->moves_since_save) {
		err = draw_button(renderer, game_screen->save_button);
	} else {
		err = draw_inactive_texture(renderer, game_screen->inactive_save_button);
	}
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

	if (should_draw_undo_button(game_screen)) {
		err = draw_button(renderer, game_screen->undo_button);
	} else {
		err = draw_inactive_texture(renderer, game_screen->inactive_undo_button);
	}
	if (err != SUCCESS) {
		return err;
	}

	return SUCCESS;
}

EngineMessage save_game_message_box(GameScreen *game_screen, GameScreenEvent *game_screen_event, GameScreenEventType event_type) {
	const SDL_MessageBoxButtonData buttons[] = {
		{ 0, 0, "no" },
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
	};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		"Save Game",
		"Do you want to save game before exiting?",
		SDL_arraysize(buttons),
		buttons,
		NULL
	};
	int buttonid = 0;

	if (game_screen->moves_since_save) {
		if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
			return SDL_ERROR;
		}
	}

	if (buttonid == 1) {
		EngineMessage msg = new_saved_game(game_screen->game);
		if (msg != SUCCESS && msg != SUCCESS_NO_PRINT) {
			return msg;
		}
		game_screen->moves_since_save = 0;
	}

	if (buttonid != -1 && buttonid != 2 && game_screen_event) {
		game_screen_event->type = event_type;
	}

	return SUCCESS;
}


EngineMessage game_screen_event_handler(SDL_Event *event, GameScreen *game_screen, GameScreenEvent *game_screen_event) {
	EngineMessage msg = SUCCESS;
	GameMove *move;
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
		if (msg == SUCCESS) {
			game_screen->moves_since_save++;
		} else if (msg == GAME_OVER) {
			game_screen->is_game_over = 1;
			game_screen->moves_since_save++;
		} else if (msg != ILLEGAL_MOVE) {
			return msg;
		}
	}

	msg = button_event_handler(event, game_screen->restart_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED && game_screen->game) {
		msg = restart_game(game_screen->game);
		if (msg != SUCCESS) {
			return msg;
		}
		game_screen->moves_since_save = 0;
		game_screen->is_game_over = 0;
	}

	if (game_screen->moves_since_save) {
		msg = button_event_handler(event, game_screen->save_button, &button_event);
		if (msg != SUCCESS) {
			return msg;
		}
		if (button_event.type == BUTTON_PUSHED) {
			msg = new_saved_game(game_screen->game);
			if (msg != SUCCESS && msg != SUCCESS_NO_PRINT) {
				return msg;
			}
			game_screen->moves_since_save = 0;
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
		msg = save_game_message_box(game_screen, game_screen_event, GAME_SCREEN_MOVE_TO_MAIN_MENU);
		if (msg != SUCCESS) {
			return msg;
		}
	}

	msg = button_event_handler(event, game_screen->quit_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		save_game_message_box(game_screen, game_screen_event, GAME_SCREEN_QUIT);
		if (msg != SUCCESS) {
			return msg;
		}
	}

	if (should_draw_undo_button(game_screen)) {
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
			game_screen->moves_since_save--;
			game_screen->is_game_over = 0;

			if (game_screen->game->current_player == PLAYER2) {
				msg = undo_move(game_screen->game, &move);
				if (msg != SUCCESS) {
					return msg;
				}
				free(move);
				game_screen->moves_since_save--;
			}

			if (game_screen->moves_since_save < 0) {
				// Saving should be possible now if we continue undoing, and if we do new moves.
				game_screen->moves_since_save = spArrayListSize(game_screen->game->move_history) + 1;
			}
		}
	}

	if (game_screen->game && !game_screen->is_game_over && game_screen->game->mode == ONE_PLAYER && game_screen->game->current_player == PLAYER2) {
		/* Initialize comp_move so we can know if it was assigned a value yet */
		GameMove comp_move = {-1, -1, -1, -1};
		msg = minimax_suggest_move(game_screen->game, game_screen->game->difficulty, &comp_move);
		if (msg != SUCCESS) {
			return msg;
		}
		msg = move_game_piece(game_screen->game, comp_move.src_x, comp_move.src_y, comp_move.dst_x, comp_move.dst_y);
		if (msg == GAME_OVER) {
			game_screen->is_game_over = 1;
		} else if (msg != SUCCESS) {
			return msg;
		}
		game_screen->moves_since_save++;
	}

	return SUCCESS;
}
