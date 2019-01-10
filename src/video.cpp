#include "video.hpp"

Video video;

void Video::drawO(int x, int y) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = 64;
	rect.h = 64;
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
}

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

void Video::drawObjs(GameObjsPtr objects) {
	for (auto &obj : *objects) {
		drawO(obj->x, obj->y);
	}
}

void Video::update(uint32_t time, GameObjsPtr objects) {
	// handle events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			exit(0);
		}
	}

	// draw and clear renderer
	drawObjs(objects);
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// wait
	double trueWaitTime = Video::msPerFrame - SDL_GetTicks() + frameTime + remainder;
	int waitTime = trueWaitTime;
	if (waitTime > 0) {
		remainder = trueWaitTime - waitTime;
		SDL_Delay(waitTime);
	}
	frameTime = SDL_GetTicks();
}

const double Video::msPerFrame = 16.667L;
