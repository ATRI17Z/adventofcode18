#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <array>
#include <algorithm>

#include "Coordinate.h"
#include "Node.h"


/*
 - region divided into squares (type: rocky, narrow, wet) of X,Y coordinates (starting at 0,0)
 - type defined by erosion level
 - erosion level defined by geological index
 - geolocial index derived from 5 rules (the first applicable counts)
		1) (0,0) has geological index of 0
		2) target coordinate has geological index 0
		3) If Y=0 -> geoIdx = X*16807
		4) If X=0 -> geoIdx = Y*48271
		5) Else: geoIdx = erosionLvl(X-1,Y) * erosionLvl(X,Y-1)
- Erosion level = (geoIdx*depth)%20183
- Region type = erosionLvl % 3 (where 0:rocky, 1:wet, 2:narrow)

*/

void removeElementFromList(Node, std::list<Node>&);
bool listContains(Node, std::list<Node>);
std::list<Node> getFeasibleChildren(const Node, const Node, const std::vector<std::vector<Coordinate>>&);
Tool getCommonTool(const Node, const Node, const std::vector<std::vector<Coordinate>>&);
int getStepCost(const Node, const Node);
ll estimateFScore(Coordinate*, Coordinate*,const std::vector<std::vector<Coordinate>>&);
bool isFeasibleTransition(const Node, const Coordinate&);
std::string toolStr(Tool);
std::string typeStr(int);

// Test Methods
void testFeasibleTransition();

void printMap(const std::vector<std::vector<Coordinate>>&, size_t , size_t);
void printCostMap(std::vector<std::vector<ll>> cost_so_far, const std::vector<std::vector<Coordinate>>&, std::vector<GridLocation>&);

