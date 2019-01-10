#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <array>
#include <regex>
#include <vector>

typedef int ll;

ll getManhattenDist4D(std::array<ll, 4>, std::array<ll, 4>);
std::list<std::string> getInputPerLines(std::string);


int main() {

	std::array<ll, 4> point;
	std::vector<std::array<ll, 4>> pointList;
	


	// PARSE INPUT
	std::list<std::string> pointStrings = getInputPerLines("input_Day25.txt");
	std::regex regexpr("(-?[0-9]+)");
	std::smatch m;
	for (std::list<std::string>::iterator it = pointStrings.begin(); it != pointStrings.end(); ++it) {
		std::regex_iterator<std::string::iterator> rit(it->begin(), it->end(), regexpr);
		std::regex_iterator<std::string::iterator> rend;

		point[0] = std::stoi(rit->str(), nullptr, 0); ++rit;
		point[1] = std::stoi(rit->str(), nullptr, 0); ++rit;
		point[2] = std::stoi(rit->str(), nullptr, 0); ++rit;
		point[3] = std::stoi(rit->str(), nullptr, 0);

		//for (auto n : point) {
		//	std::cout << n << " ";
		//}
		//std::cout << std::endl;

		pointList.push_back(point);
	}
	//std::cout << "=====================" << std::endl;

	int numConst = 1;
	int dist;
	std::vector<int> usedPoints(pointList.size(),0);
	std::vector<int> cliqueIdx;
	std::vector<std::vector<int>> cliqueListIdx;

	for (int i = 0; i < pointList.size(); ++i) {
		if (usedPoints[i] < 1) {
			cliqueIdx.push_back(i);
			usedPoints[i] = numConst;
			bool stillMembersToCheck = true;
			while (stillMembersToCheck) {
				stillMembersToCheck = false;
			}
			for (int j = 0; j < pointList.size(); ++j) {
				if (i == j) continue;
				bool isInConstellation = false;
				//bool notAlreadyContained = true;
				for (int k = 0; k < cliqueIdx.size(); ++k) {
					dist = getManhattenDist4D(pointList[cliqueIdx[k]], pointList[j]);
					//std::cout << "Dist: " << j << "-" << cliqueIdx[k] << ": " << dist << std::endl;
					if (dist < 4 &&
						j != cliqueIdx[k] &&
						usedPoints[j] < 1) {
						//std::cout << "\t MatchDist: " << j << "-" << cliqueIdx[k] << ": " << dist << std::endl;
						// these two point form a constelation
						isInConstellation = true;
						break;
					}
				}
				if (isInConstellation) {
					//std::cout << "Adding " << j << " to clique " << numConst << std::endl;
					cliqueIdx.push_back(j);
					usedPoints[j] = numConst;
					j = 0; // start over

					// DEBUG
					//for (auto s : cliqueIdx) {
					//	std::cout << "[" << s << "]" << " ";
					//}
					//std::cout << std::endl;
					//std::cin.get();
				}

			}

			cliqueListIdx.push_back(cliqueIdx);
			cliqueIdx.erase(cliqueIdx.begin(), cliqueIdx.end());
		}
		++numConst;
	}
	//std::cout << "=====================" << std::endl;
	
	numConst = 0;
	for (auto c : cliqueListIdx) {
		if (c.size() > 2) ++numConst;
		for (auto s : c) {
			std::cout << "[" << s << "]" << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "Num Constellations: " << cliqueListIdx.size() << std::endl;
	// 169: too low
	// 567: too high (counted all longly single points as well)

	return 0;
}




/******************************
*   INPUT HELPER FUNCTIONS   *
******************************/

ll getManhattenDist4D(std::array<ll, 4> a, std::array<ll, 4> b)
{
	ll dist;
	dist = abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2]) + abs(a[3] - b[3]);
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