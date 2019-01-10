#include "game.hpp"

Game game;

Game::Game() {
	objects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
	active = NULL;
}

void Game::handleInput(InputEvent e) {
	if (!active)
		return;
	switch (e.key) {
		case LEFT:
			moveLeft = e.keyUp ? 0 : 1;
			break;
		case RIGHT:
			moveRight = e.keyUp ? 0 : 1;
			break;
		case UP:
			moveUp = e.keyUp ? 0 : 1;
			break;
		case DOWN:
			moveDown = e.keyUp ? 0 : 1;
			break;
	}
}

void Game::update(uint32_t time) {
	if (!(time % 420)) {
		active = std::make_shared<GameObject>(100, 20, 1);
		objects->push_back(active);
	}
	if (active) {
		active->x += moveSpeed * (moveRight - moveLeft);
		active->y += moveSpeed * moveDown;
		if (++active->y >= 500) {
			objects->erase(std::remove(objects->begin(), objects->end(), active), objects->end());
			active = NULL;
		}
	}
}
