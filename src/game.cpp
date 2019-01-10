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
			moveLeft = !e.keyUp;
			if (moveLeft)
				rightPriority = false;
			break;
		case RIGHT:
			moveRight = !e.keyUp;
			if (moveRight)
				rightPriority = true;
			break;
		case UP:
			moveUp = !e.keyUp;
			break;
		case DOWN:
			moveDown = !e.keyUp;
			break;
	}
}

void Game::moveActive() {
	if (rightPriority && moveRight) {
		if (moveRight++ == 1 || moveRight > 14) {
			active->x += moveSpeed;
		}
		if (moveLeft)
			moveLeft = 1;
	} else if (!rightPriority && moveLeft) {
		if (moveLeft++ == 1 || moveLeft > 14) {
			active->x -= moveSpeed;
		}
		if (moveRight)
			moveRight = 1;
	}
	active->y += moveSpeed * moveDown;
	if (++gravityTimer == 64) {
		active->y += moveSpeed;
		gravityTimer = 0;
	}
	if (active->y >= 586) {
		objects->erase(std::remove(objects->begin(), objects->end(), active), objects->end());
		active = NULL;
	}
}

void Game::update(uint32_t time) {
	if (!(time % 2000)) {
		active = std::make_shared<GameObject>(128, 0, 1);
		objects->push_back(active);
	}
	if (active) {
		moveActive();
	}
}
