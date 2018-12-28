#include "Node.h"

Node::Node() : tool(torch), parent(nullptr), hCost(0), gCost(LLONG_MAX) {
	this->loc.x = 0;
	this->loc.y = 0;
}

Node::Node(const Coordinate* c): hCost(0), gCost(LLONG_MAX)
{
	this->tool = Tool(c->getType());
	this->loc.x = c->getX();
	this->loc.y = c->getY();
	this->parent = nullptr;
}

ll Node::getFcost() const
{
	return this->hCost + this->gCost;
}

ll Node::getGcost() const
{
	return this->gCost;
}


