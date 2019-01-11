#include "game.hpp"

Game game;

Game::Game() {
	objects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();
	active = NULL;
	moveUp = 0, moveDown = 0, moveRight = 0, moveUp = 0;
	gravityTimer = 0;
	tileMap = std::vector<uint8_t>(200);
}

Game::~Game() {
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

// 0: no col, 1: return to prev pos, 2: return to prev pos + lock
/*int Game::checkCollision() {
	int res = 0;
	if (MAP_WIDTH < active->x+64 || 0 > active->x) {
		res = 1;
	}
	if (MAP_HEIGHT < active->y+64 || 0 > active->y) {
		res = 2;
	}
	if (res)
		return res;
	for (auto &obj : *objects) {
		res = 0;
		if (obj == active)
			continue;
		if (obj->x <= active->x+64 && obj->x >= active->x) {
			res = 1;
			if (obj->y <= active->y+64 && obj->y >= active->y) {
				res = 2;
			}
		}
		if (res)
			return res;
	}
	return 0;
}*/

bool Game::checkCollision() {
	int x, y;
	for (std::size_t i = 0; i < pieceMap.size(); ++i) {
		for (std::size_t j = 0; j < pieceMap[i].size(); ++j) {
			x = active->x + j;
			y = active->y + i;
			if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || tileMap[y * MAP_WIDTH + x]) {
				return true;
			}
		}
	}
	return false;
}

void Game::moveActive() {
	int oldx = active->x;
	int oldy = active->y;
	if (rightPriority && moveRight) {
		if (moveRight++ == 1 || moveRight > 14) {
			active->x++;
		}
		if (moveLeft)
			moveLeft = 1;
	} else if (!rightPriority && moveLeft) {
		if (moveLeft++ == 1 || moveLeft > 14) {
			active->x--;
		}
		if (moveRight)
			moveRight = 1;
	}
	if (checkCollision())
		active->x = oldx;
	active->y += moveDown;
	if (++gravityTimer == 64) {
		active->y++;
		gravityTimer = 0;
	}
	if (checkCollision()) {
		active->y = oldy;
		active = NULL;
	}
}

void Game::update(uint32_t time) {
	if (!active) {
		active = std::make_shared<GameObject>(5, 0, 1);
		objects->push_back(active);
	}
	if (active) {
		moveActive();
	}
}
