#include "window.h"


EngineMessage create_window(Window **window) {
	EngineMessage msg = SUCCESS;
	Window *new_window = (Window*)calloc(1, sizeof(Window));
	if (!new_window) {
		return MALLOC_FAILURE;
	}
	
	new_window->sdl_window = SDL_CreateWindow("Chess",	// window title
			SDL_WINDOWPOS_CENTERED,			// initial x position
			SDL_WINDOWPOS_CENTERED,			// initial y position
			1000,							// width, in pixels
			800,							// height, in pixels
			SDL_WINDOW_OPENGL				// flags - see below
	);
	if (!new_window->sdl_window) {
		destroy_window(new_window);
		return SDL_ERROR;
	}

	new_window->renderer = SDL_CreateRenderer(new_window->sdl_window, -1, SDL_RENDERER_ACCELERATED);
	if (!new_window->renderer) {
		destroy_window(new_window);
		return SDL_ERROR;
	}

	new_window->screen = SETTINGS_SCREEN;

	msg = create_settings_screen(&new_window->settings_screen, new_window->renderer);
	if (msg != SUCCESS) {
		destroy_window(new_window);
		return msg;
	}

	msg = create_game_screen(&new_window->game_screen, new_window->renderer);
	if(msg != SUCCESS) {
		destroy_window(new_window);
		return msg;
	}

	*window = new_window;
	return SUCCESS;
}

void destroy_window(Window *window) {
	if (window->game_screen) {
		destroy_game_screen(window->game_screen);
	}
	if (window->settings_screen) {
		destroy_settings_screen(window->settings_screen);
	}
	if (window->renderer) {
		SDL_DestroyRenderer(window->renderer);
	}
	if (window->sdl_window) {
		SDL_DestroyWindow(window->sdl_window);
	}
	free(window);
}

EngineMessage draw_window(Window *window) {
	SDL_RenderClear(window->renderer);

	switch (window->screen) {
		case SETTINGS_SCREEN:
			draw_settings_screen(window->renderer, window->settings_screen);
			break;
		case GAME_SCREEN:
			draw_game_screen(window->renderer, window->game_screen);
			break;
	}

	SDL_RenderPresent(window->renderer);
	return SUCCESS;
}

void window_event_handler(SDL_Event *event, Window *window, GameEvent *game_event) {
	GameEvent screen_event;

	if (event->type == SDL_QUIT) {
		game_event->type = QUIT_GAME;
		return;
	}

	switch (window->screen) {
		case SETTINGS_SCREEN:
			settings_screen_event_handler(event, window->settings_screen, &screen_event);
			if (screen_event.type == NEW_GAME) {
				window->screen = GAME_SCREEN;
				start_new_game(&screen_event.data.new_game_settings, window->game_screen);
			} else if (screen_event.type == EXIT_SETTINGS_SCREEN) {
				window->screen = GAME_SCREEN; //to change to main menu
			}
			break;
		case GAME_SCREEN:
			game_screen_event_handler(event, window->game_screen, &screen_event);
			if (screen_event.type == MOVE_TO_SETTINGS_SCREEN) {
				reset_settings_screen(window->settings_screen);
				window->screen = SETTINGS_SCREEN;
			} else if (screen_event.type == QUIT_GAME) {
				game_event->type = QUIT_GAME;
			}
			break;
	}
}
