#include "button.h"
#include "utils.h"

EngineMessage create_button(Button **button, SDL_Rect *area, SDL_Renderer *renderer, int button_id, char *reg_path, char *pushed_path) {
	Button *new_button = (Button*)calloc(1, sizeof(Button));
	if (!new_button) {
		return MALLOC_FAILURE;
	}
	new_button->regular_texture = create_texture(renderer, reg_path);
	if (!new_button->regular_texture) {
		destroy_button(new_button);
		return SDL_ERROR;
	}
	new_button->pushed_texture = create_texture(renderer, pushed_path);
	if (!new_button->pushed_texture) {
		destroy_button(new_button);
		return SDL_ERROR;
	}
	new_button->button_id = button_id;
	new_button->area = *area;
	new_button->is_pushed = 0;

	*button = new_button;

	return SUCCESS;
}


EngineMessage draw_button(SDL_Renderer *renderer, Button *button) {
	if (button->is_pushed == 0) {
		SDL_RenderCopy(renderer, button->regular_texture, NULL, &(button->area));
	} else {
		SDL_RenderCopy(renderer, button->pushed_texture, NULL, &(button->area));
	}

	return SUCCESS;
}

void destroy_button(Button *button) {
	if (button->regular_texture) {
		SDL_DestroyTexture(button->regular_texture);
	}
	if (button->pushed_texture) {
		SDL_DestroyTexture(button->pushed_texture);
	}
	free(button);
}

EngineMessage button_event_handler(SDL_Event *event, Button *button, GameEvent *game_event) {
	game_event->type = NO_EVENT;
	switch (event->type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.x >= button->area.x && event->button.x <= button->area.x + button->area.w
				&& event->button.y >= button->area.y && event->button.y <= button->area.y + button->area.h) {
				button->is_pushed = 1;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (button->is_pushed == 1) {
				button->is_pushed = 0;
				game_event->type = BUTTON_CLICKED;
				game_event->data.button.button_id = button->button_id;
			}
			break;
	}
	return SUCCESS;
}
