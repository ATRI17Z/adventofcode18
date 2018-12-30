#pragma once

#include <iostream>
#include <climits>
#include <cmath>

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
	explicit Node(const Coordinate* c);
	Node(const Node&); // Copy Constructor
	Node& operator=(Node); // Assignment operator
	Node(Node && other); // Move Constructor
	~Node();

	GridLocation getLocation() const {return this->loc; }

	ll getFcost() const;
	ll getGcost() const;
	ll getHcost() const;
	
	Tool getTool() const { return this->tool;};
	const Node* getParent() const { return this->parent; }

	Node* setNodeProperties(Node* par, ll gC, ll hC, Tool t);

	friend void swap(Node& first, Node& second);

	void printNode();

private:
	Tool tool;
	GridLocation loc;
	Node* parent;
	ll hCost; // Heuristic cost from here to goal
	ll gCost; // Cost from start to this Node

	ll getDistance(const Node* par);
	void setParent(Node* p) { this->parent = p; }
	void setGcost(ll c) { this->gCost = c; }
	void setHcost(ll h) { this->hCost = h; }
	void setTool(Tool t) { this->tool = t; }

	

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

// Operator definition for GridLocation
inline bool operator==(const GridLocation& lhs, const GridLocation& rhs) {
	// return true if lhs and rhs have same distance
	if (lhs.x == rhs.x &&
		lhs.y == rhs.y) return true;
	return false;
}
inline bool operator!=(const GridLocation& lhs, const GridLocation& rhs) { return !(lhs == rhs); }