int main() {

	// INPUT Day17
	int depth = 3066;
	
	int xTarget = 13;
	int yTarget = 726;
	size_t nCols = 26 + 1;
	size_t nRows = 1452 + 1;
	Coordinate* target = new Coordinate(xTarget, yTarget,0 );

	// Test Input
	depth = 510;
	xTarget = 10;
	yTarget = 10;
	nCols = 16+1;
	nRows = 16+1;
	target = new Coordinate(10, 10,0);
	target->setIsTarget(true);

	Coordinate* mouth = new Coordinate(0, 0,0);

	// Map
	std::vector<std::vector<Coordinate>> map(nCols, std::vector<Coordinate>(nRows, Coordinate(0, 0)));
	// Init Map
	ll risk = 0;
	map[0][0] = *mouth;
	map[xTarget][yTarget] = *target;
	// Assign Type to borders
	for (int xC = 1; xC < nCols; ++xC) {
		map[xC][0] = Coordinate(xC, 0, (ll)xC * 16807);
		if (xC <= xTarget) risk += map[xC][0].getType();// add risk
	}
	for (int yC = 1; yC < nRows; ++yC) {
		map[0][yC] = Coordinate(0, yC, (ll)yC * 48271);
		if (yC <= yTarget) risk += map[0][yC].getType();// add risk
	}

	// Fill map
	size_t s, xC, yC;
	ll gIdx;
	
	for (s = 2; s < (nCols + nRows - 2); ++s) {
		for (xC = 1; xC < nCols; ++xC) {
			yC = s - xC;
			if (yC < 1 || yC >= nRows) continue;
			//std::cout << "(" << xC << "," << yC << ")";
			if (xC != xTarget || yC != yTarget) { // do not redefine type of target with non-zero geoIdx
				gIdx = map[xC - 1][yC].getELvl() * map[xC][yC - 1].getELvl();
				map[xC][yC] = Coordinate((int)xC, (int)yC, gIdx);
			}
			if (xC <= xTarget && yC <= yTarget) risk += map[xC][yC].getType(); // add risk
		}
		//std::cout << std::endl;
	}
	
	printMap(map, xTarget, yTarget);

	// Show resulting risk (mouth and target have risk 0)
	std::cout << "P1: Risk: " << risk << std::endl; // solution: 10115
	
	//////////////////////////////////////////////////////////////////
	////////////////////////// PART TWO: /////////////////////////////
	//////////////////////////////////////////////////////////////////
	// - two tools, can use, one of them or none (torch, climbing gear)
	//		- rocky: need to use torch or climbing
	//		- wet: need to use climbing gear or neither
	//		- narrow: need to use torch or neither
	//
	// - start at mouth with torch
	// - can move up, down, left, right
	// - one step: 1 Minute
	// - switching tools: 7 Minutes
	// - at target u need the torch
	// - valid type transitions:
	//		- 0: torch				allows	rocky(0)[.] <-> narrow(2)[|]
	//		- 1: climbing gear		allows	rocky(0)[.] <-> wet(1)[=]
	//		- 2: neither			allows	wet(1)[=]   <-> narrow(2)[|]
	


	
	/////////////////////////////////////////////////////////////////////////////////////////
	// A* Method
	// ================
	// g(n): exact cost from start to node n
	// h(n): estimated cost from node to target
	//       * If h(n) is always <= the cost of moving from n to the goal,
	//		   then A* is guaranteed to find a shortest path. 
	//		   The lower h(n) is, the more node A* expands, making it slower.
	//		 * If h(n) is sometimes > the cost of moving from n to the goal,
	//		   then A* is NOT guaranteed to find a shortest path, but it can run faster.
	//
	// Algo Outline:
	//	OPEN = priority queue containing START
	//	CLOSED = empty set
	//	while lowest rank in OPEN is not the GOAL:
	//		current = remove lowest rank item from OPEN
	//		add current to CLOSED
	//		for neighbors of current:
	//			cost = g(current) + movementcost(current, neighbor)
	//			if neighbor in OPEN and cost less than g(neighbor):
	//				remove neighbor from OPEN, because new path is better
	//			if neighbor in CLOSED and cost less than g(neighbor):
	//				remove neighbor from CLOSED
	//			if neighbor not in OPEN and neighbor not in CLOSED:
	//				set g(neighbor) to cost
	//				add neighbor to OPEN
	//				set priority queue rank to g(neighbor) + h(neighbor)
	//				set neighbor's parent to current
	//
	//	reconstruct reverse path from goal to start
	//	by following parent pointers
	/////////////////////////////////////////////////////////////////////////////////////////

	std::list<Node> open, closed, neighbor;
	std::vector<std::vector<GridLocation>> came_from(nCols, std::vector<GridLocation>(nRows, GridLocation()));
	std::vector<std::vector<ll>> cost_so_far(nCols, std::vector<ll>(nRows, LLONG_MAX));
	//std::map<GridLocation, GridLocation> came_from; // Issues with comparator leads to use vector class
	//std::map<GridLocation, ll> cost_so_far;
	Node start(mouth);
	start.setGcost(0);
	came_from[start.getLocation().x][start.getLocation().x] = start.getLocation();
	cost_so_far[start.getLocation().x][start.getLocation().x] = 0;
	Node goal(target);
	Node cur;

	open.push_back(start); // DEBUG, should be start!!!!!!!!!!!! 

	//while (open.front() != goal) {
	while (!open.empty()) {
		cur = open.front();		// Assign the lowest rank member of <open> to <cur>
		open.pop_front();		// Remove the lowest rank member from the OPEN list
		closed.push_back(cur);	// Add the lowest rank member <cur> to CLOSED list

		// Get feasible neibhbors (UP,RIGH,DOWN,LEFT, but not at neg. location and with valid transition)
		neighbor = getFeasibleChildren(cur, goal, map);
		for (std::list<Node>::iterator it = neighbor.begin(); it != neighbor.end(); ++it) {
			// Calculate cost from START to current neighbor node: g(cur)+move2Cost(cur,neighbor)
			ll neighborCost = cost_so_far[cur.getLocation().x][cur.getLocation().y] + getStepCost(cur, *it);
			if (listContains(*it, open) && neighborCost < cost_so_far[it->getLocation().x][it->getLocation().y]) {
				std::cout << "Neighbor[" << it->getLocation().x << "," << it->getLocation().y << "] ist element of OPEN and has smaller cost than before at this Node" << "(" << neighborCost << ")/(" << cost_so_far[it->getLocation().x][it->getLocation().y] << ")" << std::endl;
				removeElementFromList(*it, open);
				std::cout << std::endl;
			}
			if (listContains(*it, closed) && neighborCost < cost_so_far[it->getLocation().x][it->getLocation().y]) {
				std::cout << "Neighbor[" << it->getLocation().x << "," << it->getLocation().y << "] ist element of CLOSED and has smaller cost than before at this Node" << "(" << neighborCost << ")/(" << cost_so_far[it->getLocation().x][it->getLocation().y] << ")" << std::endl;
				removeElementFromList(*it, closed);
			}
			if (!listContains(*it, open) && !listContains(*it, closed)) {
				std::cout << "Adding " << "[" << it->getLocation().x << "," << it->getLocation().y << "] " << std::endl;
				cost_so_far[it->getLocation().x][it->getLocation().y] = neighborCost;
				it->setGcost(neighborCost);
				it->setParent(&cur);
				std::cout << "Added " << "[" << it->getLocation().x << "," << it->getLocation().y << "]: " << it->getGcost() << std::endl;
				//priority = neighborCost + costToEnd();
				open.push_back(*it);
				came_from[it->getLocation().x][it->getLocation().y] = cur.getLocation();
			}
			else {
				std::cout << "Already contained " << "[" << it->getLocation().x << "," << it->getLocation().y << "] " << "(" << neighborCost << ")/(" << cost_so_far[it->getLocation().x][it->getLocation().y] << ")" << std::endl;

			}

		}
		std::cout << "OPEN: ";
		for (auto o : open) {
			std::cout << "[" << o.getLocation().x << "," << o.getLocation().y << "] ";
		}
		std::cout << std::endl << "========================================" << std::endl;
	}

	//testFeasibleTransition();
	std::cout << "Cost so far of 'Start': " << cost_so_far[start.getLocation().x][start.getLocation().y] << std::endl;
	std::cout << "Cost of unknown element: " << cost_so_far[cur.getLocation().x][cur.getLocation().y] << std::endl;
	std::cout << "Cost of goal element: " << cost_so_far[goal.getLocation().x][goal.getLocation().y] << std::endl;
	
	
	// reconstruct path:
	std::vector<GridLocation> exampleOptimalPath;
	GridLocation newLoc;
	std::array<size_t,25> xArr = { 10,10,10, 9, 8, 7, 6, 6, 5, 5,5,5,4,4,4,4,4,4,4,4,3,2,1,0,0 };
	std::array<size_t,25> yArr = { 10,11,12,12,12,12,12,11,11,10,9,8,8,7,6,5,4,3,2,1,1,1,1,1,0 };
	std::cout << "Example Solution      : ";
	for (int i = 0; i < xArr.size(); ++i) {
		newLoc.x = xArr[i]; newLoc.y = yArr[i];
		exampleOptimalPath.push_back(newLoc);
		std::cout << "[" << newLoc.x << "," << newLoc.y << "]-";
	}
	std::cout << std::endl;
	
	std::vector<GridLocation> optimalPath;
	GridLocation parent = goal.getLocation();
	optimalPath.push_back(parent);
	std::cout << "Best path found so far: ";
	size_t goalX = start.getLocation().x;
	size_t goalY = start.getLocation().y;

	parent = came_from[7][3];
	std::cout << "[" << parent.x << "," << parent.y << "]-";
	parent = came_from[8][3];
	std::cout << "[" << parent.x << "," << parent.y << "]-";

	//while ((parent.x != goalX) || (parent.y != goalY)) {
	//	std::cout << "[" << parent.x << "," << parent.y << "]-";
	//	parent = came_from[parent.x][parent.y];
	//	optimalPath.push_back(parent);
	//}
	std::cout << std::endl;
	std::cout << "========================================" << std::endl;
	printCostMap(cost_so_far, map, optimalPath);
	std::cout << "Exampel Solution" << std::endl;
	std::cout << "========================================" << std::endl;
	printCostMap(cost_so_far, map, exampleOptimalPath);


	return 0;
}

