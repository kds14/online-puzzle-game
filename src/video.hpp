#ifndef VIDEO_H
#define VIDEO_H

#include "SDL.h"
#include "game.hpp"

class Video {
	public:
		bool init(int width, int height);
		void update(uint32_t time, GameObjsPtr objects);
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		double remainder = 0;
		uint32_t frameTime = 0;
		static const double msPerFrame;

		void drawO(int x, int y);
		void drawObjs(GameObjsPtr objects);
};

extern Video video;

#endif
