#include <time.h>
#include <stdlib.h>

#include "game.hpp"

Game game;

Game::Game() {
	objects = std::vector<std::shared_ptr<GameObject>>();
	active = NULL;
	moveUp = 0, moveDown = 0, moveRight = 0, moveUp = 0;
	gravityTimer = 0, moveTimer = 0, lockTimer = -1;
	lockTimerMax = 50;
	rngInit = 0;
	rightPriority = false;
	tileMap = std::vector<uint8_t>(200);
}

void Game::printMap() {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			printf("%02X ", tileMap[i * MAP_WIDTH + j]);
		}
		puts("");
	}
}

Game::~Game() {
}

void Game::handleInput(InputEvent e) {
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
		case ROT_LEFT:
			if (!e.keyUp)
				rotateActive(true);
			break;
		case ROT_RIGHT:
			if (!e.keyUp)
				rotateActive(false);
			break;
			break;
		default:
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
			if (y < 0)
				break;
			if (x < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT || tileMap[y * MAP_WIDTH + x]) {
				return true;
			}
		}
	}
	return false;
}

void Game::checkLineClear() {
	int idx = 0;
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		bool full = true;
		for (int j = 0; j < MAP_WIDTH; ++j) {
			if (!tileMap[i * MAP_WIDTH + j]) {
				full = false;
				break;
			}
		}
		if (full) {
			uint8_t tmp[MAP_HEIGHT * MAP_WIDTH];
			idx = i * MAP_WIDTH;
			memcpy(&tmp, &tileMap[0], idx);
			memcpy(&tileMap[MAP_WIDTH], &tmp, idx);
			memset(&tileMap[0], 0, MAP_WIDTH);
			game_flags |= LINE_CLEAR;
		}
	}
}

void Game::placeActive(bool force) {
	if (!force) {
		if (lockTimer == -1) {
			lockTimer = lockTimerMax;
			return;
		}
		lockTimer = -1;
	}
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
	checkLineClear();
	active = NULL;
}

void Game::moveActive() {
	if (lockTimer > 0) {
		--lockTimer;
	}

	// horizontal movement logic
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

	// down arrow movement logic
	oldx = active->x;
	if (moveDown) {
		if (++moveTimer == 2) {
			active->y += moveDown;
			moveTimer = 0;
			if (checkCollision()) {
				active->y = oldy;
				placeActive(false);
				if (!active)
					return;
			}
		}
	} else {
		moveTimer = 0;
	}
	// if theres no lock timer, do gravity logic
	if (lockTimer == -1) {
		oldy = active->y;
		if (++gravityTimer == 1) {
			active->y++;
			gravityTimer = 0;
		}
		if (checkCollision()) {
			active->y = oldy;
			placeActive(false);
		} 
	}
	// if theres still a piece, do lock timer logic
	if (active) {
		active->y++;
		bool col = checkCollision();
		if (lockTimer > 0 && !col) {
			lockTimer = -1;
		} else if (lockTimer == 0 && col) {
			active->y--;
			placeActive(false);
			lockTimer = -1;
			return;
		} else if (col && lockTimer == -1) {
			lockTimer = lockTimerMax;
		}
		active->y--;
	} else {
		lockTimer = -1;
	}
}

std::shared_ptr<GamePiece> Game::nextPiece() {
	if (!rngInit) {
		rngInit = 1;
		srand(time(NULL));
	}
	auto ptr = std::make_shared<GamePiece>();
	ptr->x = 3;
	ptr->y = -1;
	ptr->rot = 1;
	int idx = rand() % 7;
	if (!idx)
		ptr->y -= 1;
	else if (idx == 1)
		ptr->rot = 0;
	ptr->map = PieceMap(pieces[idx]);
	return ptr;
}

void Game::rotateActive(bool cc) {
	if (!active || !active->rot)
		return;
	PieceMap m = PieceMap(active->map);
	int h = active->map.size();
	int w = active->map[0].size();
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (cc)
				m[h-j-1][i] = active->map[i][j];
			else
				m[j][h-i-1] = active->map[i][j];

		}
	}
	PieceMap old = active->map;
	active->map = m;
	if (checkCollision())
		active->map = old;
}


void Game::update(uint32_t time) {
	if (game_flags & LINE_CLEAR)
		return;
	if (!active) {
		active = nextPiece();
		if (checkCollision()) {
			game_flags |= LOSE_FLAG;
			placeActive(true);
		}
	}
	if (active) {
		moveActive();
	}
}
