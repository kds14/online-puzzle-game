#include <iostream>

#include "game.hpp"
#include "platform.hpp"
#include "trl.hpp"

void handleInput(InputEvent e) {
	game.handleInput(e);
}

int main() {
	platform.init(320,640);
	platform.onInputEvent = handleInput;
	uint32_t time = 0;
	while(1) {
		game.update(time);
		platform.update(time, game.objects);
		time++;
	}
	return 0;
}
