#ifndef TRL_H
#define TRL_H

#include <memory>
#include <vector>
#include <algorithm>

extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;
extern const int TILE_SIZE;

enum Key {UP, DOWN, LEFT, RIGHT};

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

#endif
