#include <iostream>

#include "game.hpp"
#include "platform.hpp"
#include "trl.hpp"

const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 20;
const int TILE_SIZE = 32;

void handleInput(InputEvent e) {
	game.handleInput(e);
}

int main() {
	platform.init(MAP_WIDTH, MAP_HEIGHT);
	platform.onInputEvent = handleInput;
	uint32_t time = 0;
	while(1) {
		game.update(time);
		platform.update(time, game.tileMap, game.objects);
		time++;
	}
	return 0;
}
