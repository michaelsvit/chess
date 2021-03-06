#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <SDL_video.h>

#include "game_screen.h"
#include "settings_screen.h"
#include "main_screen.h"
#include "load_screen.h"

typedef enum {
    NO_WINDOW_EVENT = 0,
    QUIT_WINDOW
} WindowEventType;

typedef struct {
    WindowEventType type;
} WindowEvent;

typedef enum {
	SETTINGS_SCREEN,
	GAME_SCREEN,
	MAIN_SCREEN,
	LOAD_SCREEN
} ScreenType;

typedef struct {
	SDL_Window *sdl_window;
	SDL_Renderer *renderer;
	ScreenType screen;
	ScreenType back_screen;
	GameScreen *game_screen;
	SettingsScreen *settings_screen;
	MainScreen *main_screen;
	LoadScreen *load_screen;
} Window;

EngineMessage create_window(Window **window);

void destroy_window(Window *window);

EngineMessage draw_window(Window *window);

EngineMessage window_event_handler(SDL_Event *event, Window *window, WindowEvent *window_event);

#endif
