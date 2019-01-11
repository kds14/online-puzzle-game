#ifndef GAME_H
#define GAME_H

#include "trl.hpp"

class Game {
	public:
		void update(uint32_t time);
		void handleInput(InputEvent e);
		Game();
		~Game();

		std::vector<uint8_t> tileMap;
		GameObjs objects;
	private:
		void moveActive();
		bool checkCollision();
		void placeActive();

		bool rightPriority = false;
		int moveUp, moveDown, moveRight, moveLeft;
		int gravityTimer;
		std::shared_ptr<GameObject> active;
};

static std::vector<std::vector<bool>> pieceMap = { {1, 1}, {1, 1} };

static const int moveSpeed = 32;

extern Game game;

#endif
