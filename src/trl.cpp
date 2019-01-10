#include <iostream>

#include "game.hpp"
#include "video.hpp"

int main() {
	video.init(500,500);
	uint32_t time = 0;
	while(1) {
		game.update(time);
		video.update(time, game.objects);
		time++;
	}
	return 0;
}
