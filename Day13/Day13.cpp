#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include "Cart.h"

/*
- turns left the first time, goes straight the second time, turns right the third time, and then repeats those directions..
- arts on the top row move first (acting from left to right), then carts on the second row move (again from left to right)...
- each of these loops is called a tick.
- The location of the crash is marked with an X.
- you'd like to know the location of the first crash in (X,Y)
- X: Columns from left to right, starting at 0
- Y: Rows from top to bottom, starting at 0
-> Example result: (7,3) 
*/

typedef std::vector<Cart> cartList;

std::string getInputAsString(std::string);
std::list<std::string> getInputPerLines(std::string);

int main() {

	size_t nCols, nRows;
	
	std::list<std::string> rules = getInputPerLines("input_Day13.txt");
	
	nCols = rules.front().size();
	nRows = rules.size();
	charMap cmap(nCols, std::vector<char>(nRows, ' '));
	std::cout << "Map size is: " << nCols << " x " << nRows << std::endl;
	cartList carts;	

	// Process map
	size_t xPos = 0, yPos=0;
	for (std::list<std::string>::iterator it = rules.begin(); it != rules.end(); ++it) {
		// procces each line of the map
		xPos = 0;
		for (std::string::iterator at = it->begin(); at != it->end(); ++at) {
			// fill cmap
			cmap[xPos][yPos] = *at;

			// Chars to process: | - / \ + > < ^ v
			// |: 1 - vertical track
			// -: 2 - horizontal track
			// /: 3 - Turn right: BotRight or TopLeft
			// \: 4 - Turn left: BotLeft or TopRight
			// +: 5 - intersection
			// >: 6 - cart going right on horizontal track
			// <: 7 - cart going left on horizontal track
			// ^: 8 - cart going up on vertical track
			// v: 9 - cart going down on vertical track
			
			// Get Carts on map and replace its position with the proper track
			if (*at == '>') {
				cmap[xPos][yPos] = '-'; *at = '-';
				carts.push_back(Cart((int)xPos, (int)yPos, Right));
				std::cout << "Cart added at [" << xPos << "," << yPos << "] " << std::endl;
			}
			else if (*at == '<') {
				cmap[xPos][yPos] = '-'; *at = '-';
				carts.push_back(Cart((int)xPos, (int)yPos, Left));
				std::cout << "Cart added at [" << xPos << "," << yPos << "]" << std::endl;
			}
			else if (*at == '^') {
				cmap[xPos][yPos] = '|'; *at = '|';
				carts.push_back(Cart((int)xPos, (int)yPos, Up));
				std::cout << "Cart added at [" << xPos << "," << yPos << "]" << std::endl;
			}
			else if (*at == 'v') {
				cmap[xPos][yPos] = '|'; *at = '|';
				carts.push_back(Cart((int)xPos, (int)yPos, Down));
				std::cout << "Cart added at [" << xPos << "," << yPos << "]" << std::endl;
			}
			++xPos;
		}
		++yPos;
	}

	// Sort cart list not needed, the way we find them makes them sorted
	std::cout << "Num Carts: " << carts.size() << std::endl;

	// Start moving carts autonomously
	std::vector<std::vector<char>> trails(nCols, std::vector<char>(nRows, ' '));
	bool lastManStanding = false;
	while (!lastManStanding) {
		// Move all carts
		for (cartList::iterator it = carts.begin(); it != carts.end(); ++it) {
			// move next car
			it->moveAuto(cmap, carts);
		}
		
		// remove defect carts
		for (std::vector<Cart>::iterator it = carts.begin(); it != carts.end(); ) {
			// remove defect carts
			if (it->defect) {
				std::cout << "Removing defect cart " << it->num << " at ";
				std::cout << "[" << it->getX() << "," << it->getY() << "]" << std::endl;
				it = carts.erase(it);
			}
			else {
				++it;
			}
		}

		// sort such that the carts get moved in the right order
		std::sort(carts.begin(), carts.end(), cartOrder);

		// Show location of last cart
		if (carts.size() <= 1) {
			std::cout << "Only " << carts.size() << " cart left:" << std::endl;
			for (cartList::iterator it = carts.begin(); it != carts.end(); ++it) {
				std::cout << "Cart(" << it->num << ") at [" << it->getX() << "," << it->getY() << "] ";
				std::cout << "<- Solution Part 2" << std::endl;
			}
			lastManStanding = true;
		}

		//dispCartMap(cmap, carts, trails);
	}
	
	
	return 0;
}


/******************************
*   INPUT HELPER FUNCTIONS   *
******************************/

std::string getInputAsString(std::string fileName) {
	std::string str;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
		return 0;
	}

	// Create String
	str.assign((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	in.close();
	return str;
}

std::list<std::string> getInputPerLines(std::string fileName) {
	std::list<std::string> lines;
	std::string line;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
	}

	// Create Vector of lines
	while (getline(in, line)) {
		lines.push_back(line);
	}
	in.close();
	return lines;
}