// Remove <cur> from list <list>
void removeElementFromList(Node cur, std::list<Node>& list) {
	std::list<Node>::iterator findIter = std::find(list.begin(), list.end(), cur);
	if (findIter != list.end()) {
		findIter = list.erase(findIter);
	}
	else {
		std::cout << "Could not find [" << cur.getLocation().x << "," << cur.getLocation().y << "] in OPEN for removal!" << std::endl;
	}
}

// Check if Node cur is element of list open
bool listContains(Node cur, std::list<Node> open) {
	std::list<Node>::iterator findIter = std::find(open.begin(), open.end(), cur);
	if (findIter == open.end()) {
		return false;
	}
	else {
		return true;
	}
}

// Get feasible Children for current Node <cur> in Map <map>
// TODO: Make sure we don't go backwards (should be ok, by design)
std::list<Node> getFeasibleChildren(const Node cur, const Node goal, const std::vector<std::vector<Coordinate>>& map) {

	std::list<Node> children;
	Node child;
	int nCols = (int)map.size();
	int nRows = (int)map.front().size();
	
	// travel directions: 0:UP, 1:RIGHT, 2:DOWN, 3:LEFT
	int dx[] = {  0, 1, 0, -1 };
	int dy[] = { -1, 0, 1,  0 };
	int newX, newY;
	
	// Check all four direcitions around OPEN
	std::cout << "New Feasbile Nodes for [" << cur.getLocation().x << "," << cur.getLocation().y << "](t:";
	std::cout << toolStr(cur.getTool()) << "): ";
	for (int i = 0; i < 4; ++i) {
		newX = (int)cur.getLocation().x + dx[i];
		newY = (int)cur.getLocation().y + dy[i];

		if (newX >= 0 && newY >= 0) {// not in solid ground
			if (newX >= nCols || newY >= nRows) { // MAP maybe TOO SMALL
				std::cout << "MAP IS MAYBE TOO SMALL" << std::endl;
				continue;
			}
			
			child = Node(&map[newX][newY]);
			if (child == goal) {
				std::cout << "CHILD IS GOAL" << std::endl;
				child.setTool(torch);
			}
			else {
				child.setTool(getCommonTool(cur, child, map));
			}
			
			children.push_back(child);
			std::cout << "{" << newX << "," << newY << "}(t:" << toolStr(child.getTool()) << ") ";
		}
	}
	std::cout << std::endl;

	return children;
}

