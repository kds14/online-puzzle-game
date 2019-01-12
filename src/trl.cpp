#include <iostream>
#include <string>

#include "game.hpp"
#include "platform.hpp"
#include "trl.hpp"
#include "network.hpp"

const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 20;
const int TILE_SIZE = 32;

uint32_t game_flags = 0;

void handleInput(InputEvent e) {
	game.handleInput(e);
}

void parseArgs(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (!arg.compare(HOST_ARG)) {
			if (argc > i + 1) {
				std::string p = argv[i+1];
				if (!network.host(p))
					printf("Successful connection on port %s\n", p.c_str());
				else
					exit(0);
			} else {
				fprintf(stderr, "Error: No port given with -h\n");
				exit(0);
			}
		} else if (!arg.compare(CLIENT_ARG)) {
			if (argc > i + 2) {
				std::string h = argv[i+1];
				std::string p = argv[i+2];
				if (!network.conn(h, p))
					printf("Successful connection to host %s on port %s\n", h.c_str(), p.c_str());
				else
					exit(0);
			} else {
				fprintf(stderr, "Error: Incorrect connection format. Try -c HOSTNAME PORT\n");
				exit(0);
			}
		}
	}
}

int main(int argc, char** argv) {
	parseArgs(argc, argv);
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
