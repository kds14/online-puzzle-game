#include <time.h>
#include <unistd.h>

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
	multiplier = 1.0L;
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

bool Game::checkTileCollision(int idx) {
	int x = idx % MAP_WIDTH;
	int y = (idx / MAP_WIDTH);
	if (x < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT || state->tileMap[idx] > CLEAR_PIECE)
		return true;
	return false;
}

bool Game::checkCollision() {
	int x, y;
	for (std::size_t i = 0; i < state->active->map.size(); ++i) {
		for (std::size_t j = 0; j < state->active->map[i].size(); ++j) {
			if (state->active->map[i][j] <= CLEAR_PIECE)
				continue;
			x = state->active->x + j;
			y = state->active->y + i;
			if (y < 0)
				break;
			if (x < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT || state->tileMap[y * MAP_WIDTH + x] > 1) {
				return true;
			}
		}
	}
	return false;
}

std::vector<int> Game::getNeighbors(int idx) {
	std::vector<int> r;
	if (idx % MAP_WIDTH) {
		r.push_back(idx-1);
	}
	if ((idx + 1) % MAP_WIDTH) {
		r.push_back(idx+1);
	}
	if ((idx + MAP_WIDTH) < MAP_WIDTH * MAP_HEIGHT) {
		r.push_back(idx+MAP_WIDTH);
	}
	if ((idx - MAP_WIDTH) >= 0 ) {
		r.push_back(idx - MAP_WIDTH);
	}
	return r;
}

void Game::absCheckLineClear() {
	auto cd = std::unique_ptr<ClearData>(new ClearData());
	std::list<int> cleared;
	std::vector<bool> vis(MAP_HEIGHT * MAP_WIDTH);
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			int idx = i * MAP_WIDTH + j;
			if (state->tileMap[idx] <= JUNK_PIECE || vis[idx])
				continue;
			std::list<int> poss;
			std::list<int> match;
			poss.push_back(idx);
			match.push_back(idx);
			while(poss.size() > 0) {
				int f = poss.front();
				poss.pop_front();
				vis[f] = true;
				uint8_t fc = state->tileMap[f];
				auto n = getNeighbors(f);
				for (std::size_t k = 0; k < n.size(); ++k) {
					auto nk = state->tileMap[n[k]];
					if (fc == nk && !vis[n[k]] && nk > JUNK_PIECE) {
						vis[n[k]] = true;
						poss.push_back(n[k]);
						match.push_back(n[k]);
					}
				}
			}
			auto msz = match.size();
			if (match.size() >= 4) {
				switch(state->tileMap[match.front()]) {
					case RED_PIECE:
						cd->r += msz;
						break;
					case GREEN_PIECE:
						cd->g += msz;
						break;
					case BLUE_PIECE:
						cd->b += msz;
						break;
				}
				cleared.splice(cleared.end(), match);
			}
		}
	}
	if (cleared.size() > 0) {
		game_flags |= LINE_CLEAR;
		for (auto &i: cleared) {
			state->tileMap[i] = 1;
		}
		toClear = cleared;
		cd->multiplier = multiplier;
		multiplier += 0.1;
	}
	clearData = std::move(cd);
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
	std::list<int> placed;
	for (std::size_t i = 0; i < state->active->map.size(); ++i) {
		for (std::size_t j = 0; j < state->active->map[i].size(); ++j) {
			if (!state->active->map[i][j])
				continue;
			x = state->active->x + j;
			y = state->active->y + i;
			int idx = y * MAP_WIDTH + x;
			placed.push_back(idx);
			state->tileMap[idx] = state->active->map[i][j];
		}
	}
	/*placed.sort([](int a, int b) {return a > b;});
	for (auto &i: placed) {
		applyGravity(i);
	}*/
	absCheckLineClear();
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
		srand(time(NULL) + getpid());
	}
	auto ptr = std::make_shared<GamePiece>();
	if (state->next.size() > 0) {
		ptr->map = state->next.front();
		state->next.pop_front();
	} else {
		int idx = rand() % 3;
		if (!idx)
			ptr->y += 1;
		ptr->map = PieceMap(pieces[idx]);
		for (auto &row : ptr->map) {
			for (auto &item : row) {
				item *= (rand() % 3) + 3;
			}
		}
	}
	ptr->x = 2;
	ptr->y = ptr->map.size() == 2 ? 0 : -1;
	ptr->rot = 1;
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

void Game::applyGravity(int idx) {
	int prev;
	while (!checkTileCollision(idx + MAP_WIDTH)) {
		prev = idx;
		idx += MAP_WIDTH;
		state->tileMap[idx] = state->tileMap[prev];
		state->tileMap[prev] = 0;
	}
}

void Game::init(std::shared_ptr<Boss> boss) {
	this->boss = boss;
}

void Game::applyGravityByGroup() {
	std::vector<bool> vis(MAP_HEIGHT * MAP_WIDTH);
	std::vector<std::list<int>> groups;
	for (int i = MAP_HEIGHT * MAP_WIDTH - 1; i >= 0; --i) {
		if (vis[i] || state->tileMap[i] <= CLEAR_PIECE)
			continue;
		std::list<int> match;
		std::list<int> poss;
		match.push_back(i);
		poss.push_back(i);
		bool grounded = false;
		while (poss.size() > 0) {
			int idx = poss.front();
			poss.pop_front();
			vis[idx] = true;
			if (idx >= (MAP_HEIGHT - 1) * MAP_WIDTH)
				grounded = true;
			auto n = getNeighbors(idx);
			for (auto &ne: n) {
				if (!vis[ne] && state->tileMap[ne] > 1) {
					vis[ne] = true;
					poss.push_back(ne);
					match.push_back(ne);
				}
			}
		}
		if (!grounded)
			groups.push_back(match);
	}

	for (auto &group: groups) {
		std::vector<int> bot(group.size());
		std::copy_if (group.begin(), group.end(), bot.begin(),
				[this](int idx){return !checkTileCollision(idx + MAP_WIDTH);});
		while (true) {
			bool col = false;
			for (auto &b: bot) {
				if (checkTileCollision(b + MAP_WIDTH)) {
					col = true;
					break;
				}
				b += MAP_WIDTH;
			}
			if (col)
				break;
			group.sort([](int a, int b){return a > b;});
			for (auto &g: group) {
				int nxt = g + MAP_WIDTH;
				state->tileMap[nxt] = state->tileMap[g];
				state->tileMap[g] = 0;
				g = nxt;
			}
		}
	}
}

void Game::update(uint32_t time) {
	boss->update(state, std::move(clearData));
	clearData = nullptr;
	if (game_flags & LINE_CLEAR)
		return;
	std::unique_ptr<ClearData> d(nullptr);
	if (toClear.size() > 0) {
		toClear.sort([](int a, int b) {return a > b;});
		for (auto &i: toClear) {
			state->tileMap[i] = 0;
		}
		toClear = std::list<int>();
		applyGravityByGroup();
		absCheckLineClear();
		return;
	}
	multiplier = 1.0L;
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

	playerHp -= state->playerRecDmg;
	state->playerRecDmg = 0;
	if (playerHp <= 0) {
		game_flags |= LOSE_FLAG;
	}
	state->hp = (uint8_t)(((double)playerHp / maxPlayerHp)*255);
}
