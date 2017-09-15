#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SDL.h>
#include <SDL_video.h>

#include "../game_engine.h"
#include "chess_board.h"
#include "button.h"

typedef struct {
	Game *game;
	ChessBoard *chess_board;
	Button *restart_button;
	Button *save_button;
	Button *load_button;
	Button *undo_button;
	Button *new_game_button;
	Button *main_menu_button;
	Button *quit_button;
} GameScreen;

EngineMessage create_game_screen(GameScreen **game_screen, SDL_Renderer *renderer);

void destroy_game_screen(GameScreen *game_screen);

EngineMessage start_new_game(GameSettings *settings, GameScreen *game_screen);

EngineMessage draw_game_screen(SDL_Renderer *renderer, GameScreen *game_screen);

EngineMessage game_screen_event_handler(SDL_Event *event, GameScreen *game_screen, GameEvent *game_event);

#endif
