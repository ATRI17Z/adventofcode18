#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <array>
#include <algorithm>
#include <cmath>

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

//void removeElementFromList(Node, std::list<Node>&);
bool listContains(Node, std::list<Node>&);
std::list<Node> getFeasibleChildren(Node&, std::list<Node>&, std::list<Node>&, const std::vector<std::vector<Coordinate>>&);
//Tool getCommonTool(const Node, const Node, const std::vector<std::vector<Coordinate>>&);
//int getStepCost(const Node, const Node);
ll estimateHScore(Node&, const Node&, const std::vector<std::vector<Coordinate>>&);
//bool lowestFcost(const Node&, const Node&);
bool smallerDistance(const Node&, const Node&);
//bool isFeasibleTransition(const Node, const Coordinate&);
bool isValidTool(Tool, Coordinate);
std::string toolStr(Tool);
std::string typeStr(int);


void printMap(const std::vector<std::vector<Coordinate>>&, size_t , size_t);

int main() {

	// INPUT Day17
	//depth: 3066
	//target : 13, 726
	int depth = 3066;
	int xTarget = 13;
	int yTarget = 726;
	size_t nCols = 1000;
	size_t nRows = 2000;
	Coordinate* target = new Coordinate(xTarget, yTarget,0 );

	// Test Input
	//depth = 510;
	//xTarget = 10;
	//yTarget = 10;
	//nCols = 16+1;
	//nRows = 16+1;
	//target = new Coordinate(10, 10,0);
	target->setIsTarget(true);

	Coordinate* mouth = new Coordinate(0, 0, 0);

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
	
	//printMap(map, xTarget, yTarget);

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
	Node goalNode;
	Node start(mouth);
	Node goal(target);
	goal.setNodeProperties(nullptr, LLONG_MAX, 0, torch);
	start.setNodeProperties(&start, 0, estimateHScore(start, goal, map), torch);
	Node cur;

	open.push_back(start);

	//while (open.front() != goal) {
	while (!open.empty()) {
		cur = open.front();		// Assign the lowest rank member of <open> to <cur>
		open.pop_front();		// Remove the lowest rank member from the OPEN list
		closed.push_back(cur);	// Add the lowest rank member <cur> to CLOSED list

		if (cur.getLocation().x == goal.getLocation().x &&
			cur.getLocation().y == goal.getLocation().y &&
			cur.getTool() == torch) {
			// Store goal node for path reconstruction
			goal = cur;
			std::cout << "Reached Goal Node: " << cur.getGcost() << std::endl;
			break;
		}

		// Get feasible neibhbors (UP,RIGH,DOWN,LEFT, but not at neg. location and with valid transition)
		neighbor = getFeasibleChildren(cur, open, closed, map);
		for (std::list<Node>::iterator it = neighbor.begin(); it != neighbor.end(); ++it) {
			open.push_back(*it);
		}
		// Sort according to estimated total cost
		open.sort(smallerDistance);
	}

	//testFeasibleTransition();
	std::cout << "Cost so far of 'Start': " << start.getGcost() << std::endl;
	std::cout << "Cost of goal element: " << goal.getGcost() << std::endl;
	
	std::vector<Node> optimalPath;
	Node parent = goal;
	parent.printNode();
	optimalPath.push_back(parent);
	
	size_t goalX = start.getLocation().x;
	size_t goalY = start.getLocation().y;
	
	std::cout << "Best path found so far (" << optimalPath.size() << "): ";
	while ((parent.getLocation().x != goalX) || (parent.getLocation().y != goalY)) {
		std::cout << "[" << parent.getLocation().x << "," << parent.getLocation().y << "]-";
		if (parent.getParent() == nullptr) { std::cout << " Optimal Path not restoreable" << std::endl; }
		parent = *parent.getParent();
		optimalPath.push_back(parent);
	}
	
	// Part 2: 990

	// free memory
	delete target, mouth;
	return 0;
}