// Valid type transitions:
//		- 0: torch				allows	rocky(0)[.] <-> narrow(2)[|]
//		- 1: climbing gear		allows	rocky(0)[.] <-> wet(1)[=]
//		- 2: neither			allows	wet(1)[=]   <-> narrow(2)[|]
void testFeasibleTransition() {
	Node cur = Node();
	Coordinate next;
	
	for (int i = 0; i < 3; ++i) {
		next.setType(i);			// rock: 0, wet: 1, narrow: 2,
		cur.setTool(torch);
		if (isFeasibleTransition(cur, next)) {
			std::cout << "Transition to " << typeStr(i) << " with torch is possible" << std::endl;
		}
		cur.setTool(climbingGear);
		if (isFeasibleTransition(cur, next)) {
			std::cout << "Transition to " << typeStr(i) << " with climbing gear is possible" << std::endl;
		}
		cur.setTool(neither);
		if (isFeasibleTransition(cur, next)) {
			std::cout << "Transition to " << typeStr(i) << " with neither is possible" << std::endl;
		}
	}
	

}

bool isFeasibleTransition(const Node cur, const Coordinate& next) {
	if (cur.getTool() == next.getType() || (cur.getTool() + 2) % 3 == next.getType()) {
		return true;
	}
	else {
		std::cout << "Cannot enter " << typeStr(next.getType()) << " with " << toolStr(cur.getTool()) << std::endl;
		return false;
	}
}

