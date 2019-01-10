#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <algorithm>

struct GameObject {
	int x, y, sprite;
	GameObject(int x, int y, int sprite) {
		this->x = x;
		this->y = y;
		this->sprite = sprite;
	}
};

typedef std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> GameObjsPtr;

class Game {
	public:
		void update(uint32_t time);
		GameObjsPtr objects;
		Game();
	private:
		std::shared_ptr<GameObject> active;
		
};

extern Game game;

#endif
