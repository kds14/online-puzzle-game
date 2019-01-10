#ifndef PLATFORM_H
#define PLATFORM_H

#include "SDL.h"
#include "trl.hpp"

class Platform {
	public:
		bool init(int width, int height);
		void update(uint32_t time, GameObjsPtr objects);
		void (*onInputEvent)(InputEvent);
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		double remainder = 0;
		uint32_t frameTime = 0;
		static const double msPerFrame;

		void drawO(int x, int y);
		void drawObjs(GameObjsPtr objects);
		void handleEvents();
};

extern Platform platform;

#endif
