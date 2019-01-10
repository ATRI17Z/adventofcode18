#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <regex>

typedef std::vector<std::vector<int>> matrixMap;


class Coordinate {
public:
	Coordinate() : x(0), y(0) {}
	Coordinate(int x, int y) : x(x), y(y) {}
	Coordinate(const Coordinate& c) : x(c.x), y(c.y) {}
	~Coordinate() {}

	int x;
	int y;
};

class WaterFront {
public:
	WaterFront() : front(0, 0) { trail.push_back(Coordinate()); }
	WaterFront(int x, int y, Coordinate c) : front(x, y) { trail.push_back(Coordinate(c)); }
	~WaterFront() {}

	Coordinate front;
	std::list<Coordinate> trail;

};

void fillSolidWater(std::list<WaterFront>::iterator, int, int, matrixMap &);
void fillFlowWater(std::list<WaterFront>::iterator, int, int, matrixMap &);
int flowLeft(std::list<WaterFront>::iterator&, matrixMap &);
int flowRight(std::list<WaterFront>::iterator&, matrixMap &);
int flowDownUntilClay(std::list<WaterFront>::iterator&, matrixMap&);
void printMap(const std::vector<std::vector<int>>&);
void writeMap(const matrixMap&);

int main() {

	// Access file with input data : input.txt
	std::ifstream inputfile("input_Day17.txt"); //open the file
	if (inputfile.is_open() && inputfile.good()) {
		// all good
		std::cout << "File open" << std::endl;
	}
	else {
		std::cout << "Failed to open input" << std::endl;
		return 0;
	}

	int maxY = 0;
	int minY = INT_MAX;
	int maxX = 0;
	int minX = INT_MAX;
	std::list<Coordinate> solidPts;
	std::string line;
	std::regex regXs("x=([0-9]+)[ ]*,");
	std::regex regYs("y=([0-9]+)[ ]*,");
	std::regex regXr(",[ ]*x=([0-9]+)..([0-9]+)");
	std::regex regYr(",[ ]*y=([0-9]+)..([0-9]+)");
	std::smatch x,y;

	// Parse Input
	while (getline(inputfile, line)) {
		if (std::regex_search(line, x, regXs)) {
			// got an single x value and y range
			std::regex_search(line, y, regYr);

			int xV = std::stoi(x[1].str(), nullptr, 10);
			int yVmin = std::stoi(y[1].str(), nullptr, 10);
			int yVmax = std::stoi(y[2].str(), nullptr, 10);
			
			if (minX > xV) minX = xV;
			if (maxX < xV) maxX = xV;
			if (minY > yVmin) minY = yVmin;
			if (maxY < yVmax) maxY = yVmax;
			for (int i = yVmin; i <= yVmax; ++i) {
				solidPts.push_back(Coordinate(xV,i));
				//std::cout << "store: " << xV << "x" << i << std::endl;
			}

			//std::cout << "X: " << x[1] << " ";
			//std::cout << "Range Y: " << y[1] << " - " << y[2] << "\t";
			//std::cout << "xLim: " << minX << " - " << maxX << " ";
			//std::cout << "yLim: " << minY << " - " << maxY << std::endl;
		}
		else {
			// got an x range and single y value
			std::regex_search(line, y, regYs);
			std::regex_search(line, x, regXr);

			int xVmin = std::stoi(x[1].str(), nullptr, 10);
			int xVmax = std::stoi(x[2].str(), nullptr, 10);
			int yV = std::stoi(y[1].str(), nullptr, 10);
			
			if (minX > xVmin) minX = xVmin;
			if (maxX < xVmax) maxX = xVmax;
			if (minY > yV) minY = yV;
			if (maxY < yV) maxY = yV;
			for (int i = xVmin; i <= xVmax; ++i) {
				solidPts.push_back(Coordinate(i,yV));
			}

			//std::cout << "Range X: " << x[1] << " - " << x[2] << " ";
			//std::cout << "Y: " << y[1] << "\t";
			//std::cout << "xLim: " << minX << " - " << maxX << " ";
			//std::cout << "yLim: " << minY << " - " << maxY << std::endl;
		}

		
	}
	std::cout << "Total Range X: " << minX << " - " << maxX << " => D: " << maxX - minX << std::endl;
	std::cout << "Total Range Y: " << minY << " - " << maxY << " => D: " << maxY - minY << std::endl;

	int nCols = maxX - minX + 3; // two margin on either side
	int nRows = maxY - minY + 3; // two margin on either side
	int xOff = 1 - minX;
	int yOff = 1 - minY;
	matrixMap map(nCols,std::vector<int>(nRows,0));

	// Create Map
	for (auto c : solidPts) {
		map[(size_t)c.x + xOff][(size_t)c.y + yOff] = 9; // Clay
	}
	
	// Print Map
	//printMap(map);
	
	// Start pouring water
	// Map Legend:
	// 0: Free space
	// 1: Flowing Water
	// 3: Still Water
	// 9: Clay / Solid ground
	int xS = 500;
	int yS = 0;
	std::list<Coordinate> waterStream;
	std::list<WaterFront> waterFronts;

	//Coordinate waterFront(xS, yS);
	//waterStream.push_back(waterFront);
	waterFronts.push_back(WaterFront(xS + xOff, yS, Coordinate(xS + xOff, yS)));

	map[(size_t)xS+xOff][(size_t)yS] = 1;
	//printMap(map);

	int res;
	int nLeft, nRight;
	
	int debugCount = 0;
	while (!waterFronts.empty()) {
		// - extend WaterFront until it hits clay
		// - check if it can go anywhere (left or right), if so, create a new front for each free side and
		//   set the origin to the origin of the split front, and remove split front: then extend those
		// - if a front cant extend or split anymore, set the front back to its origin and the origin of the new
		//   front to the previous place
		// Map Legend:
		// 0: Free space
		// 1: Flowing Water
		// 3: Still Water
		// 9: Clay / Solid ground

		// DEBUG;
		
		//std::cout << "WaterFronts: ";
		//for (auto& i : waterFronts) {
		//	std::cout << "[" << i.front.x << "," << i.front.y << "] ";
		//}
		//std::cout << std::endl;
		//writeMap(map);
		//std::cout << "Iter: " << debugCount << std::endl;
		//if (debugCount >52) std::cin.get();
		//++debugCount;
		

		for (std::list<WaterFront>::iterator it = waterFronts.begin(); it != waterFronts.end(); ) {

			//std::cout << "Working on Waterfront: [" << it->front.x << "," << it->front.y << "]" << std::endl;

			res = 1;
			while (res) {
				res = flowDownUntilClay(it, map);
				if (res == -1) break;
			}
			//writeMap(map); // DEBUG
			if (res == -1) {
				it = waterFronts.erase(it);
				continue;
			}
			else if (res == 0) {
				// deal with water flow direction change

				// search for ground end or wall on either side
				// pos return -> edge
				// neg return -> wall
				nLeft = flowLeft(it, map);
				nRight = flowRight(it,map);
				if (nLeft <= 0 && nRight <= 0) {// we have walls on either side
					//std::cout << "[" << it->front.x << "," << it->front.y << "]:Blocked Right and Left L:";
					//std::cout << nLeft << " R: " << -nRight << std::endl;

					//if surounded by flowing water: remove water front
					if ((map[(size_t)it->front.x - 1][(size_t)it->front.y] == 1 &&
						map[(size_t)it->front.x + 1][(size_t)it->front.y] == 1) ||
						map[(size_t)it->front.x][(size_t)it->front.y + 1] == 1) {
						// flowing water left and right: delete this waterfront
						//writeMap(map);
						it = waterFronts.erase(it);
						continue;
					}

					fillSolidWater(it, -nLeft, -nRight, map);
					if (it->trail.size() > 0) {
						it->front = it->trail.back();
						it->trail.pop_back();

						
					}
					else {
						//std::cout << "Water Front trail too short" << std::endl;
						it = waterFronts.erase(it);
						//writeMap(map);
						continue;
					}
					//continue;
				}
				else if (nLeft <= 0 && nRight > 0) { // we have a wall left and open right
					//std::cout << "[" << it->front.x << "," << it->front.y << "]:Blocked Left, free Right L:";
					//std::cout << nLeft << " R: " << -nRight << std::endl;
					fillFlowWater(it, -nLeft, nRight, map);
					for (int i = 0; i <= nRight; ++i) {
						it->trail.push_back(Coordinate(it->front.x+i, it->front.y));
					}
					it->front = it->trail.back();
					//continue;
				}
				else if (nLeft > 0 && nRight <= 0) {// we have a wall right and open left
					//std::cout << "[" << it->front.x << "," << it->front.y << "]:Blocked Right, free left L:";
					//std::cout << nLeft << " R: " << -nRight << std::endl;
					fillFlowWater(it, nLeft, -nRight, map);
					for (int i = 0; i <= nLeft; ++i) {
						it->trail.push_back(Coordinate(it->front.x - i, it->front.y));
					}
					it->front = it->trail.back();
					//continue;
				}
				else if (nLeft > 0 && nRight > 0) {
					//std::cout << "[" << it->front.x << "," << it->front.y << "]:Free left and right L:";
					//std::cout << nLeft << " R: " << -nRight << std::endl;
					fillFlowWater(it, nLeft, nRight, map);

					WaterFront splitFront(it->front.x, it->front.y, Coordinate(it->front.x, it->front.y));
					for (int i = 1; i <= nLeft; ++i) {
						splitFront.trail.push_back(Coordinate(it->front.x - i, it->front.y));
					}
					splitFront.front = splitFront.trail.back();
					waterFronts.push_back(splitFront);
					for (int i = 0; i <= nRight; ++i) {
						it->trail.push_back(Coordinate(it->front.x + i, it->front.y));
					}
					it->front = it->trail.back();
					//continue;
				}
			}
			// Inc iterator
			++it;
			//std::cout << "Write Map" << std::endl;
			//writeMap(map);
			
		}

		// Remove same water fronts
		
	}

	//printMap(map);

	// Sum up all the water fields
	int waterCount = 0;
	int stillWaterCount = 0;
	/*
	int nCols = maxX - minX + 3; // two margin on either side
	int nRows = maxY - minY + 3; // two margin on either side
	int xOff = 1 - minX;
	int yOff = 1 - minY;
	*/
	for (int c = 0; c < nCols; ++c) {
		for (int r = 1; r < nRows-1; ++r) {
			if (map[c][r] == 1) {
				++waterCount;
			}
			if (map[c][r] == 3) {
				++waterCount;
				++stillWaterCount;
			}
		}
	}

	std::cout << "Part One: Water Count: " << waterCount << std::endl;
	std::cout << "Part Two: Water Count: " << stillWaterCount << std::endl;

	writeMap(map);

	return 0;
}

