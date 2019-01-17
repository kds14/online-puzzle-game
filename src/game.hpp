#ifndef GAME_H
#define GAME_H

#include "trl.hpp"
#include "piece.hpp"
#include "boss.hpp"

class Game {
	public:
		void init(std::shared_ptr<Boss> boss);
		void update(uint32_t time);
		void handleInput(InputEvent e);
		Game();
		~Game();

		std::shared_ptr<GameState> state;
		std::unique_ptr<ClearData> clearData;

		GameObjs objects;
	private:
		std::shared_ptr<Boss> boss;
		bool rightPriority;
		int moveUp, moveDown, moveRight, moveLeft;
		int gravityTimer, moveTimer, lockTimer;
		bool rngInit;
		int lockTimerMax;
		int playerHp, maxPlayerHp;
		std::list<int> toClear;
		double multiplier;

		void onOverflow();
		void moveActive();
		void rotateActive(bool cc);
		void placeActive(bool force);
		bool checkCollision();
		std::shared_ptr<GamePiece> nextPiece();
		void checkLineClear();
		void printMap();
		std::vector<int> getNeighbors(int idx);
		void applyGravity(int idx);
		bool checkTileCollision(int idx);
		void absCheckLineClear();
		void applyGravityByGroup();
};


static const int moveSpeed = 32;

extern Game game;

#endif
