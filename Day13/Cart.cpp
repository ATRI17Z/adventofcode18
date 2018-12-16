#include "Cart.h"

// Init static counter of class
int Cart::counter = 0;

bool Cart::moveAuto(charMap& map, std::vector<Cart>& carts) {
	int X, Y;
	int nX, nY;
	direction dir;
	intersectionTurn nextTurn;
	char nextTrack;
	
	// see in what direction we are looking: moveDir
	this->getState(X, Y, dir, nextTurn); // current state, would have direct access
	// and check what the next element looks like: | - \ / + 
	if (dir == Up) {
		nX = X;
		nY = Y - 1;
	}
	else if (dir == Right) {
		nX = X + 1;
		nY = Y;
	}
	else if (dir == Down) {
		nX = X;
		nY = Y + 1;
	}
	else if (dir == Left) {
		nX = X - 1;
		nY = Y;
	}
	nextTrack = map[nX][nY];

	// Define next movement direction and possibly
	// tne next Turn direction
	if (nextTrack == '\\') {
		if (dir == Up) {
			this->moveDir = Left;
		}else if (dir == Right) {
			this->moveDir = Down;
		}else if (dir == Down) {
			this->moveDir = Right;
		}else if (dir == Left) {
			this->moveDir = Up;
		}
	}
	else if (nextTrack == '/') {
		if (dir == Up) {
			this->moveDir = Right;
		}
		else if (dir == Right) {
			this->moveDir = Up;
		}
		else if (dir == Down) {
			this->moveDir = Left;
		}
		else if (dir == Left) {
			this->moveDir = Down;
		}
	}
	else if (nextTrack == '+') {
		if (dir == Up) {
			if (nextTurn == TurnLeft) {
				this->moveDir = Left;
				this->nextTurn = Straight;
			}
			else if (nextTurn == Straight) {
				// do nothing
				this->nextTurn = TurnRight;
			}
			else if (nextTurn == TurnRight) {
				this->moveDir = Right;
				this->nextTurn = TurnLeft;
			}
		}
		else if (dir == Right) {
			if (nextTurn == TurnLeft) {
				this->moveDir = Up;
				this->nextTurn = Straight;
			}
			else if (nextTurn == Straight) {
				// do nothing
				this->nextTurn = TurnRight;
			}
			else if (nextTurn == TurnRight) {
				this->moveDir = Down; 
				this->nextTurn = TurnLeft;
			}
		}
		else if (dir == Down) {
			if (nextTurn == TurnLeft) {
				this->moveDir = Right;
				this->nextTurn = Straight;
			}
			else if (nextTurn == Straight) {
				// do nothing
				this->nextTurn = TurnRight;
			}
			else if (nextTurn == TurnRight) {
				this->moveDir = Left;
				this->nextTurn = TurnLeft;
			}
		}
		else if (dir == Left) {
			if (nextTurn == TurnLeft) {
				this->moveDir = Down;
				this->nextTurn = Straight;
			}
			else if (nextTurn == Straight) {
				// do nothing
				this->nextTurn = TurnRight;
			}
			else if (nextTurn == TurnRight) {
				this->moveDir = Up;
				this->nextTurn = TurnLeft;
			}
		}
	}

	// move Cart
	this->x = nX;
	this->y = nY;

	// Check for collision
	if (collisionOccurred(carts)) {
		return true;
	}
	return false;
}

void dispCartMap(charMap& cmap, std::vector<Cart>& carts, std::vector<std::vector<char>>& trails) {
	int nCols = 150; // hard coded, i know but was just quicker
	int nRows = 150;
	int X, Y;
	direction Dir;
	intersectionTurn LastTurn;

	// remove all X and make trails out of it
	for (std::vector<std::vector<char>>::iterator it = trails.begin();
		it != trails.end(); ++it) {
		for (std::vector<char>::iterator at = it->begin();
			at != it->end(); ++at) {
			if (*at == 'X') {
				//std::cout << "Is " << *at << " == X" << std::endl;
				*at = 'o';
			}
		}
	}

	// Place cart positions on trails map
	for (std::vector<Cart>::iterator it=carts.begin(); it != carts.end(); ++it) {
		it->getState(X, Y, Dir, LastTurn);
		if (trails[X][Y] == 'X') {
			// already occupied
			std::cout << "COLLISION AT " << X << "," << Y << std::endl;
		}
		trails[X][Y] = 'X';
	}

	// Print map
	for (std::vector<std::vector<char>>::iterator it = trails.begin();
		it != trails.end(); ++it) {
		for (std::vector<char>::iterator at = it->begin();
			at != it->end(); ++at) {
			std::cout << *at;
		}
		std::cout << std::endl;
	}
	
	//std::cin.get();
	
}

bool collisionOccurred(std::vector<Cart>& carts) {
	int xOne, yOne, xTwo, yTwo, num1, num2;
	direction dir;
	intersectionTurn turn;
	for (std::vector<Cart>::iterator it = carts.begin(); it != carts.end(); ++it) {
			it->getState(xOne, yOne, dir, turn);
		for (std::vector<Cart>::iterator at = it + 1; at != carts.end(); ++at) {
			at->getState(xTwo, yTwo, dir, turn);
			// mark collided carts for removal
			if (xOne == xTwo && yOne == yTwo && (!it->defect && !at->defect)) {
				num1 = it->num;
				num2 = at->num;
				it->defect = true;
				at->defect = true;
				return true;
			}
		}
	}
	return false;
}

bool cartOrder(Cart one, Cart two) {
	if (one.getX() < two.getX()) {
		return true;
	}
	else if(one.getX() == two.getX() && one.getY() < two.getY()) {
		return true;
	}
	return false;
}
