#ifndef GAME_H
#define GAME_H

#include "trl.hpp"

class Game {
	public:
		void update(uint32_t time);
		void handleInput(InputEvent e);
		GameObjsPtr objects;
		Game();
	private:
		std::shared_ptr<GameObject> active;
		int fallRate = 10;
		bool moveUp = false;
		bool moveDown = false;
		bool moveRight = false;
		bool moveLeft = false;
		int moveSpeed = 32;
		
};

extern Game game;

#endif
