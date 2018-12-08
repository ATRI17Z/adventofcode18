#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <cmath>
#include <chrono>

int getDistance(int X, int Y, int x, int y);
int getSummedDistance(int x, int y, std::vector<int> X, std::vector<int> Y);

int main() {
	auto start = std::chrono::high_resolution_clock::now();

	std::string line;
	std::vector<int> X, Y;
	int maxX = INT_MIN;
	int maxY = INT_MIN;
	int minX = INT_MAX;
	int minY = INT_MAX;

	// Access file with input data: input.txt
	std::ifstream inputfile("input_Day6.txt"); //open the file
	if (inputfile.is_open() && inputfile.good()) {
		// all good
		std::cout << "File open" << std::endl;
	}
	else {
		std::cout << "Failed to open input" << std::endl;
		return 0;
	}

	while (getline(inputfile, line)) {
		X.push_back(std::stoi(line.substr(0, line.find(",", 0))));
		Y.push_back(std::stoi(line.substr(line.find(",", 0) + 1, line.size())));
		// Get Max and Min values
		if (X.back() > maxX) maxX = X.back();
		if (X.back() < minX) minX = X.back();
		if (Y.back() > maxY) maxY = Y.back();
		if (Y.back() < minY) minY = Y.back();
	}

	std::cout << "Range X: [" << minX << "," << maxX << "]" << std::endl;
	std::cout << "Range Y: [" << minY << "," << maxY << "]" << std::endl;

	// Brute force
	int* numPt = new int[X.size()]();
	std::vector<int> isInf(X.size());
	int dist, minDist, minIdx = 0;
	for (int i = minX; i <= maxX; ++i) {
		for (int j = minY; j <= maxY; ++j) {
			minDist = INT_MAX;
			for (int n = 0; n < X.size(); ++n) {
				// calculate distance for all coordinates
				dist = getDistance(X[n], Y[n], i, j);
				if (dist < minDist) {
					minDist = dist;
					minIdx = n;
				}
				else if (dist == minDist) {
					// Equidistant
					minIdx = -1;
				}
			}
			// Point assigend
			if (minIdx >= 0) {
				numPt[minIdx]++;
				if (i == minX || i == maxX || j == minY || j == maxY) {
					// at border thus infinite patch
					isInf[minIdx] = 1;
				}
			}
		}
	}

	int max = INT_MIN;
	int mIdx = 0;
	for (int i = 0; i < X.size(); ++i) {
		if (max < numPt[i] && isInf[i] != 1) {
			max = numPt[i];
			mIdx = i;
		}

	}
	std::cout << "P1[" << mIdx << "]: " << max << std::endl;
	auto finishONE = std::chrono::high_resolution_clock::now();

	/* PART 2*/
	// Brute force
	int sumLoc = 0;
	for (int i = minX; i <= maxX; ++i) {
		for (int j = minY; j <= maxY; ++j) {
			sumLoc += getSummedDistance(i, j, X, Y);
		}
	}

	std::cout << "P2: " << sumLoc << std::endl;
	auto finishTWO = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsedONE = finishONE - start;
	std::chrono::duration<double> elapsedTWO = finishTWO - finishONE;
	std::cout << "Elapsed time: P1: " << elapsedONE.count() * 1000 << "ms P2: " << elapsedTWO.count() * 1000 << "ms" << std::endl;
	return 0;
}

int getDistance(int X, int Y, int x, int y) {
	int dist = 0;
	dist = std::abs(X - x) + std::abs(Y - y);
	return dist;
}

int getSummedDistance(int x, int y, std::vector<int> X, std::vector<int> Y) {
	int dist = 0;
	for (int n = 0; n < X.size(); ++n) {
		// calculate distance for all coordinates
		dist += getDistance(X[n], Y[n], x, y);
	}
	if (dist < 10000) {	return 1;}
	else {	return 0;}
}