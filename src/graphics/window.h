#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <SDL_video.h>

#include "game_screen.h"
#include "settings_screen.h"

typedef enum {
	SETTINGS_SCREEN,
	GAME_SCREEN,
	/*MAIN_MENU,
	LOAD_SCREEN,*/
} ScreenType;

typedef struct {
	SDL_Window *sdl_window;
	SDL_Renderer *renderer;
	ScreenType screen;
	GameScreen *game_screen;
	SettingsScreen *settings_screen; 
} Window;

EngineMessage create_window(Window **window);

void destroy_window(Window *window);

EngineMessage draw_window(Window *window);

void window_event_handler(SDL_Event *event, Window *window, GameEvent *game_event);

#endif
