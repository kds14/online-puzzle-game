#ifndef PLATFORM_H
#define PLATFORM_H

#include "SDL.h"
#include "trl.hpp"
#include "piece.hpp"

class Platform {
	public:
		bool init(int width, int height);
		void update(uint32_t time, std::vector<uint8_t> tileMap, GameObjs objects, std::shared_ptr<GamePiece> active);
		void (*onInputEvent)(InputEvent);
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		double remainder = 0;
		uint32_t frameTime = 0;
		static const double msPerFrame;

		void drawTileMap(std::vector<uint8_t> tileMap);
		void drawO(int x, int y);
		void drawObjs(GameObjs objects);
		void drawActive(std::shared_ptr<GamePiece> active);
		void handleEvents();
};

extern Platform platform;

#endif
