#include "Warrior.h"

char Warrior::getWarriorTag(){	return 'W';}

// This function defines which move is next for this warrior in the map
// given these <enemy>
void Warrior::move(Map & map, std::list<Warrior*> enemy)
{
	// Check if <this> is already adjacent to enemy, if not proceed with move, otherwise go to attack
	if (isAdjacentToEnemy(map)) { 
		return;
	}

	// Get feasible target points (free but not necessarily reachable)
	std::list<Coordinate> feasible = getFeasible(map, enemy);
	if (feasible.empty()) {
		// do not move
		return;
	}
	
	// Get Reachable paths
	std::list<std::list<Coordinate>> reachable = getReachable(map, feasible);
	if (reachable.empty()) { 
		// do not move
		return;
	}
	
	// find the shortest reachable point (ties broken in reading order top2bot, left2right)
	Coordinate nextP = getNextStep(reachable);
	
	// Make Move and update Map
	setNewPosition(map, nextP);
	return;
}

// This function defines which <enemy> will be attackt now
int Warrior::attack(Map & map, std::list<Warrior*>& enemy)
{
	if (!isAdjacentToEnemy(map)) {
		return 0;
	}

	// If there are adjacent enemies, pick the one with the lowest hit points.
	// if tied, choose the one first in reading order
	std::list<Warrior*> potEnemies;
	Warrior* curEnemy;
	int dx[] = { 0,  1, 0, -1 }; // array of the fields to check
	int dy[] = { -1, 0, 1,  0 }; // 0: UP; 1: RIGHT; 2: DOWN; 3: LEFT
	int curX = this->getX();
	int curY = this->getY();
	int lowestHP = INT_MAX;
	size_t nCols = map.front().size();
	size_t nRows = map.size();

	for (int i = 0; i < 4; ++i) {
		if (curX + dx[i] >= 0 &&
			curX + dx[i] < (int)nCols &&
			curY + dy[i] >= 0 &&
			curY + dy[i] < (int)nRows) {
			// is contained in map
			if (map[(size_t)curX + dx[i]][(size_t)curY + dy[i]] == this->getEnemyTag()) {
				// find this Enemy in list and add this to the potential enemies
				size_t curSizePotEnemies = potEnemies.size();
				for (std::list<Warrior*>::iterator it = enemy.begin(); it != enemy.end(); ++it) {
					if (curX + dx[i] == (*it)->getX() && curY + dy[i] == (*it)->getY()) {
						// We found it, add it
						potEnemies.push_back(*it);
						if (lowestHP > (*it)->getHP()) {
							// store new lowest HP of enemy
							lowestHP = (*it)->getHP();
						}
					}
				}
				if (curSizePotEnemies + 1 != potEnemies.size()) {
					std::cout << "Enemy was not found in enemy list!" << std::endl;
				}
			}
		}
	}
	
	// find all potEnemies with lowest HP and choose the one which is first in reading order
	// only keep the ones with lowest HP
	for (std::list<Warrior*>::iterator it = potEnemies.begin(); it != potEnemies.end(); ) {
		if (lowestHP != (*it)->getHP()) {
			it = potEnemies.erase(it);
		}
		else {
			++it;
		}
	}
	// get the first one in reading order
	potEnemies.sort(minPos);
	curEnemy = potEnemies.front();

	// carry out attack
	attackEnemy(curEnemy);

	// Remove casulaties from battle ground:
	int returnVal = 0; //0: no casualties, 1: dead elf, 2: dead Goblin
	for (std::list<Warrior*>::iterator it = enemy.begin(); it != enemy.end(); ) {
		if ((*it)->isDead()) {
			// remove from enemy and from map
			map[(*it)->getX()][(*it)->getY()] = '.';
			if ((*it)->getWarriorTag() == 'E') { returnVal = 1; }
			else if ((*it)->getWarriorTag() == 'G') { returnVal = 2; }
			delete *it;
			it = enemy.erase(it);
		}
		else {
			++it;
		}
	}

	return returnVal;
}

// Transfer the hitPoints (damage) to the selected enemy
void Warrior::attackEnemy(Warrior * enemy)
{
	enemy->getAttacked( this->getHitPoints());
}

