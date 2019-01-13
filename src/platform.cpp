#include "platform.hpp"

Platform platform;

void Platform::drawTileMap(std::vector<uint8_t> tileMap, bool p1) {
	// draw blocks
	std::vector<SDL_Rect> rects;
	int count = 0;
	int offset = p1 ? p1Offset : p2Offset;
	for(int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i) {
		if (!tileMap[i])
			continue;
		SDL_Rect rect;
		rect.y = TILE_SIZE * (yOffset + i / MAP_WIDTH);
		rect.x = TILE_SIZE * (offset + i % MAP_WIDTH);
		rect.w = TILE_SIZE;
		rect.h = TILE_SIZE;
		rects.push_back(rect);
		count++;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRects(renderer, &rects[0], count);

	// draw border
	SDL_Rect bRect;
	bRect.y = yOffset * TILE_SIZE;
	bRect.x = offset * TILE_SIZE;
	bRect.w = TILE_SIZE * MAP_WIDTH;
	bRect.h = TILE_SIZE * MAP_HEIGHT;
	SDL_RenderDrawRect(renderer, &bRect);
}

void Platform::drawActive(std::shared_ptr<GamePiece> active, bool p1) {
	std::vector<SDL_Rect> rects;
	int count = 0;
	int offset = p1 ? p1Offset : p2Offset;
	for(std::size_t i = 0; i < active->map.size(); ++i) {
		for(std::size_t j = 0; j < active->map[i].size(); ++j) {
			if (!active->map[i][j])
				continue;
			SDL_Rect rect;
			rect.y = (yOffset + active->y + i) * TILE_SIZE;
			rect.x = (offset + active->x + j) * TILE_SIZE;
			rect.w = TILE_SIZE;
			rect.h = TILE_SIZE;
			rects.push_back(rect);
			count++;
		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRects(renderer, &rects[0], count);
}

bool Platform::init(int width, int height) {
	// change layout if 2p
	if (game_flags & TWOP_FLAG) {
		width = width * 3 + 2;
		p2Offset = width - 11;
	} else {
		width += 2;
		p2Offset = 0;
	}
	p1Offset = 1;
	height = height + 4;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL failed to initialize: %s\n", SDL_GetError());
		return false;
	}
	SDL_CreateWindowAndRenderer(width * TILE_SIZE, height * TILE_SIZE, 0, &this->window, &this->renderer);
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

void Platform::drawObjs(GameObjs objects) {
	for (auto &obj : objects) {
		if (obj == NULL)
			continue;
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
				case SDLK_z:
					e.key = ROT_LEFT;
					break;
				case SDLK_x:
					e.key = ROT_RIGHT;
					break;
			}
			onInputEvent(e);
		}
	}
}

void Platform::update(uint32_t time, std::vector<uint8_t> tileMap, GameObjs objects, std::shared_ptr<GamePiece> active, std::shared_ptr<GameState> p2state) {
	// draw and clear renderer
	drawObjs(objects);
	if (active)
		drawActive(active, true);
	drawTileMap(tileMap, true);
	if (game_flags & TWOP_FLAG) {
		if (p2state) {
			oldActive = NULL;
			oldTileMap = std::vector<uint8_t>();
			if (p2state->active) {
				drawActive(p2state->active, false);
				oldActive = active;
			}
			oldTileMap = p2state->tileMap;
			drawTileMap(p2state->tileMap, false);
		} else {
			if (oldActive)
				drawActive(oldActive, false);
			if (oldTileMap.size() > 0)
				drawTileMap(oldTileMap, false);
		}
	}
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	handleEvents();

	// wait
	double trueWaitTime = Platform::msPerFrame - SDL_GetTicks() + frameTime + remainder;
	int waitTime = trueWaitTime;
	if (game_flags & LOSE_FLAG)
		waitTime = 1000;
	if (waitTime > 0) {
		remainder = trueWaitTime - waitTime;
		SDL_Delay(waitTime);
	}
	frameTime = SDL_GetTicks();
}

const double Platform::msPerFrame = 16.667L;
