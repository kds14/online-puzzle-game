#ifndef BOSS_H
#define BOSS_H

#include "trl.hpp"
#include "piece.hpp"

class Boss {
	public:
		Boss(int hp) {
			this->hp = hp; 
			this->maxhp = hp; 
		}
		void dealDmg(int dmg) {
			hp -= dmg;
		}
		virtual void update(std::shared_ptr<GameState> state,
							std::unique_ptr<ClearData> clearData) = 0;
		int hp;
		int maxhp;
};

class Witch : public Boss {
	public:
		Witch(int hp) : Boss(hp) {}
		void update(std::shared_ptr<GameState> state,
					std::unique_ptr<ClearData> clearData) {
			if (clearData) {
				int sum = clearData->r + clearData->g + clearData->b;
				int dmg = sum * clearData->multiplier;
				hp -= dmg;
				state->recDmg = dmg;
			}
			if (hp <= 0) {
				printf("The witch has died!\n");
				exit(0);
			}
			if (time++ >= timer) {
				if (atkflag)
					state->next.push_back(witchPiece);
				else
					state->playerRecDmg = 5;
				atkflag = !atkflag;
				time = 0;
			}
		}
	private:
		bool atkflag = false;
		int timer = 600;
		int time = 0;
};

#endif
