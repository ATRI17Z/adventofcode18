#pragma once

#include <iostream>
#include <list>
#include <vector>

typedef std::vector<std::vector<char>> Map;
typedef std::vector<std::vector<int>> CostMap;

class Coordinate {
public:
	Coordinate() : x(0), y(0), distance(INT_MAX), gCost(INT_MAX), fCost(INT_MAX) {}
	Coordinate(int x, int y) : x(x), y(y), distance(INT_MAX), gCost(INT_MAX), fCost(INT_MAX) {}
	~Coordinate() {}

	int getX() const { return x; }
	void setX(int x) { this->x = x; }
	int getY() const { return y; }
	void setY(int y) { this->y = y; }

	int getD() const { return distance; }
	void setD(int d) { this->distance = d; }

	int getF() const { return fCost; }
	void setF(int c) { this->fCost = c; }

	int getG() const { return gCost; }
	void setG(int c) { this->gCost = c; }


private:

	int x, y;
	int distance;
	int fCost, gCost;
};

// Operator definition for Class Type Coordinate
inline bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
	// return true if same X and Y coordinates
	if (lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY()) return true;
	return false;
}
inline bool operator!=(const Coordinate& lhs, const Coordinate& rhs) { return !(lhs==rhs); }
inline bool operator< (const Coordinate& lhs, const Coordinate& rhs) { 
	// returns true if this Coordinates gets picke first in a 'reading sense' 
	if (lhs.getY() < rhs.getY()) return true;
	else if (lhs.getY() == rhs.getY() && lhs.getX() < rhs.getX()) return true;
	else return false; 
}
inline bool operator> (const Coordinate& lhs, const Coordinate& rhs) { return rhs < lhs; }
inline bool operator<=(const Coordinate& lhs, const Coordinate& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Coordinate& lhs, const Coordinate& rhs) { return !(lhs < rhs); }

// Sort Comparator for minimal F Cost



class Warrior
{
public:
	Warrior() : x(0), y(0), hitP(0), hp(0), dead(true), gORe('x') {}
	virtual ~Warrior() {}

	int getX() const { return x; }
	int getY() const { return y; }
	int getHP() const { return hp; }
	int getHitPoints() const { return hitP; }
	bool isDead() const { return dead; }
	virtual char getWarriorTag() = 0;
	virtual char getEnemyTag() = 0;
	void move(Map& map, std::list<Warrior*> enemy);
	void attack(Map& map, std::list<Warrior*>& enemy);
	void attackEnemy( Warrior* enemy);
	void getAttacked( int damage);

protected:
	Warrior(int x, int y, int hitP, int hp, bool dead, char gOrE) : x(x), y(y), hitP(hitP), hp(hp), dead(false), gORe(gOrE) {}
	

	
	

private:
	int x, y;	// Position
	int hitP;	// HitPoints
	int hp;		// Health Points
	bool dead;	// Alive or not?
	char gORe;	// Goblin or Elf

	// private methods
	// Moving
	bool isAdjacentToEnemy(const Map& map);
	std::list<Coordinate> getFeasible(const Map& map, std::list<Warrior*> enemy);
	std::list<std::list<Coordinate>> getReachable(const Map& map, std::list<Coordinate> inRange);
	Coordinate getNextStep(std::list<std::list<Coordinate>> paths);
	void setNewPosition(Map& map, Coordinate target);

	// Attacking
	std::list<Warrior*> getTargets(Map& map, std::list<Warrior*> enemy);
	virtual Warrior* getOpponent(std::list<Warrior*> enemy);

	

};

// Helper
int getManhattenDistance(int sX, int sY, int gX, int gY);
std::list<Coordinate> reconstruct_path(const CostMap& cameFrom, std::list<Coordinate>::iterator cur, Coordinate start);
bool minLength(const std::list<Coordinate>& first, const std::list<Coordinate>& second);
bool minPos(const Warrior* first, const Warrior* second);


