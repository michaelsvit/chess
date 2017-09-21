#include "button.h"
#include "utils.h"

EngineMessage create_button(Button **button, SDL_Rect *area, SDL_Renderer *renderer, char *reg_path, char *pushed_path) {
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
	new_button->area = *area;
	new_button->is_pushed = 0;

	*button = new_button;

	return SUCCESS;
}


EngineMessage draw_button(SDL_Renderer *renderer, Button *button) {
	int err;
	if (button->is_pushed == 0) {
		err = SDL_RenderCopy(renderer, button->regular_texture, NULL, &(button->area));
	} else {
		err = SDL_RenderCopy(renderer, button->pushed_texture, NULL, &(button->area));
	}

	if (err != 0) {
		return SDL_ERROR;
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

EngineMessage button_event_handler(SDL_Event *event, Button *button, ButtonEvent *button_event) {
	button_event->type = NO_BUTTON_EVENT;

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
				button_event->type = BUTTON_PUSHED;
			}
			break;
	}
	return SUCCESS;
}
