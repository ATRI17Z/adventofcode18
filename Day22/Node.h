#pragma once

#include <climits>

#include "Coordinate.h"

enum Tool {
	torch,
	climbingGear,
	neither,
};

struct GridLocation
{
	size_t x, y;
};

class Node
{
public:
	Node();
	Node(const Coordinate* c);
	~Node() {}

	GridLocation getLocation() const {return this->loc; }

	ll getFcost() const;
	ll getGcost() const;
	
	void setGcost(ll c) { this->gCost = c; }

	Tool getTool() const { return this->tool;};
	void setTool(Tool t) { this->tool = t; }

	void setParent(Node* p) { this->parent = p; }
	Node getParent() { return *(this->parent); }

	


private:
	Tool tool;
	GridLocation loc;
	Node* parent;
	ll hCost; // Heuristic cost from here to goal
	ll gCost; // Cost from start to this Node
	
};


// Operator definition for Class Type Node
inline bool operator==(const Node& lhs, const Node& rhs) {
	// return true if lhs and rhs have same distance
	if (lhs.getLocation().x == rhs.getLocation().x &&
		lhs.getLocation().y == rhs.getLocation().y &&
		lhs.getTool() == rhs.getTool()) return true;
	return false;
}
inline bool operator!=(const Node& lhs, const Node& rhs) { return !(lhs == rhs); }

// Operator definition for Class Type Node (needed by std::map)
inline bool operator< (const Node& lhs, const Node& rhs) {
	// returns true if lhs has longer distance
	if (lhs.getGcost() < rhs.getGcost()) return true;
	else return false;
}
inline bool operator> (const Node& lhs, const Node& rhs) { return rhs < lhs; }
inline bool operator<=(const Node& lhs, const Node& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Node& lhs, const Node& rhs) { return !(lhs < rhs); }


// Operator definition for GridLocation
inline bool operator==(const GridLocation& lhs, const GridLocation& rhs) {
	// return true if lhs and rhs have same distance
	if (lhs.x == rhs.x &&
		lhs.y == rhs.y) return true;
	return false;
}
inline bool operator!=(const GridLocation& lhs, const GridLocation& rhs) { return !(lhs == rhs); }
/*
// Operator definition for Class Type Node (needed by std::map)
inline bool operator< (const GridLocation& lhs, const GridLocation& rhs) {
	// returns true if lhs has longer distance
	if (lhs.x < rhs.y) { return true; }
	else if (lhs.x == rhs.x && lhs.y < rhs.y) { return true; }
	else { return false; }
}
inline bool operator> (const GridLocation& lhs, const GridLocation& rhs) { return rhs < lhs; }
inline bool operator<=(const GridLocation& lhs, const GridLocation& rhs) { return !(lhs > rhs); }
inline bool operator>=(const GridLocation& lhs, const GridLocation& rhs) { return !(lhs < rhs); }
*/

/*


bool lowestRank(const Node& lhs, const Node& rhs) {
	if (lhs.getFcost() < rhs.getFcost()) return true;
	else return false;

}
*/
