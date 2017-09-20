#include <unistd.h>

#include "load_screen.h"
#include "game_slots.h"


EngineMessage create_load_screen(LoadScreen **load_screen, SDL_Renderer *renderer) {
	EngineMessage ret;
	LoadScreen *new_load_screen = (LoadScreen*)calloc(1, sizeof(LoadScreen));
	if (!new_load_screen) {
		return MALLOC_FAILURE;
	}

	ChoiceButtonArguments saved_games[] = {
		{.regular_path = "./images/slot_1.bmp", .pushed_path = "./images/slot_1_pushed.bmp", .area={.x = 100, .y = 50, .w = 200, .h = 100}},
		{.regular_path = "./images/slot_2.bmp", .pushed_path = "./images/slot_2_pushed.bmp", .area={.x = 100, .y = 200, .w = 200, .h = 100}},
		{.regular_path = "./images/slot_3.bmp", .pushed_path = "./images/slot_3_pushed.bmp", .area={.x = 100, .y = 350, .w = 200, .h = 100}},
		{.regular_path = "./images/slot_4.bmp", .pushed_path = "./images/slot_4_pushed.bmp", .area={.x = 100, .y = 500, .w = 200, .h = 100}},
		{.regular_path = "./images/slot_5.bmp", .pushed_path = "./images/slot_5_pushed.bmp", .area={.x = 100, .y = 650, .w = 200, .h = 100}}
	};
	ret = create_multiple_choice(&new_load_screen->saved_games, renderer, saved_games, MAX_SAVED_GAME);
	if (ret != SUCCESS) {
		destroy_load_screen(new_load_screen);
		return ret;
	}

	SDL_Rect back_area = {.x = 0, .y = 700, .w = 200, .h = 100};
	ret = create_button(&new_load_screen->back_button, &back_area, renderer, "./images/back.bmp", "./images/back_pushed.bmp");
	if (ret != SUCCESS) {
		destroy_load_screen(new_load_screen);
		return ret;
	}
	SDL_Rect load_area = {.x = 600, .y = 700, .w = 200, .h = 100};
	ret = create_button(&new_load_screen->load_button, &load_area, renderer, "./images/load.bmp", "./images/load_pushed.bmp");
	if (ret != SUCCESS) {
		destroy_load_screen(new_load_screen);
		return ret;
	}
	ret = create_inactive_texture(&new_load_screen->inactive_load_button, &load_area, renderer, "./images/inactive_load.bmp");
	if (ret != SUCCESS) {
		destroy_load_screen(new_load_screen);
		return ret;
	}

	*load_screen = new_load_screen;

	return SUCCESS;
}

void reset_load_screen(LoadScreen *load_screen) {
	load_screen->saved_games->choice = -1;
	load_screen->saved_games->num_choices = num_saved_games();
}

EngineMessage draw_load_screen(SDL_Renderer *renderer, LoadScreen *load_screen) {
	EngineMessage err;
	err = draw_multiple_choice(renderer, load_screen->saved_games);
	if (err != SUCCESS) {
		return err;
	}
	err = draw_button(renderer, load_screen->back_button);
	if (err != SUCCESS) {
		return err;
	}
	if (load_screen->saved_games->choice != -1) {
		err = draw_button(renderer, load_screen->load_button);
		if (err != SUCCESS) {
			return err;
		}
	} else {
		err = draw_inactive_texture(renderer, load_screen->inactive_load_button);
		if (err != SUCCESS) {
			return err;
		}
	}
	return SUCCESS;
}

void destroy_load_screen(LoadScreen *load_screen) {
	if (load_screen->saved_games) {
		destroy_multiple_choice(load_screen->saved_games);
	}
	if (load_screen->load_button) {
		destroy_button(load_screen->load_button);
	}
	if (load_screen->back_button) {
		destroy_button(load_screen->back_button);
	}
	if (load_screen->inactive_load_button) {
		destroy_inactive_texture(load_screen->inactive_load_button);
	}
	free(load_screen);
}

EngineMessage load_screen_event_handler(SDL_Event *event, LoadScreen *load_screen, LoadScreenEvent *load_screen_event) {
	EngineMessage msg = SUCCESS;
	ButtonEvent button_event;
	load_screen_event->type = LOAD_SCREEN_NO_EVENT;

	msg = multiple_choice_event_handler(event, load_screen->saved_games);
	if (msg != SUCCESS) {
		return msg;
	}

	msg = button_event_handler(event, load_screen->back_button, &button_event);
	if (msg != SUCCESS) {
		return msg;
	}
	if (button_event.type == BUTTON_PUSHED) {
		load_screen_event->type = LOAD_SCREEN_BACK;
	}

	if (load_screen->saved_games->choice != -1) {
		msg = button_event_handler(event, load_screen->load_button, &button_event);
		if (msg != SUCCESS) {
			return msg;
		}
		if (button_event.type == BUTTON_PUSHED) {
			load_screen_event->slot_number = load_screen->saved_games->choice;
			load_screen_event->type = LOAD_SCREEN_LOAD_GAME;
		}
	}

	return SUCCESS;
}
