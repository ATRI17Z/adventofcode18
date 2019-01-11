#pragma once
#include "Warrior.h"
class Goblin : 	public Warrior
{
public:
	Goblin();
	Goblin(int x, int y, int hitP, int hp, bool dead) : Warrior(x, y, hitP, hp, dead,'G') {}
	~Goblin();

	char getWarriorTag() { return 'G'; }
	char getEnemyTag() { return 'E'; }
};

