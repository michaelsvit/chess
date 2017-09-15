#include "multiple_choice.h"
#include "utils.h"

EngineMessage create_multiple_choice(MultipleChoice **multiple_choice, SDL_Renderer *renderer, ChoiceButtonArguments choices[], int num_choices) {
	MultipleChoice *new_multiple_choice;
	int i;

	new_multiple_choice = (MultipleChoice*)calloc(1, sizeof(MultipleChoice));
	if (!new_multiple_choice) {
		return MALLOC_FAILURE;
	}

	new_multiple_choice->choice = -1;
	new_multiple_choice->num_choices = num_choices;

	for (i = 0; i < num_choices; i++) {
		ChoiceButton *button = &new_multiple_choice->choices[i];

		button->regular_texture = create_texture(renderer, choices[i].regular_path);
		if (!button->regular_texture) {
			destroy_multiple_choice(new_multiple_choice);
			return SDL_ERROR;
		}

		button->pushed_texture = create_texture(renderer, choices[i].pushed_path);
		if (!button->pushed_texture) {
			destroy_multiple_choice(new_multiple_choice);
			return SDL_ERROR;
		}

		button->area = choices[i].area;
	}
	*multiple_choice = new_multiple_choice;
	return SUCCESS;
}

EngineMessage draw_multiple_choice(SDL_Renderer *renderer, MultipleChoice *multiple_choice) {
	int i;
	for (i = 0; i < multiple_choice->num_choices; i++) {
		if (multiple_choice->choice == i) {
			SDL_RenderCopy(renderer, multiple_choice->choices[i].pushed_texture, NULL, &(multiple_choice->choices[i].area));
		} else {
			SDL_RenderCopy(renderer, multiple_choice->choices[i].regular_texture, NULL, &(multiple_choice->choices[i].area));
		}
	}
	return SUCCESS;
}

void destroy_multiple_choice(MultipleChoice *multiple_choice) {
	int i;
	for (i = 0; i < multiple_choice->num_choices; i++) {
		if (multiple_choice->choices[i].regular_texture) {
			SDL_DestroyTexture(multiple_choice->choices[i].regular_texture);
		}
		if (multiple_choice->choices[i].pushed_texture) {
			SDL_DestroyTexture(multiple_choice->choices[i].pushed_texture);
		}
	}

	free(multiple_choice);
}

void multiple_choice_event_handler(SDL_Event *event, MultipleChoice *multiple_choice) {
	int i;

	switch (event->type) {
		case SDL_MOUSEBUTTONDOWN:
			for (i = 0; i < multiple_choice->num_choices; i++) {
				if (event->button.x >= multiple_choice->choices[i].area.x && event->button.x <= multiple_choice->choices[i].area.x + multiple_choice->choices[i].area.w
				&& event->button.y >= multiple_choice->choices[i].area.y && event->button.y <= multiple_choice->choices[i].area.y +multiple_choice->choices[i].area.h) {
					multiple_choice->choice = i;
				}
			}
			break;
	}
}
