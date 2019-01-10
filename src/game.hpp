#ifndef GAME_H
#define GAME_H

#include "trl.hpp"

class Game {
	public:
		void update(uint32_t time);
		void handleInput(InputEvent e);
		Game();
		GameObjsPtr objects;
	private:
		void moveActive();
		std::shared_ptr<GameObject> active;
		bool rightPriority = false;
		int moveUp = 0;
		int moveDown = 0;
		int moveRight = 0;
		int moveLeft = 0;
		static const int moveSpeed = 32;
		int gravityTimer = 0;
		
};

extern Game game;

#endif
