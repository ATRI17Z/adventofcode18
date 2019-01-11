#pragma once
#include "Warrior.h"
class Elf : public Warrior
{
public:
	Elf();
	Elf(int x, int y, int hitP, int hp, bool dead) : Warrior(x,y,hitP,hp,dead,'E') {}
	~Elf();

	char getWarriorTag() {return 'E';}
	char getEnemyTag() { return 'G'; }
};

