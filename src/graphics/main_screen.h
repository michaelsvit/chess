#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <SDL.h>
#include <SDL_video.h>

#include "../game_engine.h"
#include "button.h"

typedef enum {
	MAIN_SCREEN_NO_EVENT = 0,
	MAIN_SCREEN_MOVE_TO_LOAD_SCREEN,
	MAIN_SCREEN_MOVE_TO_SETTINGS_SCREEN,
	MAIN_SCREEN_QUIT
} MainScreenEventType;

typedef struct {
    MainScreenEventType type;
} MainScreenEvent;

typedef struct {
	Button *load_button;
	Button *new_game_button;
	Button *quit_button;
} MainScreen;

EngineMessage create_main_screen(MainScreen **main_screen, SDL_Renderer *renderer);

void destroy_main_screen(MainScreen *main_screen);

EngineMessage draw_main_screen(SDL_Renderer *renderer, MainScreen *main_screen);

EngineMessage main_screen_event_handler(SDL_Event *event, MainScreen *main_screen, MainScreenEvent *main_screen_event);


#endif
