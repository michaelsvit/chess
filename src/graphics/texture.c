#include "texture.h"
#include "utils.h"

EngineMessage create_inactive_texture(Texture **texture, SDL_Rect *area, SDL_Renderer *renderer, char *path) {
	Texture *new_texture = (Texture*)calloc(1, sizeof(Texture));
	if (!new_texture) {
		return MALLOC_FAILURE;
	}
	new_texture->texture = create_texture(renderer, path);
	if (!new_texture->texture) {
		destroy_inactive_texture(new_texture);
		return SDL_ERROR;
	}
	new_texture->area = *area;
	*texture = new_texture;

	return SUCCESS;
}

EngineMessage draw_inactive_texture(SDL_Renderer *renderer, Texture *texture) {
	int err;
	err = SDL_RenderCopy(renderer, texture->texture, NULL, &(texture->area));
	if (err != 0) {
		return SDL_ERROR;
	}
	return SUCCESS;
}

void destroy_inactive_texture(Texture *texture) {
	if (texture->texture) {
		SDL_DestroyTexture(texture->texture);
	}
	free(texture);
}
