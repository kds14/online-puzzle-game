#include "video.hpp"

Video video;

bool Video::init(int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL failed to initialize: %s\n", SDL_GetError());
		return false;
	}
	SDL_CreateWindowAndRenderer(width, height, 0, &this->window, &this->renderer);
	if (!this->window || !this->renderer) {
		fprintf(stderr, "SDL failed to create window/renderer: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetWindowTitle(this->window, "trl");
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	return true;
}

void Video::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			exit(0);
		}
	}
}
