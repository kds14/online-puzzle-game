#include "platform.hpp"

Platform platform;
//SDL_Texture *tex;

struct TileColor {
	uint8_t r, g, b;
};

static TileColor tileColors[] = {{143,86,59}, {172,50,50}, {106,190,48},
	{91,110,225}, {251,242,54}};

void Platform::drawTileMap(std::vector<uint8_t> tileMap, bool p1) {
	// draw blocks
	int offset = p1 ? p1Offset : p2Offset;
	for(int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i) {
		if (!tileMap[i])
			continue;
		SDL_Rect rect;
		rect.y = TILE_SIZE * (yOffset + i / MAP_WIDTH);
		rect.x = TILE_SIZE * (offset + i % MAP_WIDTH);
		rect.w = TILE_SIZE;
		rect.h = TILE_SIZE;
		TileColor c = tileColors[tileMap[i]-1];
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &rect);
	}

	// draw border
	SDL_Rect bRect;
	bRect.y = yOffset * TILE_SIZE;
	bRect.x = offset * TILE_SIZE;
	bRect.w = TILE_SIZE * MAP_WIDTH;
	bRect.h = TILE_SIZE * MAP_HEIGHT;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &bRect);
}

void Platform::drawActive(std::shared_ptr<GamePiece> active, bool p1) {
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
			TileColor c = tileColors[active->map[i][j] - 1];
			SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}


bool Platform::init(int width, int height) {
	// change layout if 2p
	if (game_flags & TWOP_FLAG) {
		width = width * 3 + 2;
		p2Offset = width - 8;
	} else {
		width *= 2;
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

	/*if (IMG_Init(IMG_INIT_PNG) < 0) {
	  fprintf(stderr, "SDL_image failed to initialize: %s\n", IMG_GetError());
	  return false;
	  }
	  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	  SDL_Surface *surf = IMG_Load("/home/kds/Pictures/junkai_smol.png");
	  if (!surf) {
	  printf("Failed to load img: %s\n", IMG_GetError());
	  }
	  tex = SDL_CreateTextureFromSurface(renderer, surf);
	  SDL_FreeSurface(surf);*/

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

void Platform::drawHealthBar(uint8_t hp, bool p1) {
	SDL_Rect bar;
	int offset = p1 ? p1Offset : p2Offset;
	bar.x = offset * TILE_SIZE;
	bar.y = (yOffset + MAP_HEIGHT + 0.5) * TILE_SIZE;
	bar.h = TILE_SIZE;
	bar.w = MAP_WIDTH * TILE_SIZE;

	SDL_Rect health;
	health.x = bar.x;
	health.y = bar.y;
	health.h = bar.h;
	health.w = (hp/255.0) * MAP_WIDTH * TILE_SIZE;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &bar);
	TileColor c = tileColors[1];
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
	SDL_RenderFillRect(renderer, &health);
}

void Platform::update(uint32_t time, std::shared_ptr<GameState> p1state, GameObjs objects, std::shared_ptr<GameState> p2state) {
	// draw and clear renderer
	drawObjs(objects);
	if (p1state->active)
		drawActive(p1state->active, true);
	drawHealthBar(p1state->hp, true);
	drawTileMap(p1state->tileMap, true);
	if (game_flags & TWOP_FLAG) {
		if (p2state) {
			oldState = std::make_shared<GameState>();
			if (p2state->active) {
				drawActive(p2state->active, false);
				oldState->active = p2state->active;
			}
			oldState->tileMap = p2state->tileMap;
			drawTileMap(p2state->tileMap, false);
			drawHealthBar(p2state->hp, false);
			oldState->hp = p2state->hp;
		} else if (oldState) {
			if (oldState->active)
				drawActive(oldState->active, false);
			if (oldState->tileMap.size() > 0)
				drawTileMap(oldState->tileMap, false);
			drawHealthBar(oldState->hp, false);
		}
	}
	/*SDL_Rect target;
	  target.x = (MAP_WIDTH + 4) * TILE_SIZE;
	  target.y = TILE_SIZE;
	  target.w = (MAP_WIDTH / 2) * TILE_SIZE;
	  target.h = (1200.0/850)*((MAP_WIDTH / 2) * TILE_SIZE);
	  SDL_RenderCopy(renderer, tex, NULL, &target);
	 */
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
