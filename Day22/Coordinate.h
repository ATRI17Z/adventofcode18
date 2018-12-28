#pragma once

typedef long long int ll;

class Coordinate {
public:
	Coordinate() : target(false), x(0), y(0), type(0), erosionLvl(0), geoIdx(0) {}
	Coordinate(int x, int y) : target(false), x(x), y(y), type(0), erosionLvl(0), geoIdx(0) {}
	Coordinate(int x, int y, ll gIdx) : target(false), x(x), y(y), geoIdx(gIdx) {
		this->erosionLvl = (this->geoIdx + CAVE_DEPTH) % 20183;
		this->type = (this->erosionLvl) % 3;
	}
	~Coordinate() {}

	// Copy Constructor
	Coordinate(const Coordinate &c) {
		this->target = c.isTarget();
		this->x = c.getX();
		this->y = c.getY();
		this->type = c.getType();
		this->erosionLvl = c.getELvl();
		this->geoIdx = c.getGeoIdx();
	}

	bool isTarget() const { return this->target; }
	void setIsTarget(bool x) { this->target = x; }

	size_t getX() const { return this->x; }
	void setX(int x) { this->x = x; }
	size_t getY() const { return this->y; }
	void setY(int y) { this->y = y; }

	int getType() const { return this->type; }
	void setType(int t) { this->type = t; }

	ll getELvl() const { return this->erosionLvl; }
	void setELvl(ll t) { this->erosionLvl = t; }

	ll getGeoIdx() const { return this->geoIdx; }
	void setGeoIdx(ll t) { this->geoIdx = t; }

private:
	bool target;
	size_t x, y;
	int type;
	ll erosionLvl;
	ll geoIdx;

	static const ll CAVE_DEPTH = 510; // EXAMPLE DEPTH
	//static const ll CAVE_DEPTH = 3066; // PART ONE
};

/*
class Coordinate {
public:
	Coordinate() : target(false), x(0), y(0), parent(nullptr), type(0), erosionLvl(0), geoIdx(0),
		           distance(LLONG_MAX), fScore(LLONG_MAX), tool(0) {}
	Coordinate(int x, int y) : target(false), x(x), y(y), parent(nullptr), type(0), erosionLvl(0), geoIdx(0),
		           distance(LLONG_MAX), fScore(LLONG_MAX), tool(0) {}
	Coordinate(int x, int y, ll gIdx) : target(false), x(x), y(y), parent(nullptr), geoIdx(gIdx),
		           distance(LLONG_MAX), fScore(LLONG_MAX) {
		this->erosionLvl = (this->geoIdx + CAVE_DEPTH) % 20183;
		this->type = (this->erosionLvl) % 3;
		this->tool = this->type;
		//std::cout << "["<<this->x<<"," << this->y << "]: gIdx: " << this->geoIdx << "\teLvl: " << this->erosionLvl << "\ttype: " << this->type << std::endl;
	}
	~Coordinate() {}

	// Copy Constructor
	Coordinate(const Coordinate &c) {
		this->target = c.isTarget();
		this->x = c.getX();
		this->y = c.getY();
		this->parent = new Coordinate();
		this->parent = c.getP();
		this->type = c.getType();
		this->erosionLvl = c.getELvl();
		this->geoIdx = c.getGeoIdx();
		this->distance = c.getDist();
		this->fScore = c.getFScore();
		this->tool = c.getTool();
	}

	bool isTarget() const { return this->target; }
	void setIsTarget(bool x) { this->target = x; }

	size_t getX() const { return this->x; }
	void setX(int x) { this->x = x; }
	size_t getY() const { return this->y; }
	void setY(int y) { this->y = y; }

	Coordinate* getP() const { return this->parent; }
	void setP(Coordinate* x) { this->parent = x; }


	int getType() const { return this->type; }
	void setType(int t) { this->type = t; }

	ll getELvl() const { return this->erosionLvl; }
	void setELvl(ll t) { this->erosionLvl = t; }

	ll getGeoIdx() const { return this->geoIdx; }
	void setGeoIdx(ll t) { this->geoIdx = t; }

	ll getDist() const { return this->distance; }
	void setDist(ll t) { this->distance = t; }

	ll getFScore() const { return this->fScore; }
	void setFScore(ll t) { this->fScore = t; }

	int getTool() const { return this->tool; }
	bool requestToolChange(int t);


private:
	bool target;
	size_t x, y;
	Coordinate* parent;
	int type;
	ll erosionLvl;
	ll geoIdx;

	ll distance;
	ll fScore;
	int tool;

	static const ll CAVE_DEPTH = 510; // EXAMPLE DEPTH
	//static const ll CAVE_DEPTH = 3066; // PART ONE
};

// Operator definition for Class Type Coordinate
inline bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
	// return true if lhs and rhs have same distance
	if (lhs.getDist() == rhs.getDist()) return true;
	return false;
}
inline bool operator!=(const Coordinate& lhs, const Coordinate& rhs) { return !(lhs == rhs); }
inline bool operator< (const Coordinate& lhs, const Coordinate& rhs) {
	// returns true if lhs has longer distance
	if (lhs.getDist() < rhs.getDist()) return true;
	else return false;
}
inline bool operator> (const Coordinate& lhs, const Coordinate& rhs) { return rhs < lhs; }
inline bool operator<=(const Coordinate& lhs, const Coordinate& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Coordinate& lhs, const Coordinate& rhs) { return !(lhs < rhs); }
*/
