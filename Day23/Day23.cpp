#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <array>
#include <regex>
#include <algorithm>

typedef long long int ll;


ll getManhattenDist(std::array<ll, 4>, std::array<ll, 4> );
std::list<std::string> getInputPerLines(std::string);

int main() {

	std::list<std::string> instructions = getInputPerLines("input_Day23.txt");
	std::regex regexpr("pos=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>,[ ]?r=([0-9]+)");
	std::smatch m;

	std::vector<std::array<ll, 4>> locations;
	ll maxR = 0;
	int maxIdx = 0;
	int counter = 0;
	//bounding Box[minX,maxX,minY,maxY,minZ,maxZ]
	std::array<ll, 6> bB = {LONG_MAX,LONG_MIN,LONG_MAX,LONG_MIN,LONG_MAX,LONG_MIN };

	// read instruction set and store it in accessible vector
	for (std::list<std::string>::iterator it = instructions.begin(); it != instructions.end(); ++it) {
		if (std::regex_match(*it, m, regexpr)) {

			//std::cout << "Pos: " << m[1] << ", " << m[2] << ", " << m[3] << " wit r=" << m[4] << std::endl;
			std::array<ll, 4> tmp;
			tmp[0] = std::stoi(m[1], nullptr, 0);
			tmp[1] = std::stoi(m[2], nullptr, 0);
			tmp[2] = std::stoi(m[3], nullptr, 0);
			tmp[3] = std::stoi(m[4], nullptr, 0);
			locations.push_back(tmp);

			if (maxR < tmp[3]) {
				maxR = tmp[3];
				maxIdx = counter;
			}
			else if (maxR == tmp[3]) {
				//std::cout << " Possible DOUBLE MAX DIST " << std::endl;
			}

			// Get Bounding Box
			if (bB[0] > tmp[0])bB[0] = tmp[0];
			if (bB[1] < tmp[0])bB[1] = tmp[0];
			if (bB[2] > tmp[1])bB[2] = tmp[1];
			if (bB[3] < tmp[1])bB[3] = tmp[1];
			if (bB[4] > tmp[2])bB[4] = tmp[2];
			if (bB[5] < tmp[2])bB[5] = tmp[2];

		}
		++counter;
	}

	std::cout << "Max Radius at Elment " << maxIdx << " of " << counter << " with R=" << maxR;
	std::cout << " r= " << locations[maxIdx][0] << ", " << locations[maxIdx][1] << ", " << locations[maxIdx][2] << ", " << std::endl;

	std::cout << "Bounding Box: " << bB[0] << " " << bB[1] << " " << bB[2] << " "
		<< bB[3] << " " << bB[4] << " " << bB[5] << std::endl;
	std::cout << "Num Cooridantes " << (bB[1]-bB[0]) << " * " << (bB[3]-bB[2]) << " * " <<  (bB[5]-bB[4]) << std::endl;

	std::vector<int> contained;
	for (int i = 0; i < locations.size();++i) {
		if (getManhattenDist(locations[i], locations[maxIdx]) <= locations[maxIdx][3]) {
			// point is inside strongest nanobot
			contained.push_back(i);
		}
	}

	std::cout << "Part ONE: " << contained.size() << std::endl;

	/* PART TWO */
	counter = 0;
	int maxN = 0;
	int bestCliqueIdx;
	ll dist, radiusSum;
	std::vector<int> clique;
	std::vector<std::vector<int>> intersectingGroupIdx;
	

	// Get all the nanobots <j> which intersect with nanobot <i>
	for (int i = 0; i < locations.size(); ++i) {
		clique.push_back(i);
		for (int j = 0; j < locations.size(); ++j) {
			if (j == i) continue;
			bool inRangeOfAll = true;
			bool notAlreadyContained = true;
			for (int k = 0; k < clique.size(); ++k) {
				dist = getManhattenDist(locations[clique[k]], locations[j]);
				radiusSum = locations[clique[k]][3] + locations[j][3];

				if (dist > radiusSum) {
					// the new bot does NOT satisfies current clique member
					inRangeOfAll = false;
					break;
				}
				else if (j == clique[k]) {
					//ignore
					notAlreadyContained = false;
				}
			}
			if (inRangeOfAll && notAlreadyContained) {
				if (std::find(clique.begin(), clique.end(), j) == clique.end()) {
					// only insert idx if not already part of vector
					clique.push_back(j);
				}
			}
		}
		
		// sort and add it to the Group of intersecting BOX ID's
		std::sort(clique.begin(), clique.end());
		
		// sort and add it to the Group of intersecting BOX ID's
		std::sort(clique.begin(), clique.end());
		intersectingGroupIdx.push_back(clique);

		if (maxN < clique.size()) {
			maxN = (int)clique.size();
			bestCliqueIdx = i;
		}
		// delete old clique:
		clique.erase(clique.begin(), clique.end());
	}
	std::cout << "Best Clique with size " << maxN << ":" << std::endl;

	// Find intersection points
	clique = intersectingGroupIdx[bestCliqueIdx]; // there might be more than one of these best Cliques
	std::vector<int> newCliqueMember;
	ll maxDistOrigin = 0;
	for (int j = 0; j < clique.size(); ++j) {
		// Get Max Distance to Origin of any member nanobot of the best clique
		std::array<ll, 4> origin = { 0,0,0,0 };
		dist = getManhattenDist(origin, locations[clique[j]]) - locations[clique[j]][3];
		if (maxDistOrigin < dist) maxDistOrigin = dist;
	}

	std::cout << "Max Dist to Origin of any given matching Octaethron is: " << maxDistOrigin << std::endl; // 95541011
	

	return 0;
}






/******************************
*   INPUT HELPER FUNCTIONS   *
******************************/

ll getManhattenDist(std::array<ll, 4> a, std::array<ll, 4> b)
{
	ll dist;
	dist = abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2]);
	return dist;
}

std::list<std::string> getInputPerLines(std::string fileName) {
	std::list<std::string> lines;
	std::string line;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
		lines.push_back(std::string()); // empty string
		return lines;
	}

	// Create Vector of lines
	while (getline(in, line)) {
		lines.push_back(line);
	}
	in.close();
	return lines;
}