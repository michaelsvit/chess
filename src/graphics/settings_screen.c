#include "settings_screen.h"

EngineMessage create_settings_screen(SettingsScreen **settings_screen, SDL_Renderer *renderer) {
	EngineMessage ret = SUCCESS;
	SettingsScreen *new_settings_screen = (SettingsScreen*)calloc(1, sizeof(SettingsScreen));
	if (!new_settings_screen) {
		return MALLOC_FAILURE;
	}

	new_settings_screen->stage = MODE_STAGE;

	ChoiceButtonArguments mode_choices[] = {
		{.regular_path = "./images/1_player.bmp", .pushed_path = "./images/1_player_pushed.bmp", .area={.x = 200, .y = 100, .w = 200, .h = 100}},
		{.regular_path = "./images/2_players.bmp", .pushed_path = "./images/2_players_pushed.bmp", .area={.x = 400, .y = 100, .w = 200, .h = 100}}
	};
	ret = create_multiple_choice(&new_settings_screen->mode, renderer, mode_choices, 2);
	if (ret != SUCCESS) {
		destroy_settings_screen(new_settings_screen);
		return ret;
	}

	ChoiceButtonArguments difficulty_choices[] = {
		{.regular_path = "./images/noob.bmp", .pushed_path = "./images/noob_pushed.bmp", .area={.x = 100, .y = 80, .w = 200, .h = 100}},
		{.regular_path = "./images/easy.bmp", .pushed_path = "./images/easy_pushed.bmp", .area={.x = 100, .y = 260, .w = 200, .h = 100}},
		{.regular_path = "./images/moderate.bmp", .pushed_path = "./images/moderate_pushed.bmp", .area={.x = 100, .y = 440, .w = 200, .h = 100}},
		{.regular_path = "./images/hard.bmp", .pushed_path = "./images/hard_pushed.bmp", .area={.x = 100, .y = 620, .w = 200, .h = 100}}
	};
	ret = create_multiple_choice(&new_settings_screen->difficulty, renderer, difficulty_choices, 4);
	if (ret != SUCCESS) {
		destroy_settings_screen(new_settings_screen);
		return ret;
	}

	ChoiceButtonArguments color_choices[] = {
		{.regular_path = "./images/white.bmp", .pushed_path = "./images/white_pushed.bmp", .area={.x = 200, .y = 300, .w = 150, .h = 100}},
		{.regular_path = "./images/black.bmp", .pushed_path = "./images/black_pushed.bmp", .area={.x = 400, .y = 300, .w = 150, .h = 100}}
	};
	ret = create_multiple_choice(&new_settings_screen->color, renderer, color_choices, 2);
	if (ret != SUCCESS) {
		destroy_settings_screen(new_settings_screen);
		return ret;
	}

	SDL_Rect next_area = {.x = 600, .y = 100, .w = 200, .h = 100};
	ret = create_button(&new_settings_screen->next_button, &next_area, renderer, "./images/next.bmp", "./images/next_pushed.bmp");
	if (ret != SUCCESS) {
		destroy_settings_screen(new_settings_screen);
		return ret;
	}

	SDL_Rect start_area = {.x = 600, .y = 100, .w = 200, .h = 100};
	ret = create_button(&new_settings_screen->start_button, &start_area, renderer, "./images/start.bmp", "./images/start_pushed.bmp");
	if (ret != SUCCESS) {
		destroy_settings_screen(new_settings_screen);
		return ret;
	}

	SDL_Rect back_area = {.x = 600, .y = 500, .w = 200, .h = 100};
	ret = create_button(&new_settings_screen->back_button, &back_area, renderer, "./images/back.bmp", "./images/back_pushed.bmp");
	if (ret != SUCCESS) {
		destroy_settings_screen(new_settings_screen);
		return ret;
	}

	*settings_screen = new_settings_screen;
	return SUCCESS;
}

void destroy_settings_screen(SettingsScreen *settings_screen) {
	if (settings_screen->mode) {
		destroy_multiple_choice(settings_screen->mode);
	}
	if (settings_screen->difficulty) {
		destroy_multiple_choice(settings_screen->difficulty);
	}
	if (settings_screen->color) {
		destroy_multiple_choice(settings_screen->color);
	}
	if (settings_screen->next_button) {
		destroy_button(settings_screen->next_button);
	}
	if (settings_screen->back_button) {
		destroy_button(settings_screen->back_button);
	}
	if (settings_screen->start_button) {
		destroy_button(settings_screen->start_button);
	}

	free(settings_screen);
}

