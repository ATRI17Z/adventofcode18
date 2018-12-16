#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

enum direction {
	Up,
	Right,
	Down,
	Left
};

enum intersectionTurn {
	TurnLeft,
	Straight,
	TurnRight
};

typedef std::vector<std::vector<char>> charMap;

class Cart
{
public:
	Cart(int x, int y, direction dir): x(x), y(y), moveDir(dir), nextTurn(TurnLeft), num(counter++), defect(false) {}
	~Cart() {};
	
	static int counter;		// bad design ;)
	int num;				// bad design ;)
	bool defect;			// bad design ;)

	void getState(int& x, int& y, direction& dir, intersectionTurn& lastTurn) {
		x = this->x;
		y = this->y;
		dir = this->moveDir;
		lastTurn = this->nextTurn;
	};

	int getX() { return x; }
	int getY() { return y; }

	bool moveAuto(charMap& map, std::vector<Cart>& carts);

private:
	
	int x, y;
	direction moveDir;
	intersectionTurn nextTurn;
};

//void removeBrokenCarts(std::vector<Cart>& carts);
void dispCartMap(charMap& map, std::vector<Cart>& carts, std::vector<std::vector<char>>& trails);
bool collisionOccurred(std::vector<Cart>& carts);
bool cartOrder(Cart cart1, Cart cart2);