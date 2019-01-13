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
		bool rightPriority;
		int moveUp, moveDown, moveRight, moveLeft;
		int gravityTimer, moveTimer, lockTimer;
		bool rngInit;
		int lockTimerMax;

		void moveActive();
		void rotateActive(bool cc);
		void placeActive();
		bool checkCollision();
		std::shared_ptr<GamePiece> nextPiece();
		void checkLineClear();
		void printMap();
};


static const int moveSpeed = 32;

extern Game game;

#endif
