#ifndef TRL_H
#define TRL_H

#include <memory>
#include <vector>
#include <algorithm>

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

typedef std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> GameObjsPtr;

#endif