// Map Legend:
// 0: Free space
// 1: Flowing Water
// 3: Still Water
// 9: Clay / Solid ground

// Fill map with solid water arount iterator position <it>
//  nLeft: offset to the left from <it>
//  nRight: offset to the right form <it>
void fillSolidWater(std::list<WaterFront>::iterator it, int nLeft, int nRight, matrixMap &map) {
	for (int X = it->front.x - nLeft; X <= it->front.x + nRight;++X) {
		if (map[X][it->front.y+ (size_t)1]>1) {// 
			map[X][it->front.y] = 3;
		}
		else {
			// solid water cannot be above flowing water or empty space
			std::cout << "WARNGIN: Still water cannot be above flowing water" << std::endl;
			return;
		}
	}
}

// Fill map with flowing water arount iterator position <it>
//  nLeft: offset to the left from <it>
//  nRight: offset to the right form <it>
void fillFlowWater(std::list<WaterFront>::iterator it, int nLeft, int nRight, matrixMap &map) {
	for (int X = it->front.x - nLeft; X <= it->front.x + nRight; ++X) {
		//if (map[X][it->front.y + (size_t)1] > 1) {// 
			map[X][it->front.y] = 1;
		//}
		//else {
			// solid water cannot be above flowing water or empty space
		//	std::cout << "WARNGIN: Flowing water cannot be above flowing water" << std::endl;
		//	return;
		//}
	}
}