// Get the tool which is common to the current and next node
// Valid type transitions:
//		- 0: torch				allows	rocky(0)[.] <-> narrow(2)[|]
//		- 1: climbing gear		allows	rocky(0)[.] <-> wet(1)[=]
//		- 2: neither			allows	wet(1)[=]   <-> narrow(2)[|]
Tool getCommonTool(const Node curNode, const Node nextNode, const std::vector<std::vector<Coordinate>>& map) {
	Coordinate curC = map[curNode.getLocation().x][curNode.getLocation().y];
	Coordinate nxtC = map[nextNode.getLocation().x][nextNode.getLocation().y];
	int curType = curC.getType();
	int nextType = nxtC.getType();
	if (curType == nextType) return curNode.getTool();
	else if ((nextType == 0 || nextType == 2) && (curType == 0 || curType == 2)) { return torch; }// torch
	else if ((nextType == 0 || nextType == 1) && (curType == 0 || curType == 1)) { return climbingGear; }// climbingGear
	else if ((nextType == 1 || nextType == 2) && (curType == 1 || curType == 2)) { return neither; }// neither
	else {
		std::cout << "Sth went wrong with your tools!" << std::endl;
	}				// Case which should not happen
	return torch;
}
Tool getCommonTool(Tool curT, const  Coordinate curNode, const Coordinate nextNode, const std::vector<std::vector<Coordinate>>& map) {
	int curType = curNode.getType();
	int nextType = nextNode.getType();
	if (curType == nextType) return curT;
	else if ((nextType == 0 || nextType == 2) && (curType == 0 || curType == 2)) { return torch; }// torch
	else if ((nextType == 0 || nextType == 1) && (curType == 0 || curType == 1)) { return climbingGear; }// climbingGear
	else if ((nextType == 1 || nextType == 2) && (curType == 1 || curType == 2)) { return neither; }// neither
	else {
		std::cout << "Sth went wrong with your tools!" << std::endl;
	}				// Case which should not happen
	return torch;
}

// Calculate the step distance (time) between two adjacent nodes
int getStepCost(const Node cur, const Node next) {
	//		- 0: torch				allows	rocky(0) and narrow(2)
	//		- 1: climbing gear		allows	rocky(0) and wet(1)
	//		- 2: neither			allows	wet(1) and narrow(2)
	//std::cout << "Cost from " << toolStr(cur.getTool()) << " to " << toolStr(next.getTool()) << " is ";
	if (cur.getTool() == next.getTool()) {
		//std::cout << "1" << std::endl;
		return 1;
	}
	else {
		//std::cout << "1+7" << std::endl;
		return 1 + 7;
	}
}


// Return matching string for tool integer
//		- 0: torch				allows	rocky(0) and narrow(2)
//		- 1: climbing gear		allows	rocky(0) and wet(1)
//		- 2: neither			allows	wet(1) and narrow(2)
std::string toolStr(Tool tool) {
	if (tool == torch) return "torch";
	else if (tool == climbingGear) return "climbing gear";
	else if (tool == neither) return "neither";
	else return "undefined tool code";
}

// Return matching string for ground type integer
//		- 0: torch				allows	rocky(0) and narrow(2)
//		- 1: climbing gear		allows	rocky(0) and wet(1)
//		- 2: neither			allows	wet(1) and narrow(2)
std::string typeStr(int type) {
	if (type == 0) return "rocky";
	else if (type == 1) return "wet";
	else if (type == 2) return "narrow";
	else return "undefined type code";
}

void printMap(const std::vector<std::vector<Coordinate>>& map,size_t tX, size_t tY) {
	int nCols = (int)map.size();
	int nRows = (int)map.front().size();
	for (int i = 0; i < nRows; ++i) {
		std::cout << i << ":\t";
		for (int j = 0; j < nCols; ++j) {
			if (i == 0 && j == 0) {
				std::cout << "M";
			}
			else if (i == tY && j == tX) {
				std::cout << "T"; 
			}
			else if (map[j][i].getType() == 0) { // rocky
				std::cout << ".";
			}
			else if (map[j][i].getType() == 1) { // wet
				std::cout << "=";
			}
			else if (map[j][i].getType() == 2) { // narrow
				std::cout << "|";
			}
			else { // undef
				// do nothing
				std::cout << "*";
			}
		}
		std::cout << std::endl;
	}

}

