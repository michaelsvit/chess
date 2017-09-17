#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include <SDL.h>
#include <SDL_video.h>
#include "button.h"
#include "multiple_choice.h"

typedef enum {
	BACK_TO_MAIN_SCREEN = 0,
	BACK_TO_GAME_SCREEN
} BackTo;

typedef enum {
	LOAD_SCREEN_NO_EVENT = 0,
	LOAD_SCREEN_LOAD_GAME,
	GAME_SCREEN_BACK
} LoadScreenEventType;

typedef struct {
    LoadScreenEventType type;
} LoadScreenEvent;

typedef struct {
	MultipleChoice *saved_games;
	Button *load_button;
	Button *back_button;
	BackTo back_to;
} LoadScreen;

EngineMessage create_load_screen(LoadScreen **load_screen, SDL_Renderer *renderer);

EngineMessage draw_load_screen(SDL_Renderer *renderer, LoadScreen *load_screen);

void destroy_load_screen(LoadScreen *load_screen);

EngineMessage load_screen_event_handler(SDL_Event *event, LoadScreen *load_screen, LoadScreenEvent *load_screen_event);


#endif
