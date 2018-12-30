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

	//static const ll CAVE_DEPTH = 510; // EXAMPLE DEPTH
	static const ll CAVE_DEPTH = 3066; // PART ONE
};


