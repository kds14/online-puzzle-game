#include "game.hpp"

Game game;

Game::Game() {
	objects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
	active = NULL;
}

void Game::update(uint32_t time) {
	if (!(time % 420)) {
		puts("YAY");
		active = std::make_shared<GameObject>(100, 20, 1);
		objects->push_back(active);
	}
	if (active) {
		++active->y;
		if (active->y > 500) {
			objects->erase(std::remove(objects->begin(), objects->end(), active), objects->end());
			active = NULL;
		}
	}
}