// Receive an attack, lower HP and remove from map if dead
void Warrior::getAttacked(int damage)
{
	this->hp -= damage;
	if (this->hp <= 0) {
		this->dead = true;
	}
}

// Check if the current Warrior is already next to an enemy
bool Warrior::isAdjacentToEnemy(const Map & map)
{
	int dx[] = { 0,  1, 0, -1 }; // array of the fields to check
	int dy[] = { -1, 0, 1,  0 }; // 0: UP; 1: RIGHT; 2: DOWN; 3: LEFT
	size_t nCols = map.front().size();
	size_t nRows = map.size();
	for (int i = 0; i < 4; ++i) {
		// Check all directions
		size_t nX = (size_t)this->getX() + dx[i];
		size_t nY = (size_t)this->getY() + dy[i];
		if (nX >= 0 && nX < nCols && nY >= 0 && nY < nRows) {
			if (map[nX][nY] == this->getEnemyTag()) {
				return true;
			}
		}
	}
	return false;
}

// This function goes over the list of Warrios 
// and returns a list of Coordinates which are possible positions to attack
// an enemy. These are just the free fields next to enemies not checking if
// they are even reachable
std::list<Coordinate> Warrior::getFeasible(const Map& map, std::list<Warrior*> enemy)
{
	

	std::list<Coordinate> feasible;
	int dx[] = {0,  1, 0, -1}; // array of the fields to check
	int dy[] = {-1, 0, 1,  0}; // 0: UP; 1: RIGHT; 2: DOWN; 3: LEFT
	size_t nCols = map.front().size();
	size_t nRows = map.size();

	for (std::list<Warrior*>::iterator it = enemy.begin(); it != enemy.end(); ++it) {
		int x = (*it)->getX();
		int y = (*it)->getY();

		if (this->getWarriorTag() == (*it)->getWarriorTag()) continue; // don't compare Elf to Elf / Goblin to Goblin
		for (int i = 0; i < 4; ++i) {
			// Check all directions
			size_t nX = (size_t)x + dx[i];
			size_t nY = (size_t)y + dy[i];
			if (nX >= 0 && nX < nCols && nY >= 0 && nY < nRows) {
				if (map[nX][nY] == '.') {
					feasible.push_back(Coordinate(x + dx[i], y + dy[i]));
				}
			}
		}
	}

	return feasible;
}

