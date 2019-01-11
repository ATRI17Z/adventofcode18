#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#include "Elf.h"
#include "Goblin.h"


/*
 - Unit play in turns (in reading order), a unit is moved and then attacks before next units turn
 - Move into range of an enemy 
 - And attack if in range
 - Units only move vertically or horizontally
 - Ties are broken in reading order (top to bottom then left to right)

 - 1. Identify all possible targets (when no targets available: combat ends)
 - 2. Identify all open squares that are in range of each target (adjacent up,down, left, righ)
 - 3. If the unit is not already in range of a target, and there are no open squares which are in range of a target,
      the unit ends its turn.
      If alraedy in range, does not move but will attack. Otherwise move
 - 4. Plan: * Get the squares in range of an enemy (adjacent of opponents)
            * Which of these are reachable
			* Which of these are nearest
			* Choose the one in reading order
 - 5. Move: Take a single step; if more than one path, choose the step first in reading order (above: horz before vert; below: vert before horz)
 - 6. If Opponents are adjacent, they attack
 - 7. Opponent with fewest hitpoints gets selected (start: 200 hit points, 3 attack points)
 
 You need to determine the outcome of the battle: the number of full rounds that were completed
 (not counting the round in which combat ends) multiplied by the sum of the hit points of all 
 remaining units at the moment combat ends. (Combat only ends when a unit finds no targets during its turn.)
 Elves or Goblins...

 Examples:
 1: 47 * 590 = 27'730
 2: 37 * 982 = 36'334
 3: 46 * 859 = 39'514
 4: 35 - 793 = 27'755
 5: 54 * 536 = 28'944
 6: 20 * 937 = 18'740
 */

typedef std::vector<std::vector<char>> charMap;
typedef std::vector<Elf*> elfsArmy;
typedef std::vector<Goblin*> goblinArmy;

void printMap(const charMap& map);
std::string getInputAsString(std::string);
std::list<std::string> getInputPerLines(std::string);

int main() {

	size_t nCols, nRows;

	std::list<std::string> grid = getInputPerLines("input_Day15_test1.txt");
	nCols = grid.front().size();
	nRows = grid.size();
	std::cout << "Map size is: " << nCols << " x " << nRows << std::endl;

	charMap map(nCols, std::vector<char>(nRows, ' '));
	elfsArmy elfs;
	goblinArmy goblins;
	std::list<Warrior*> warriors;
	
	// Process Map
	int xPos = 0, yPos = 0;
	for (std::list<std::string>::iterator it = grid.begin(); it != grid.end(); ++it) {
		// procces each line of the map
		xPos = 0;
		for (std::string::iterator at = it->begin(); at != it->end(); ++at) {
			// fill cmap
			if (*at == 'E') {
				// Create a new Elf
				//Elf newElf(xPos, yPos, 3, 200, false);
				//std::unique_ptr<Elf> newElf(new Elf(xPos, yPos, 3, 200, false));
				Elf* newElf = new Elf(xPos, yPos, 3, 200, false);
				//elfs.push_back(newElf);
				warriors.push_back(newElf);
				std::cout << "New Elf at " << newElf->getWarriorTag() << ":[" << xPos << "," << yPos << "]" << std::endl;
			}
			else if (*at == 'G') {
				// Create a new Goblin
				//Goblin newGoblin(xPos, yPos, 3, 200, false);
				//std::unique_ptr<Goblin> newGoblin(new Goblin(xPos, yPos, 3, 200, false));
				Goblin* newGoblin = new Goblin(xPos, yPos, 3, 200, false);
				//goblins.push_back(newGoblin);
				warriors.push_back(newGoblin);
				std::cout << "New Goblin at " << newGoblin->getWarriorTag() << ":[" << xPos << "," << yPos << "]" << std::endl;
			}

			map[xPos][yPos] = *at;

			++xPos;
		}
		++yPos;
	}

	bool aliveEnemies = true;
	int battleRound = 0;
	std::cout << "Battle Round: " << battleRound << std::endl;
	printMap(map);
	while (aliveEnemies) {
		// Cycle through all warriors (elfs and goblins)
		// Move and/or attack
		// Reorder list sucht that moves are in reading order

		for (std::list<Warrior*>::iterator it = warriors.begin(); it != warriors.end(); ++it) {
			(*it)->move(map, warriors);
			(*it)->attack(map, warriors);
		}

		// check if there is only one species left in warriors
		aliveEnemies = false;
		char warTag = (*(warriors.begin()))->getWarriorTag();
		for (std::list<Warrior*>::iterator it = warriors.begin(); it != warriors.end(); ++it) {
			if (warTag != (*it)->getWarriorTag()) {
				aliveEnemies = true;
				break;
			}
		}

		std::cout << "Battle Round: " << ++battleRound << std::endl;
		printMap(map);
		for (std::list<Warrior*>::iterator it = warriors.begin(); it != warriors.end(); ++it) {
			std::cout << (*it)->getWarriorTag() << ":[" << (*it)->getX() << "," << (*it)->getY() << "]HP(";
			std::cout << (*it)->getHP() << ")" << std::endl;
		}
		std::cout << "==============================" << std::endl;

		// reorder warriors according to their position in reading order
		warriors.sort(minPos);
	}

	// sum up all remainnig HP points and multiply by battlRounds fought
	long long int sumHP = 0;
	for (auto* w : warriors) {
		sumHP += w->getHP();
	}
	std::cout << "Solution Part One: " << sumHP * (battleRound) << std::endl;
	std::cout << "Solution Part One: " << sumHP * (battleRound - (long long int)1) << std::endl;

	// Free memory
	for (auto&& w:warriors) {delete w;}

	return 0;
}

/******************************
*   INPUT HELPER FUNCTIONS   *
******************************/

void printMap(const charMap& map) {

	// maps[x][y] thus 'for (auto const & row:map)' is not the way we want it

	// Just to display column numbers
	size_t nCols = map.front().size();
	size_t nRows = map.size();
	std::cout << "   ";
	for (int i = 0; i < (nCols / 10); ++i) {
		for (int j = 0; j < 10; ++j) { std::cout << i; }
	}
	for (int i = 0; i < (nCols % 10); ++i) {
		std::cout << (nCols / 10);
	}
	std::cout << std::endl << "   ";;
	for (int i = 0; i < (nCols / 10); ++i) {
		for (int j = 0; j < 10; ++j) { std::cout << j; }
	}
	for (int i = 0; i < (nCols % 10); ++i) {
		std::cout << i;
	}
	std::cout << std::endl << "   ";;
	for (int i = 0; i < nCols; ++i) {
		std::cout << "-";
	}
	std::cout << std::endl;

	
	int nLine = 0;
	for (int i = 0; i < nRows; ++i) {
		std::cout << std::setfill('0') << std::setw(2) << nLine << '|'; // Line Number
		for (int j = 0; j < nCols; ++j) {
			std::cout << map[j][i];
		}
		std::cout << std::endl;
		++nLine;
	}
}

std::string getInputAsString(std::string fileName) {
	std::string str;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
		return std::string();
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