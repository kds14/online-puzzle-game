#include <time.h>
#include <stdlib.h>

#include "game.hpp"

Game game;

Game::Game() {
	objects = std::vector<std::shared_ptr<GameObject>>(10);
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

bool Game::checkCollision() {
	int x, y;
	for (std::size_t i = 0; i < active->map.size(); ++i) {
		for (std::size_t j = 0; j < active->map[i].size(); ++j) {
			if (!active->map[i][j])
				continue;
			x = active->x + j;
			y = active->y + i;
			if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || tileMap[y * MAP_WIDTH + x]) {
				return true;
			}
		}
	}
	return false;
}

void Game::placeActive() {
	int x, y;
	for (std::size_t i = 0; i < active->map.size(); ++i) {
		for (std::size_t j = 0; j < active->map[i].size(); ++j) {
			if (!active->map[i][j])
				continue;
			x = active->x + j;
			y = active->y + i;
			tileMap[y * MAP_WIDTH + x] = 1;
		}
	}
	active = NULL;
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
	if (checkCollision()) {
		active->y = oldy;
		placeActive();
		return;
	}
	if (++gravityTimer == 64) {
		active->y++;
		gravityTimer = 0;
	}
	if (checkCollision()) {
		active->y = oldy;
		placeActive();
	}
}

std::shared_ptr<GamePiece> Game::nextPiece() {
	if (!rngInit) {
		rngInit = 1;
		srand(time(NULL));
	}
	auto ptr = std::make_shared<GamePiece>();
	ptr->x = 4;
	ptr->y = 0;
	ptr->rot = 0;
	ptr->map = pieces[rand() % 7];
	return ptr;
}

void Game::update(uint32_t time) {
	if (!active) {
		active = nextPiece();
	}
	if (active) {
		moveActive();
	}
}
