#ifndef BOSS_H
#define BOSS_H

#include "trl.hpp"
#include "piece.hpp"

class Boss {
	public:
		Boss(int hp) {
			this->hp = hp; 
		}
		virtual void update(std::shared_ptr<GameState> state,
							std::unique_ptr<ClearData> clearData) = 0;
		int hp;
};

class Witch : public Boss {
	public:
		Witch(int hp) : Boss(hp) {}
		void update(std::shared_ptr<GameState> state,
					std::unique_ptr<ClearData> clearData) {
			if (clearData) {
				int sum = clearData->r + clearData->g + clearData->b;
				int dmg = sum * clearData->multiplier;
				printf("DMG: %d (%d %f)\n", dmg, sum, clearData->multiplier);
				hp -= dmg;
			}
			if (hp <= 0) {
				printf("The witch has died!\n");
				exit(0);
			}
			if (time++ >= timer) {
				state->next.push_back(witchPiece);
				time = 0;
			}
		}
	private:
		int timer = 1200;
		int time = 0;
};

#endif
