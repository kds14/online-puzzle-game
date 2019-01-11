#include <iostream>

#include "game.hpp"
#include "platform.hpp"
#include "trl.hpp"

const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 20;
const int TILE_SIZE = 32;

uint32_t game_flags = 0;

void handleInput(InputEvent e) {
	game.handleInput(e);
}

int main() {
	platform.init(MAP_WIDTH, MAP_HEIGHT);
	platform.onInputEvent = handleInput;
	uint32_t time = 0;
	int line_timer = 0;
	while(1) {
		if (game_flags & LINE_CLEAR && ++line_timer == 40) {
			game_flags &= ~LINE_CLEAR;
			line_timer = 0;
		}
		game.update(time);
		platform.update(time, game.tileMap, game.objects, game.active);
		time++;
	}
	return 0;
}
