#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <list>
#include <climits>
#include <chrono>

struct Point {
	int oX; // x Origin
	int oY; // y Origin
	int rX; // x Position
	int rY; // y Position
	int vX; // x Velocity
	int vY; // y velocity
};

void displayPoints(std::list<Point>& P, int maxX, int maxY, int minX, int minY);

int main() {
	
	std::string line;
	std::regex regex_num("(-?[0-9]+)");
	std::list<Point> navPoints;
	int maxX = INT_MIN;
	int maxY = INT_MIN;
	int minX = INT_MAX;
	int minY = INT_MAX;

	// Access file with input data: input.txt
	std::ifstream inputfile("input_Day10.txt"); //open the file
	if (inputfile.is_open() && inputfile.good()) {
		// all good
		std::cout << "File open" << std::endl;
	}
	else {
		std::cout << "Failed to open input" << std::endl;
		return 0;
	}

	Point navPoint;
	while (getline(inputfile, line)) {
		std::regex_iterator<std::string::iterator> reg_it(line.begin(), line.end(), regex_num);
		navPoint.oX = std::stoi(reg_it->str(), nullptr, 10);
		navPoint.rX = navPoint.oX;
		++reg_it;
		navPoint.oY = std::stoi(reg_it->str(), nullptr, 10);
		navPoint.rY = navPoint.oY;
		++reg_it;
		navPoint.vX = std::stoi(reg_it->str(), nullptr, 10);
		++reg_it;
		navPoint.vY = std::stoi(reg_it->str(), nullptr, 10);
		++reg_it;
		navPoints.push_back(navPoint);

		// Keep track of Maximum and Minimum
		if (maxX < navPoint.oX) maxX = navPoint.oX;
		if (maxY < navPoint.oY) maxY = navPoint.oY;
		if (minX > navPoint.oX) minX = navPoint.oX;
		if (minY > navPoint.oY) minY = navPoint.oY;
	}
	std::cout << "Input read, start processing..." << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	int steps = 0;
	while (true) {
		// Show Current Point Locations
		if (maxY - minY < 12) { // This number was larger initially (based on Example)
			displayPoints(navPoints, maxX, maxY, minX, minY);
			std::cout << steps << "s " << std::endl;
			std::cout << std::endl;
			break;
		}

		// Reset min max values
		maxX = INT_MIN;
		maxY = INT_MIN;
		minX = INT_MAX;
		minY = INT_MAX;
		// Move one step
		for (std::list<Point>::iterator it = navPoints.begin();
			it != navPoints.end(); ++it) {
			it->rX += it->vX;
			it->rY += it->vY;

			// Keep track of Maximum and Minimum
			if (maxX < it->rX) maxX = it->rX;
			if (maxY < it->rY) maxY = it->rY;
			if (minX > it->rX) minX = it->rX;
			if (minY > it->rY) minY = it->rY;
		}
		/*
		std::cout << std::endl;
		std::cout << "==========================================" << std::endl;
		std::cout << "[minX,maxX] - > [minY,maxY]" << std::endl;
		std::cout << "[" << minX << "," << maxX << "] - > [" << minY << "," << maxY << "]" << std::endl;
		std::cout << "==========================================" << std::endl;
		*/
		++steps;
	}
	auto finishONE = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedONE = finishONE - start;
	std::cout << "Elapsed time: P1+P2: " << elapsedONE.count() * 1000 << "ms" << std::endl;

	return 0;
}


void displayPoints(std::list<Point>& P, int maxX, int maxY, int minX, int minY) {
	//maxX=238, int maxY=226, int minX=133, int minY=167
	//
	int numRows = maxY - minY + 1;
	int numCols = maxX - minX + 1;
	//int xOff = (minX > 0) ? minX : -minX;
	//int yOff = (minY > 0) ? minY : -minY;
	int xOff = minX;
	int yOff = minY;
	int xIdx, yIdx;
	//std::cout << "Rows: " << numRows << "; Cols: " << numCols << std::endl;
	std::vector<std::vector<int>> data(numRows, std::vector<int>(numCols, 0));

	// fill vector with Points
	for (std::list<Point>::iterator it = P.begin();it != P.end(); ++it) {
		//std::cout << "i: " << it->rY + yOff << "; j: " << it->rX + xOff << std::endl;
		yIdx = it->rY - yOff;
		xIdx = it->rX - xOff;
		if (yIdx < 0 || xIdx < 0 || yIdx >= numRows || xIdx >= numCols) {
			std::cout << "Index Out of Bounds" << std::endl;
			std::cout << "nR: " << numRows << " ";
			std::cout << "nC: " << numCols << " ";
			std::cout << "yI: " << yIdx << " ";
			std::cout << "xI: " << xIdx << " " << std::endl;
			return;
		}else {
			data[yIdx][xIdx] = 1;
		}
	}

	// Print vector
	for (int i = 0; i < numRows; ++i) {
		for (int j = 0; j < numCols; ++j) {
			std::cout << data[i][j];
		}
		std::cout << std::endl;
	}
	
}