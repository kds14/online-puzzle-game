#include "platform.hpp"

Platform platform;

void Platform::drawO(int x, int y) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = 64;
	rect.h = 64;
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
}

bool Platform::init(int width, int height) {
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

void Platform::drawObjs(GameObjsPtr objects) {
	for (auto &obj : *objects) {
		drawO(obj->x, obj->y);
	}
}

void Platform::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			exit(0);
		} else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			InputEvent e;
			e.keyUp = event.type == SDL_KEYUP;
			switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					e.key = LEFT;
					break;
				case SDLK_RIGHT:
					e.key = RIGHT;
					break;
				case SDLK_UP:
					e.key = UP;
					break;
				case SDLK_DOWN:
					e.key = DOWN;
					break;
			}
			onInputEvent(e);
		}
	}
}

void Platform::update(uint32_t time, GameObjsPtr objects) {
	handleEvents();

	// draw and clear renderer
	drawObjs(objects);
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// wait
	double trueWaitTime = Platform::msPerFrame - SDL_GetTicks() + frameTime + remainder;
	int waitTime = trueWaitTime;
	if (waitTime > 0) {
		remainder = trueWaitTime - waitTime;
		SDL_Delay(waitTime);
	}
	frameTime = SDL_GetTicks();
}

const double Platform::msPerFrame = 16.667L;