// Check if Node cur is element of list open
bool listContains(Node cur, std::list<Node>& list) {
	std::list<Node>::iterator findIter = std::find(list.begin(), list.end(), cur);
	if (findIter == list.end()) {
		//std::cout << "Element [" << cur.getLocation().x << "," << cur.getLocation().y << "] NOT found in list" << std::endl;
		return false;
	}
	else {
		//std::cout << "Element [" << cur.getLocation().x << "," << cur.getLocation().y << "] FOUND in list" << std::endl;
		return true;
	}
}

// Get feasible Children for current Node <cur> in Map <map>
// TODO: Make sure we don't go backwards (should be ok, by design)
std::list<Node> getFeasibleChildren(Node& cur, std::list<Node>& open, std::list<Node>& closed, const std::vector<std::vector<Coordinate>>& map) {

	std::list<Node> children;
	Node child;
	int nCols = (int)map.size();
	int nRows = (int)map.front().size();

	// Get Nodes on same location as cur but with different tool
	for (int i = 0; i < 3; ++i) {
		if (isValidTool(Tool(i), map[cur.getLocation().x][cur.getLocation().y]) && cur.getTool() != Tool(i)) {
			child = Node(&map[cur.getLocation().x][cur.getLocation().y]);
			child.setNodeProperties(&cur, cur.getGcost() + 7, LLONG_MAX, Tool(i));
			if (!(listContains(child, open)) && !(listContains(child, closed))) {
				children.push_back(child);
				//std::cout << "Added Node [" << cur.getLocation().x << "," << cur.getLocation().y << "] with new Tool " << toolStr(Tool(i)) << std::endl;
			}
			
		}
	}
	
	// travel directions: 0:UP, 1:RIGHT, 2:DOWN, 3:LEFT
	int dx[] = {  0, 1, 0, -1 };
	int dy[] = { -1, 0, 1,  0 };
	int newX, newY;
	
	// Check all four direcitions around OPEN wich can be reached with same tool
	for (int i = 0; i < 4; ++i) {
		newX = (int)cur.getLocation().x + dx[i];
		newY = (int)cur.getLocation().y + dy[i];

		// not in solid ground or outside map
		if (!(newX >= 0 && newY >= 0 && newX < nCols && newY < nRows)) { continue; }
		if (isValidTool(cur.getTool(), map[newX][newY])) {
			child = Node(&map[newX][newY]);
			child.setNodeProperties(&cur, cur.getGcost() + 1, LLONG_MAX, cur.getTool());
			if (!(listContains(child, open)) && !(listContains(child, closed))) {
				children.push_back(child);
				//std::cout << "Added new Node [" << newX << "," << newY << "] with Tool " << toolStr(cur.getTool()) << std::endl;
			}
			
		}

	}
	//std::cout << std::endl;

	return children;
}

// Get the tool which is common to the current and next node
// Valid type transitions:
//		- 0: torch				allows	rocky(0)[.] <-> narrow(2)[|]
//		- 1: climbing gear		allows	rocky(0)[.] <-> wet(1)[=]
//		- 2: neither			allows	wet(1)[=]   <-> narrow(2)[|]
bool isValidTool(Tool tool, Coordinate curC) {
	if (curC.getType() == 0 && (tool == 0 || tool == 1)) return true;
	if (curC.getType() == 1 && (tool == 1 || tool == 2)) return true;
	if (curC.getType() == 2 && (tool == 2 || tool == 0)) return true;
	return false;
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


// Estimate cost from current location <node> to <target>
ll estimateHScore(Node& node, const Node& target, const std::vector<std::vector<Coordinate>>& map) {
	size_t deltaX = target.getLocation().x - node.getLocation().x;
	size_t deltaY = target.getLocation().y - node.getLocation().y;
	ll fScore = 0;
	
	// Simple Manhatten Distance so far
	fScore = std::abs((ll)deltaX) + std::abs((ll)deltaY);

	return fScore;
}

bool smallerDistance(const Node& lhs, const Node& rhs) {
	if (lhs.getGcost() < rhs.getGcost()) return true;
	else return false;
}

