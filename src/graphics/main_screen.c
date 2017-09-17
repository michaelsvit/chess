#include "main_screen.h"

EngineMessage create_main_screen(MainScreen **main_screen, SDL_Renderer *renderer) {
	EngineMessage msg;

	MainScreen *new_main_screen = (MainScreen*)calloc(1,sizeof(MainScreen));
	if (!new_main_screen) {
		return MALLOC_FAILURE;
	}
	SDL_Rect load_area = {.x = 350, .y = 300, .w = 200, .h = 100};
	msg = create_button(&new_main_screen->load_button, &load_area, renderer, "./images/load.bmp", "./images/load_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_main_screen(new_main_screen);
		return msg;
	}

	SDL_Rect new_game_area = {.x = 350, .y = 500, .w = 200, .h = 100};
	msg = create_button(&new_main_screen->new_game_button, &new_game_area, renderer, "./images/new_game.bmp", "./images/new_game_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_main_screen(new_main_screen);
		return msg;
	}

	SDL_Rect quit_area = {.x = 350, .y = 700, .w = 200, .h = 100};
	msg = create_button(&new_main_screen->quit_button, &quit_area, renderer, "./images/quit.bmp", "./images/quit_pushed.bmp");
	if (msg != SUCCESS) {
		destroy_main_screen(new_main_screen);
		return msg;
	}

	*main_screen = new_main_screen;
	return SUCCESS;
}

void destroy_main_screen(MainScreen *main_screen) {
	if (main_screen->load_button) {
		destroy_button(main_screen->load_button);
	}
	if (main_screen->new_game_button) {
		destroy_button(main_screen->new_game_button);
	}
	if (main_screen->quit_button) {
		destroy_button(main_screen->quit_button);
	}
	free(main_screen);
}

EngineMessage draw_main_screen(SDL_Renderer *renderer, MainScreen *main_screen) {
	EngineMessage err;
	err = draw_button(renderer, main_screen->load_button);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, main_screen->new_game_button);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, main_screen->quit_button);
	if (err != SUCCESS) {
		return err;
	}
	return SUCCESS;
}

EngineMessage main_screen_event_handler(SDL_Event *event, MainScreen *main_screen, MainScreenEvent *main_screen_event) {
	ButtonEvent button_event;
	EngineMessage msg = SUCCESS;

	main_screen_event->type = MAIN_SCREEN_NO_EVENT;

	msg = button_event_handler(event, main_screen->load_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		main_screen_event->type = MAIN_SCREEN_MOVE_TO_LOAD_SCREEN;
	}

	msg = button_event_handler(event, main_screen->new_game_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		main_screen_event->type = MAIN_SCREEN_MOVE_TO_SETTINGS_SCREEN;
	}

	msg = button_event_handler(event, main_screen->quit_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		main_screen_event->type = MAIN_SCREEN_QUIT;
	}
	return msg;
}
