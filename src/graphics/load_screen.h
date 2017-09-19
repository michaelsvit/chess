#ifndef LOAD_SCREEN_H
#define LOAD_SCREEN_H

#include <SDL.h>
#include <SDL_video.h>
#include "button.h"
#include "multiple_choice.h"

typedef enum {
	LOAD_SCREEN_NO_EVENT = 0,
	LOAD_SCREEN_LOAD_GAME,
	LOAD_SCREEN_BACK
} LoadScreenEventType;

typedef struct {
	LoadScreenEventType type;
	int slot_number;
} LoadScreenEvent;

typedef struct {
	MultipleChoice *saved_games;
	Button *load_button;
	Button *back_button;
} LoadScreen;

EngineMessage create_load_screen(LoadScreen **load_screen, SDL_Renderer *renderer);

void reset_load_screen(LoadScreen *load_screen);

EngineMessage draw_load_screen(SDL_Renderer *renderer, LoadScreen *load_screen);

void destroy_load_screen(LoadScreen *load_screen);

EngineMessage load_screen_event_handler(SDL_Event *event, LoadScreen *load_screen, LoadScreenEvent *load_screen_event);

#endif
