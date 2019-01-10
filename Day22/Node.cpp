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
// Copy Constructor
Node::Node(const Node & elem)
{
	this->tool = elem.getTool();
	this->loc = elem.getLocation();
	this->hCost = elem.getHcost();
	this->gCost = elem.getGcost();

	if (elem.getParent() != nullptr) {
		this->parent = new Node(&Coordinate((int)elem.getParent()->getLocation().x, (int)elem.getParent()->getLocation().y));
	}else {
		this->parent = nullptr;
	}
}

// Assignment operator
Node & Node::operator=(Node other)
{
	swap(*this, other);
	return *this;
	//std::cout << " Assignment Operator used " << std::endl;
}

// Move Constructor
Node::Node(Node && other)
{
	// Manually default construct class
	this->parent = nullptr;
	this->tool = torch;
	this->loc.x = 0;
	this->loc.y = 0;
	this->hCost = 0;
	this->gCost = 0;
	// swap
	swap(*this, other);
	//std::cout << " Move Constructor used " << std::endl;
}

// Destructor
Node::~Node()
{

}

ll Node::getFcost() const
{
	return this->hCost + this->gCost;
}

ll Node::getGcost() const
{
	return this->gCost;
}

ll Node::getHcost() const
{
	return this->hCost;
}

Node* Node::setNodeProperties(Node * par, ll gC, ll hC, Tool t)
{
	// Check if it is a feasible parent
	if (par == nullptr) {
		this->setParent(nullptr);
	}else{
		this->setParent(par);
	}

	
	// Set gCost
	this->setGcost(gC);

	// Set estimated hCost
	this->setHcost(hC);

	// Set given Tool
	this->setTool(t);

	return this;
}

void Node::printNode()
{
	/********************
	Loc: [x,y]
	Par: [x,y]
	gC:  ll
	hC:  ll
	Tool: ...
	*/
	std::cout << "**************************" << std::endl;
	std::cout << "Loc: [" << this->loc.x << "," << this->loc.y << "]" << std::endl;
	if (this->parent != nullptr) {
		std::cout << "Par: [" << this->parent->loc.x << "," << this->parent->loc.y << "]" << std::endl;
	}
	else {
		std::cout << "Par: [-]" << std::endl;
	}
	std::cout << "gC:  [" << this->gCost << "]" << std::endl;
	std::cout << "hC:  [" << this->hCost << "]" << std::endl;
	std::cout << "Tool: " << this->tool << std::endl;
	std::cout << "**************************" << std::endl;

}

ll Node::getDistance(const Node * par)
{
	return (std::abs((ll)this->loc.x - (ll)par->getLocation().x)
		+ std::abs((ll)this->loc.y - (ll)par->getLocation().y));
}

void swap(Node & first, Node & second)
{
	std::swap(first.tool, second.tool);
	std::swap(first.loc, second.loc);
	std::swap(first.parent, second.parent);
	std::swap(first.hCost, second.hCost);
	std::swap(first.gCost, second.gCost);
}