void printCostMap(std::vector<std::vector<ll>> cost_so_far,
	              const std::vector<std::vector<Coordinate>>& map,
				  std::vector<GridLocation>& optiPath)
{
	int nCols = (int)map.size();
	int nRows = (int)map.front().size();

	
	if (optiPath.empty()) {
		std::cout << "y\\x:\t";
		for (int i = 0; i < nCols; ++i) {
			std::cout << std::setw(3) << i << " ";
		}
		std::cout << std::endl;
		for (int i = 0; i < nRows; ++i) {
			std::cout << std::setw(3) << i << ":\t";
			for (int j = 0; j < nCols; ++j) {
				std::cout << std::setw(3) << cost_so_far[j][i] << " ";
			}
			std::cout << std::endl;
		}
	}
	else {
		std::cout << "y\\x:\t";
		for (int i = 0; i < nCols; ++i) {
			std::cout << " " << std::setw(3) << i << "  ";
		}
		std::cout << std::endl;
		for (int i = 0; i < nRows; ++i) {
			std::cout << std::setw(3) << i << ":\t";
			for (int j = 0; j < nCols; ++j) {
				GridLocation pathPoint;
				pathPoint.x = j; pathPoint.y = i;
				std::vector<GridLocation>::iterator it = std::find(optiPath.begin(), optiPath.end(), pathPoint);
				if (it != optiPath.end()) {
					std::cout << "[" << typeStr(map[j][i].getType())[0] << std::setw(3) << cost_so_far[j][i] << "]" << " ";
				}
				else {
					std::cout  << " " << typeStr(map[j][i].getType())[0] << std::setw(3) << cost_so_far[j][i] << "  ";
				}
				
			}
			std::cout << std::endl;
		}
	}
	std::cout << "========================================" << std::endl;
}

/*
// Estimate some distance from current location <node> to <target>
ll estimateFScore(Coordinate* node, Coordinate* target, const std::vector<std::vector<Coordinate>>& map) {
	size_t deltaX = target->getX() - node->getX();
	size_t deltaY = target->getY() - node->getY();
	ll fScore = 0;
	int xN, yN;

	// Create list of points to visit towards target
	std::vector<int> x, y;
	if (deltaX > 0) {
		for (int i = 0; i <= deltaX; ++i) {
			x.push_back((int)node->getX() + i);
			y.push_back((int)node->getY());
		}
	}
	else {
		for (int i = 0; i > deltaX; --i) {
			x.push_back((int)node->getX() + i);
			y.push_back((int)node->getY());
		}
	}
	xN = x.back();
	yN = y.back();
	if (deltaY > 0) {
		for (int i = 0; i <= deltaY; ++i) {
			x.push_back(xN);
			y.push_back(yN + i);
		}
	}
	else {
		for (int i = 0; i > deltaY; --i) {
			x.push_back(xN);
			y.push_back(yN + i);
		}
	}

	Coordinate cur, next;
	for (int i = 0; i < x.size()-1; ++i) {
		cur = map[(size_t)x[i]][(size_t)y[i]];
		next = map[(size_t)x[(size_t)i + 1]][(size_t)y[(size_t)i + 1]];
		fScore += getStepCost(&cur, &next);
		std::cout << "Going from [" << x[(size_t)i] << "," << y[(size_t)i] << "](";
		std::cout << typeStr(cur.getType()) << "," << toolStr(cur.getTool()) << ") -> [";
		std::cout << x[(size_t)i + 1] << "," << y[(size_t)i + 1] << "](";
		std::cout << typeStr(next.getType()) << "," << toolStr(next.getTool()) << ")with total cost ";
		std::cout << fScore << std::endl;
	}
	return fScore;
}

*/