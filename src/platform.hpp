#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "trl.hpp"
#include "piece.hpp"

class Platform {
	public:
		bool init(int width, int height);
		void update(uint32_t time, std::shared_ptr<GameState> p1state,
					GameObjs objects, std::shared_ptr<GameState> p2state);
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
};

extern Platform platform;

#endif
