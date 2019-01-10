#ifndef VIDEO_H
#define VIDEO_H

#include "SDL.h"

class Video {
public:
	bool init(int width, int height);
	void update();
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
};

extern Video video;

#endif