int is_next_button_visible(SettingsScreen *settings_screen) {
	if (settings_screen->stage == MODE_STAGE && settings_screen->mode->choice == 0) {
		return 1;
	}
	if (settings_screen->stage == DIFFICULTY_STAGE && settings_screen->difficulty->choice != -1) {
		return 1;
	}
	return 0;
}

int is_start_button_visible(SettingsScreen *settings_screen) {
	if (settings_screen->stage == MODE_STAGE && settings_screen->mode->choice == 1) {
		return 1;
	}
	if (settings_screen->stage == COLOR_STAGE && settings_screen->color->choice != -1) {
		return 1;
	}
	return 0;
}

EngineMessage draw_settings_screen(SDL_Renderer *renderer, SettingsScreen *settings_screen) {
	switch (settings_screen->stage) {
		case MODE_STAGE:
			draw_multiple_choice(renderer, settings_screen->mode);
			break;
		case DIFFICULTY_STAGE:
			draw_multiple_choice(renderer, settings_screen->difficulty);
			break;
		case COLOR_STAGE:
			draw_multiple_choice(renderer, settings_screen->color);
			break;
	}

	draw_button(renderer, settings_screen->back_button);

	if (is_next_button_visible(settings_screen)) {
		draw_button(renderer, settings_screen->next_button);
	}
	if (is_start_button_visible(settings_screen)) {
		draw_button(renderer, settings_screen->start_button);
	}

	return SUCCESS;
}

EngineMessage settings_screen_event_handler(SDL_Event *event, SettingsScreen *settings_screen, SettingsScreenEvent *settings_screen_event) {
	ButtonEvent button_event;

	settings_screen_event->type = SETTINGS_SCREEN_NO_EVENT;

	switch (settings_screen->stage) {
		case MODE_STAGE:
			multiple_choice_event_handler(event, settings_screen->mode);
			break;
		case DIFFICULTY_STAGE:
			multiple_choice_event_handler(event, settings_screen->difficulty);
			break;
		case COLOR_STAGE:
			multiple_choice_event_handler(event, settings_screen->color);
			break;
	}

	button_event_handler(event, settings_screen->next_button, &button_event);
	if (is_next_button_visible(settings_screen) && button_event.type == BUTTON_PUSHED) {
		if (settings_screen->stage == MODE_STAGE) {
			settings_screen->stage = DIFFICULTY_STAGE;
		} else if (settings_screen->stage == DIFFICULTY_STAGE) {
			settings_screen->stage = COLOR_STAGE;
		}
	}

	button_event_handler(event, settings_screen->start_button, &button_event);
	if (is_start_button_visible(settings_screen) && button_event.type == BUTTON_PUSHED) {
		settings_screen_event->type = SETTINGS_SCREEN_NEW_GAME;
		if (settings_screen->mode->choice == 0) {
			settings_screen_event->data.settings.mode = ONE_PLAYER;
			settings_screen_event->data.settings.difficulty = 1 + settings_screen->difficulty->choice;
			settings_screen_event->data.settings.player1_color = (settings_screen->color->choice == 0) ? WHITE : BLACK;
		} else {
			settings_screen_event->data.settings.mode = TWO_PLAYER;
			settings_screen_event->data.settings.difficulty = 1;
			settings_screen_event->data.settings.player1_color = WHITE;
		}
	}

	button_event_handler(event, settings_screen->back_button, &button_event);
	if (button_event.type == BUTTON_PUSHED) {
		if (settings_screen->stage == COLOR_STAGE) {
			settings_screen->stage = DIFFICULTY_STAGE;
		} else if (settings_screen->stage == DIFFICULTY_STAGE) {
			settings_screen->stage = MODE_STAGE;
		} else if (settings_screen->stage == MODE_STAGE) {
			reset_settings_screen(settings_screen);
			settings_screen_event->type = SETTINGS_SCREEN_EXIT;
		}
	}
	return SUCCESS;
}

void reset_settings_screen(SettingsScreen *settings_screen) {
	settings_screen->stage = MODE_STAGE;
	settings_screen->mode->choice = -1;
	settings_screen->difficulty->choice = -1;
	settings_screen->color->choice = -1;
}
