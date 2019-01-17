#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __linux__
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#elif _WIN32
#include "SDL.h"
#endif

#include "trl.hpp"
#include "piece.hpp"
#include "boss.hpp"

class Platform {
	public:
		bool init(int width, int height);
		void update(uint32_t time, std::shared_ptr<GameState> p1state,
					GameObjs objects, std::shared_ptr<GameState> p2state,
					std::shared_ptr<Boss> boss);
		void (*onInputEvent)(InputEvent);
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		double remainder = 0;
		uint32_t frameTime = 0;
		static const double msPerFrame;
		int p2Offset;
		int p1Offset;
		int yOffset = 2;
		std::shared_ptr<GameState> oldState;

		void drawTileMap(std::vector<uint8_t> tileMap, bool p1);
		void drawObjs(GameObjs objects);
		void drawActive(std::shared_ptr<GamePiece> active, bool p1);
		void drawHealthBar(uint8_t hp, bool p1);
		void handleEvents();
		void drawBoss(std::shared_ptr<Boss> boss);
};

extern Platform platform;

#endif