// Search ground until water can go down again or hits a clay wall
// returns pos. number of steps if there is an edge to flow down (pos number)
// returns 0 if no step possible and thus this waterfront should be deleted
// returns neg. number of steps if there is a wall
// Map Legend:
// 0: Free space
// 1: Flowing Water
// 3: Still Water
// 9: Clay / Solid ground
int flowLeft(std::list<WaterFront>::iterator& it, matrixMap &map) {
	int newX, newY;
	newX = it->front.x - 1;
	newY = it->front.y;
	while (newX > 0) {
		if (map[(size_t)newX][(size_t)newY] <= 1) {
			if (map[(size_t)newX][(size_t)newY + 1] < 1) { // we have an edge
				return it->front.x - newX;
			}
			else if (map[(size_t)newX][(size_t)newY + 1] == 1) {// we are flowing above flowing water
				return 0;
			}
			else {
				--newX;
			}
		}//else if (map[(size_t)newX][(size_t)newY] == 1 )
		//	     map[(size_t)newX][(size_t)newY+1] == 3 ) {
			// check if there is a front involved. if so, overrite and remove front
			//TODO;
			//--newX;
		//	return 0;
		//}
		else {
			break;
		}
	}
	return newX - it->front.x + 1; // Steps until wall
}

// Search ground until water can go down again or hits a clay wall
// returns pos. number of steps if there is an edge to flow down (pos number)
// returns 0 if no step possible and thus this waterfront should be deleted
// returns neg. number of steps if there is a wall
// Map Legend:
// 0: Free space
// 1: Flowing Water
// 3: Still Water
// 9: Clay / Solid ground
int flowRight(std::list<WaterFront>::iterator& it, matrixMap &map) {
	int newX, newY;
	newX = it->front.x + 1;
	newY = it->front.y;
	while (newX > 0) {
		if (map[(size_t)newX][(size_t)newY] <= 1) { // go right until hitting still water or clay
			if (map[(size_t)newX][(size_t)newY + 1] < 1) { // we have an edge
				return newX - it->front.x;
			}
			else if (map[(size_t)newX][(size_t)newY + 1] == 1) { // we are flowing above flowing water
				return 0;
			}
			else {
				++newX;
			}
		}
		//else if (map[(size_t)newX][(size_t)newY] == 1 ||
		//	     map[(size_t)newX][(size_t)newY] == 3) {
			// check if there is a front involved. if so, overrite and remove front
			//TODO;
			//++newX;
		//	return 0;
		//}
		else {
			break;
		}
	}
	return it->front.x - newX + 1; // Steps until wall or water from somewhere else
}

