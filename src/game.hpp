#ifndef GAME_H
#define GAME_H

#include "trl.hpp"
#include "piece.hpp"

class Game {
	public:
		void update(uint32_t time);
		void handleInput(InputEvent e);
		Game();
		~Game();

		std::vector<uint8_t> tileMap;
		std::shared_ptr<GamePiece> active;
		GameObjs objects;
	private:
		void moveActive();
		bool checkCollision();
		void placeActive();
		std::shared_ptr<GamePiece> nextPiece();

		bool rightPriority = false;
		int moveUp, moveDown, moveRight, moveLeft;
		int gravityTimer;
		bool rngInit = 0;
};


static const int moveSpeed = 32;

extern Game game;

#endif