// This function runs over the list of coordinates and checks which of these 
// coordinates are reachable and not blocked by walls or enemies
//
// This is basically an implementation of a A* algorithm or something similar
std::list<std::list<Coordinate>> Warrior::getReachable(const Map & map, std::list<Coordinate> feasible)
{

	int dx[] = { 0,  1, 0, -1 }; // array of the fields to check direct neighbors
	int dy[] = { -1, 0, 1,  0 }; // 0: UP; 1: RIGHT; 2: DOWN; 3: LEFT
	int curX, curY;
	size_t nCols = map.front().size();
	size_t nRows = map.size();
	// Start Coordinats
	Coordinate start(this->x, this->y);
	
	// List of results
	std::list<std::list<Coordinate>> reachable;

	// Cycle over all potential goal locations
	for (std::list<Coordinate>::iterator it = feasible.begin(); it != feasible.end(); ++it) {

		// Goal Coordinates
		Coordinate goal(it->getX(), it->getY());

		// Set of nodes already evaluated
		std::list<Coordinate> closedSet;
		// Set of currently discovered nodes not yet evaluated
		std::list<Coordinate> openSet;
		// Most efficient prev. node for each node
		CostMap cameFrom(nCols, std::vector<int>(nRows, -1));
		// For each node the cost of getting form the start to that node
		CostMap gCost(nCols, std::vector<int>(nRows,INT_MAX));
		// For each node the total cost of getting form the start to the goal by passing this node (partly heuristic)
		CostMap fCost(nCols, std::vector<int>(nRows, INT_MAX));


		// init start node
		start.setG(0);
		start.setF(getManhattenDistance(start.getX(), start.getY(), goal.getX(), goal.getY()));
		gCost[start.getX()][start.getY()] = start.getG();
		fCost[start.getX()][start.getY()] = start.getF();
		openSet.push_back(start);

		std::list<Coordinate>::iterator cur;
		while (!openSet.empty()) {
			// get node with lowest gCost -> sort openSet according to gCost
			openSet.sort(minPosC);
			cur = openSet.begin();

			// Check if we are at goal
			if (*cur == goal) { // THIS DOES NOT WORK SO FAR
				// reconstruct path
				reachable.push_back(reconstruct_path(cameFrom, cur, start));
			}

			closedSet.push_back(*cur);
			curX = cur->getX();
			curY = cur->getY();
			openSet.erase(cur);

			// for each valid neighbor
			for (int i = 0; i < 4; ++i) {
				if (curX + dx[i] >= 0 &&
					curX + dx[i] < (int)nCols &&
					curY + dy[i] >= 0 &&
					curY + dy[i] < (int)nRows) {
					// is contained in map
					if (map[(size_t)curX + dx[i]][(size_t)curY + dy[i]] == '.') {
						//this is a feasible neighbor
						Coordinate neighbor(curX + dx[i], curY + dy[i]);
						bool isInClosedSet = false;
						// Ignore neighbors in closed set
						for (std::list<Coordinate>::iterator at = closedSet.begin(); at != closedSet.end(); ++at) {
							if (*at == neighbor) {
								//Neibhbor is in closed set
								isInClosedSet = true;
								continue;
							}
						}
						if (isInClosedSet) continue;

						// Distance from start to neighbor
						int tentCost = gCost[(size_t)curX][(size_t)curY] + 1; // Distance to neighbor is always one here
						neighbor.setG(tentCost);
						neighbor.setF(tentCost + getManhattenDistance(neighbor.getX(), neighbor.getY(), goal.getX(), goal.getY()));

						// Check if neighbor is in open set, otherwise add
						bool isNotInOpenSet = true;
						for (std::list<Coordinate>::iterator at = openSet.begin(); at != openSet.end(); ++at) {
							if (*at == neighbor) {
								//Neibhbor is in closed set
								isNotInOpenSet = false;
								break;
							}
						}
						if (isNotInOpenSet) {
							openSet.push_back(neighbor);
						}
						else if (tentCost >= gCost[(size_t)curX + dx[i]][(size_t)curY + dy[i]]) {
							continue; 
						}

						// // If we reached this part, we found the best possible way so far
						// Store where we came from: reversed neighbors direction
						cameFrom[(size_t)curX + dx[i]][(size_t)curY + dy[i]] = (i+2)%4; // assumes U-R-D-L directions
						gCost[(size_t)curX + dx[i]][(size_t)curY + dy[i]] = tentCost;
						fCost[(size_t)curX + dx[i]][(size_t)curY + dy[i]] = tentCost + 
							getManhattenDistance(neighbor.getX(), neighbor.getY(), goal.getX(), goal.getY());
					}
				}
			}
		}
	}
	// sort according to length (shortest first):
	reachable.sort(minLength);
	return reachable;
}

// This function gets a list of paths to reachable targets. It will select the path with
// the shortest distance (== length of path) and break ties (not sure how to call this rule)
Coordinate Warrior::getNextStep(std::list<std::list<Coordinate>> paths)
{

	// Only keep the paths with shortest distance (this assumes a sorted list of paths [shortest first])
	// otherwise, after finding <shortestPath> we have to go over the <paths> again.
	size_t shortestPath = INT_MAX;
	Coordinate bestTarget, startPos, nextPos;
	for (std::list<std::list<Coordinate>>::iterator it=paths.begin();it!=paths.end();){
		if (shortestPath >= it->size()) {
			shortestPath = it->size();
			++it;
		}else {
			it = paths.erase(it);
		}
	}
	
	// If there is more than one path left, choose the one with the target first in reading order
	bestTarget = Coordinate(INT_MAX, INT_MAX);
	while (paths.size() > 1) {
		for (std::list<std::list<Coordinate>>::iterator it = paths.begin(); it != paths.end();) {
			if (bestTarget > it->back()) {
				bestTarget = it->back();
				++it;
			}
			else {
				it = paths.erase(it);
			}
			if (paths.size() == 1) { break; }
		}
	}
	
	// get second element of remaining path (first is the current pos of the warrior)
	std::list<std::list<Coordinate>>::iterator it = paths.begin();
	std::list<Coordinate> bestPath = *it;
	startPos = bestPath.front();
	bestPath.pop_front();
	nextPos = bestPath.front();
	
	return nextPos;
}

