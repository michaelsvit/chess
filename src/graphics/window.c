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

	new_window->screen = MAIN_SCREEN;
	new_window->back_screen = MAIN_SCREEN;

	msg = create_main_screen(&new_window->main_screen, new_window->renderer);
	if(msg != SUCCESS) {
		destroy_window(new_window);
		return msg;
	}

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

	msg = create_load_screen(&new_window->load_screen, new_window->renderer);
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
	if (window->main_screen) {
		destroy_main_screen(window->main_screen);
	}
	if (window->load_screen) {
		destroy_load_screen(window->load_screen);
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
	EngineMessage err;
	err = SDL_RenderClear(window->renderer);
	if (err != 0) {
		return SDL_ERROR;
	}

	switch (window->screen) {
		case SETTINGS_SCREEN:
			err = draw_settings_screen(window->renderer, window->settings_screen);
			break;
		case GAME_SCREEN:
			err = draw_game_screen(window->renderer, window->game_screen);
			break;
		case MAIN_SCREEN:
			err = draw_main_screen(window->renderer, window->main_screen);
			break;
		case LOAD_SCREEN:
			err = draw_load_screen(window->renderer, window->load_screen);
			break;
	}
	if (err != SUCCESS) {
		return err;
	}

	SDL_RenderPresent(window->renderer);
	return SUCCESS;
}

EngineMessage window_event_handler(SDL_Event *event, Window *window, WindowEvent *window_event) {
	EngineMessage err = SUCCESS;
	GameScreenEvent game_screen_event;
	SettingsScreenEvent settings_screen_event;
	MainScreenEvent main_screen_event;
	LoadScreenEvent load_screen_event;

	window_event->type = NO_WINDOW_EVENT;

	if (event->type == SDL_QUIT) {
		window_event->type = QUIT_WINDOW;
		return err;
	}

	switch (window->screen) {
		case MAIN_SCREEN:
			err = main_screen_event_handler(event, window->main_screen, &main_screen_event);
			if (err != SUCCESS) {
				return err;
			}
			if (main_screen_event.type == MAIN_SCREEN_MOVE_TO_SETTINGS_SCREEN) {
				reset_settings_screen(window->settings_screen);
				window->back_screen = MAIN_SCREEN;
				window->screen = SETTINGS_SCREEN;
			} else if (main_screen_event.type == MAIN_SCREEN_MOVE_TO_LOAD_SCREEN) {
				reset_load_screen(window->load_screen);
				window->back_screen = MAIN_SCREEN;
				window->screen = LOAD_SCREEN;
			} else if (main_screen_event.type == MAIN_SCREEN_QUIT) {
				window_event->type = QUIT_WINDOW;
			}
			break;
		case SETTINGS_SCREEN:
			err = settings_screen_event_handler(event, window->settings_screen, &settings_screen_event);
			if (err != SUCCESS) {
				return err;
			}
			if (settings_screen_event.type == SETTINGS_SCREEN_NEW_GAME) {
				window->screen = GAME_SCREEN;
				err = start_new_game(&settings_screen_event.data.settings, window->game_screen);
				if (err != SUCCESS) {
					return err;
				}
			} else if (settings_screen_event.type == SETTINGS_SCREEN_EXIT) {
				window->screen = window->back_screen;
			}
			break;
		case GAME_SCREEN:
			err = game_screen_event_handler(event, window->game_screen, &game_screen_event);
			if (err != SUCCESS) {
				return err;
			}
			if (game_screen_event.type == GAME_SCREEN_MOVE_TO_SETTINGS_SCREEN) {
				reset_settings_screen(window->settings_screen);
				window->back_screen = GAME_SCREEN;
				window->screen = SETTINGS_SCREEN;
			} else if (game_screen_event.type == GAME_SCREEN_MOVE_TO_MAIN_MENU) {
				window->screen = MAIN_SCREEN;
			} else if (game_screen_event.type == GAME_SCREEN_MOVE_TO_LOAD_SCREEN) {
				reset_load_screen(window->load_screen);
				window->back_screen = GAME_SCREEN;
				window->screen = LOAD_SCREEN;
			} else if (game_screen_event.type == GAME_SCREEN_QUIT) {
				window_event->type = QUIT_WINDOW;
			}
			break;
		case LOAD_SCREEN:
			err = load_screen_event_handler(event, window->load_screen, &load_screen_event);
			if (err != SUCCESS) {
				return err;
			}
			if (load_screen_event.type == LOAD_SCREEN_LOAD_GAME) {
				err = start_load_game(window->load_screen->saved_games->choice, window->game_screen);
				if (err != SUCCESS) {
					return err;
				}
				window->screen = GAME_SCREEN;
			} else if (load_screen_event.type == LOAD_SCREEN_BACK) {
				window->screen = window->back_screen;
			}
			break;
	}

	return SUCCESS;
}