// Push Front one down, 
// returns:
//   1 if down was a free space, 
//   0 when still water or solid ground,
//  -1 when out of map or on flowing water
// Map Legend:
//  0: Free space
//  1: Flowing Water
//  3: Still Water
//  9: Clay / Solid ground
int flowDownUntilClay(std::list<WaterFront>::iterator& it, matrixMap &map) {
	int newX, newY;
	size_t nRows = map.front().size();
	size_t nCols = map.size();
	newY = it->front.y+1;
	newX = it->front.x;
	if (newY >= nRows) {
		// Outside Y range, water flowing out at bottom
		return -1;
	}else{
		if (it->front.x >= nCols) std::cout << "Out of Col Range" << std::endl;
		if (map[newX][newY] > 1) {
			// hitting water or clay below
			return 0;
		}
		else if (map[newX][newY] == 1) {
			// hitting flowing water
			//std::cout << "Hitting Flowing Water" << std::endl;
			return -1;
		}
		else {
			// extend trail of water front by one
			it->trail.push_back(Coordinate(it->front.x, it->front.y));
			it->front = Coordinate(newX,newY);
			map[newX][newY] = 1;
			return 1;
		}
	}
}

void printMap(const std::vector<std::vector<int>>& map) {
	int nCols = (int)map.size();
	int nRows = (int)map.front().size();
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			if (map[j][i] == 0) { // free space
				std::cout << ".";
			}
			else if (map[j][i] <= 1) { // flowing water at moving front level
				std::cout << "|";
			}
			else if (map[j][i] <= 2) { // flowing water 
				std::cout << "|";
			}
			else if (map[j][i] <= 3) { // still water
				std::cout << "~";
			}
			else { // clay
				std::cout << map[j][i];
			}
		}
		std::cout << std::endl;
	}

}

void writeMap(const matrixMap& map) {
	std::ofstream myfile;
	myfile.open("mapWriteOut.txt",std::ios::in | std::ios::trunc);
	
	int nCols = (int)map.size();
	int nRows = (int)map.front().size();
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			if (map[j][i] == 0) { // free space
				myfile << ".";
			}
			else if (map[j][i] <= 1) { // flowing water at moving front level
				myfile << "|";
			}
			else if (map[j][i] <= 2) { // flowing water 
				myfile << "|";
			}
			else if (map[j][i] <= 3) { // still water
				myfile << "~";
			}
			else { // clay
				myfile << map[j][i];
			}
		}
		myfile << std::endl;
	}

	myfile.close();
	return;
}