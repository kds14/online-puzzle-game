#ifndef TRL_H
#define TRL_H

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>
#include <list>
#include <stdlib.h>
#include <stdint.h>

#include "piece.hpp"

extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;
extern const int TILE_SIZE;

extern uint32_t game_flags;
extern const uint32_t LINE_CLEAR;
extern const uint32_t TWOP_FLAG;
extern const uint32_t LOSE_FLAG;

enum Key {UP, DOWN, LEFT, RIGHT, ROT_LEFT, ROT_RIGHT};

struct InputEvent {
	bool keyUp; // false for down
	enum Key key;
};

struct GameObject {
	int x, y, sprite;
	GameObject(int x, int y, int sprite) {
		this->x = x;
		this->y = y;
		this->sprite = sprite;
	}
};

typedef std::vector<std::shared_ptr<GameObject>> GameObjs;
typedef std::vector<uint8_t> TileMap;
typedef std::shared_ptr<GamePiece> GamePiecePtr;

struct GameState {
	GamePiecePtr active;
	TileMap tileMap;
	uint8_t hp;
	std::list<PieceMap> next;
	int recDmg;
	int playerRecDmg;

	GameState() {
		active = NULL;
		tileMap = TileMap(200);
		hp = 0;
		next = std::list<PieceMap>();
	}
};

struct ClearData {
	int r;
	int g;
	int b;
	double multiplier;
};

#endif
