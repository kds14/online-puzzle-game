#ifndef TRL_H
#define TRL_H

#include <memory>
#include <vector>
#include <algorithm>
#include <string.h>

#include "piece.hpp"

extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;
extern const int TILE_SIZE;

extern uint32_t game_flags;
static const uint32_t LINE_CLEAR = 0x1;
static const uint32_t TWOP_FLAG = 0x2;

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
};



#endif
