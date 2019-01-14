#include <time.h>
#include <stdlib.h>

#include "game.hpp"

Game game;

Game::Game() {
	objects = std::vector<std::shared_ptr<GameObject>>();
	moveUp = 0, moveDown = 0, moveRight = 0, moveUp = 0;
	gravityTimer = 0, moveTimer = 0, lockTimer = -1;
	lockTimerMax = 50;
	rngInit = 0;
	rightPriority = false;
	state = std::make_shared<GameState>();
	maxPlayerHp = 100;
	playerHp = maxPlayerHp;
}

void Game::printMap() {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			printf("%02X ", state->tileMap[i * MAP_WIDTH + j]);
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
	for (std::size_t i = 0; i < state->active->map.size(); ++i) {
		for (std::size_t j = 0; j < state->active->map[i].size(); ++j) {
			if (!state->active->map[i][j])
				continue;
			x = state->active->x + j;
			y = state->active->y + i;
			if (y < 0)
				break;
			if (x < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT || state->tileMap[y * MAP_WIDTH + x]) {
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
			if (!state->tileMap[i * MAP_WIDTH + j]) {
				full = false;
				break;
			}
		}
		if (full) {
			uint8_t tmp[MAP_HEIGHT * MAP_WIDTH];
			idx = i * MAP_WIDTH;
			memcpy(&tmp, &state->tileMap[0], idx);
			memcpy(&state->tileMap[MAP_WIDTH], &tmp, idx);
			memset(&state->tileMap[0], 0, MAP_WIDTH);
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
	for (std::size_t i = 0; i < state->active->map.size(); ++i) {
		for (std::size_t j = 0; j < state->active->map[i].size(); ++j) {
			if (!state->active->map[i][j])
				continue;
			x = state->active->x + j;
			y = state->active->y + i;
			state->tileMap[y * MAP_WIDTH + x] = 1;
		}
	}
	checkLineClear();
	state->active = NULL;
}

void Game::moveActive() {
	if (lockTimer > 0) {
		--lockTimer;
	}

	// horizontal movement logic
	int oldx = state->active->x;
	int oldy = state->active->y;
	if (rightPriority && moveRight) {
		if (moveRight++ == 1 || moveRight > 14) {
			state->active->x++;
		}
		if (moveLeft)
			moveLeft = 1;
	} else if (!rightPriority && moveLeft) {
		if (moveLeft++ == 1 || moveLeft > 14) {
			state->active->x--;
		}
		if (moveRight)
			moveRight = 1;
	}
	if (checkCollision())
		state->active->x = oldx;

	// down arrow movement logic
	oldx = state->active->x;
	if (moveDown) {
		if (++moveTimer == 2) {
			state->active->y += moveDown;
			moveTimer = 0;
			if (checkCollision()) {
				state->active->y = oldy;
				placeActive(false);
				if (!state->active)
					return;
			}
		}
	} else {
		moveTimer = 0;
	}
	// if theres no lock timer, do gravity logic
	if (lockTimer == -1) {
		oldy = state->active->y;
		if (++gravityTimer == 32) {
			state->active->y++;
			gravityTimer = 0;
		}
		if (checkCollision()) {
			state->active->y = oldy;
			placeActive(false);
		} 
	}
	// if theres still a piece, do lock timer logic
	if (state->active) {
		state->active->y++;
		bool col = checkCollision();
		if (lockTimer >= 0 && !col) {
			lockTimer = -1;
		} else if (lockTimer == 0 && col) {
			state->active->y--;
			placeActive(false);
			lockTimer = -1;
			return;
		} else if (col && lockTimer == -1) {
			lockTimer = lockTimerMax;
		}
		state->active->y--;
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
	int idx = rand() % 3;
	if (!idx)
		ptr->y += 1;
	ptr->map = PieceMap(pieces[idx]);
	return ptr;
}

void Game::rotateActive(bool cc) {
	if (!state->active || !state->active->rot)
		return;
	PieceMap m = PieceMap(state->active->map);
	int h = state->active->map.size();
	int w = state->active->map[0].size();
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (cc)
				m[h-j-1][i] = state->active->map[i][j];
			else
				m[j][h-i-1] = state->active->map[i][j];

		}
	}
	PieceMap old = state->active->map;
	state->active->map = m;
	if (checkCollision())
		state->active->map = old;
}

void Game::onOverflow() {
	playerHp -= 50;
	if (playerHp <= 0) {
		game_flags |= LOSE_FLAG;
	} else {
		game_flags |= LINE_CLEAR;
	}
	state->tileMap = TileMap(200);
}

void Game::update(uint32_t time) {
	if (game_flags & LINE_CLEAR)
		return;
	if (!state->active) {
		state->active = nextPiece();
		if (checkCollision()) {
			placeActive(true);
			onOverflow();
		}
	}
	if (state->active) {
		moveActive();
	}
	state->hp = (uint8_t)(((double)playerHp / maxPlayerHp)*255);
}