// This function sets the new position of the warrior and updates the war map
void Warrior::setNewPosition(Map & map, Coordinate target)
{
	// Check that target is only one step away
	if (getManhattenDistance(this->x,this->y, target.getX(), target.getY()) > 1) {
		std::cout << "ERROR[Warrior::setNewPosition] got a invalid position to set" << std::endl;
	}
	map[this->x][this->y] = '.';
	map[target.getX()][target.getY()] = this->getWarriorTag();
	this->x = target.getX();
	this->y = target.getY();

	return;
}

// Only Stub
std::list<Warrior*> Warrior::getTargets(Map & map, std::list<Warrior*> enemy)
{
	return std::list<Warrior*>();
}

// Only Stub
Warrior * Warrior::getOpponent(std::list<Warrior*> enemy)
{
	return nullptr;
}

// DEBUG method to show the cost map for a single path
void Warrior::printCostMap(const Coordinate & goal, const Map & map, const CostMap & gCost)
{
	// Just to display column numbers
	size_t nCols = map.front().size();
	size_t nRows = map.size();
	std::cout << "Cost Map for [" << this->getX() << "," << this->getY() << "] to [";
	std::cout << goal.getX() << "," << goal.getY() << "]:" << std::endl;
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
			// capture start
			if (this->getX() == j && this->getY() == i) {
				std::cout << 'S';
			}
			//capture goal
			else if (goal.getX() == j && goal.getY() == i) {
				std::cout << 'X';
			}
			//capture target (todo)
			//show cost number
			else if (gCost[j][i] < INT_MAX){// && gCost[j][i] > 0) {
				std::cout << gCost[j][i];
			}else{
				// capture goal
				// capture target
				std::cout << map[j][i];
			}
		}
		std::cout << std::endl;
		++nLine;
	}
}


int getManhattenDistance(int sX, int sY, int gX, int gY)
{
	int dist = 0;
	dist = std::abs(gX - sX) + std::abs(gY - sY);
	return dist;
}



std::list<Coordinate> reconstruct_path(const CostMap& cameFrom, std::list<Coordinate>::iterator cur, Coordinate start) {
	int dx[] = { 0,  1, 0, -1 }; // array of the fields to check direct neighbors
	int dy[] = { -1, 0, 1,  0 }; // 0: UP; 1: RIGHT; 2: DOWN; 3: LEFT
	std::list<Coordinate> path; // path from start to goal
	Coordinate newC(cur->getX(), cur->getY());
	path.push_front(newC);

	while (newC != start) {
		// get previous step
		int dirToPrev = cameFrom[newC.getX()][newC.getY()];
		newC.setX(newC.getX() + dx[dirToPrev]);
		newC.setY(newC.getY() + dy[dirToPrev]);
		// add to list from front
		path.push_front(newC);
	}
	return path;
}

bool minLength(const std::list<Coordinate>& first, const std::list<Coordinate>& second)
{
	if (first.size() < second.size()) { return true; }
	return false;
}

bool minPos(const Warrior * first, const Warrior * second)
{
	if (first->getY() < second->getY()) { 
		return true; 
	}
	else if (first->getY() == second->getY() &&
		first->getX() < second->getX()) {
		return true;
	}
	return false;
}

// returns true for smaller cost, if equal cost it returns true if first
// is first in reading order
bool minPosC(const Coordinate & first, const Coordinate & second)
{
	if (first.getG() < second.getG()) { return true; }
	else if (first.getG() > second.getG()) { return false; }
	else {
		if (first.getY() < second.getY()) {
			return true;
		}
		else if (first.getY() == second.getY() &&
			first.getX() < second.getX()) {
			return true;
		}
		return false;
	}
}
