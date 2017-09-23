#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SDL.h>
#include <SDL_video.h>

#include "../game_engine.h"
#include "chess_board.h"
#include "button.h"
#include "game_slots.h"
#include "texture.h"


typedef enum {
	GAME_SCREEN_NO_EVENT = 0,
	GAME_SCREEN_MOVE_TO_LOAD_SCREEN,
	GAME_SCREEN_MOVE_TO_SETTINGS_SCREEN,
	GAME_SCREEN_MOVE_TO_MAIN_MENU,
	GAME_SCREEN_QUIT
} GameScreenEventType;

typedef struct {
	GameScreenEventType type;
} GameScreenEvent;

typedef struct {
	Game *game;
	ChessBoard *chess_board;
	Button *restart_button;
	Button *save_button;
	Texture *inactive_save_button;
	Button *load_button;
	Button *undo_button;
	Texture *inactive_undo_button;
	Button *new_game_button;
	Button *main_menu_button;
	Button *quit_button;

	Texture *white_check;
	Texture *white_checkmate;
	Texture *black_check;
	Texture *black_checkmate;
	Texture *tie;

	int moves_since_save;
	int is_game_over;
} GameScreen;

EngineMessage create_game_screen(GameScreen **game_screen, SDL_Renderer *renderer);

void destroy_game_screen(GameScreen *game_screen);

EngineMessage start_new_game(GameSettings *settings, GameScreen *game_screen);

EngineMessage draw_game_screen(SDL_Renderer *renderer, GameScreen *game_screen);

EngineMessage game_screen_event_handler(SDL_Event *event, GameScreen *game_screen, GameScreenEvent *game_screen_event);

EngineMessage start_load_game(int slot_num, GameScreen *game_screen);

#endif
