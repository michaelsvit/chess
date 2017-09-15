#include "utils.h"

SDL_Texture* create_texture(SDL_Renderer *renderer, char *file_name) {
	SDL_Texture* res = NULL;
	SDL_Surface* loading_surface = SDL_LoadBMP(file_name);
	if (loading_surface == NULL) {
		return NULL;
	}
	SDL_SetColorKey(loading_surface, SDL_TRUE, SDL_MapRGB(loading_surface->format, 0, 187, 0));
	res = SDL_CreateTextureFromSurface(renderer, loading_surface);
	if (res == NULL ) {
		printf("Could not create a texture: %s\n", SDL_GetError());
		SDL_DestroyTexture(res);
		return NULL ;
	}
	SDL_FreeSurface(loading_surface);
	return res;
}
