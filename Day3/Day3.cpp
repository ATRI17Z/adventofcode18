#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <regex>

struct Patch
{
	int num;
	int xCoord, yCoord;
	int xWidth, yWidth;
};

int main()
{
	// Read file input
	std::string line;
	std::ifstream infile("input_Day3.txt");
	std::list<Patch> patches;

	// Regex Preparation: #1161 @ 949,691: 19x25
	std::regex regex_exp("#([0-9]+) @ ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+)");
	//std::regex regex_exp("(-?[0-9]+)");
	std::smatch pieces_match;
	//int num, xCoord, yCoord, xWidth, yWidth;
	int maxX = 0, maxY = 0;

	while (std::getline(infile, line)) {
		Patch newPatch;

		if (std::regex_search(line, pieces_match, regex_exp)) {
			// got match
			
			//For some reason in match[0] the whole string is captured
			newPatch.num = std::stoi(pieces_match[1].str(), nullptr, 10);
			newPatch.xCoord = std::stoi(pieces_match[2].str(), nullptr, 10);
			newPatch.yCoord = std::stoi(pieces_match[3].str(), nullptr, 10);
			newPatch.xWidth = std::stoi(pieces_match[4].str(), nullptr, 10);
			newPatch.yWidth = std::stoi(pieces_match[5].str(), nullptr, 10);

			// Add Patch to list
			patches.push_back(newPatch);
			// Keep track of max size of used area
			if (maxX < newPatch.xCoord + newPatch.xWidth) {
				maxX = newPatch.xCoord + newPatch.xWidth;
			}
			if (maxY < newPatch.yCoord + newPatch.yWidth) {
				maxY = newPatch.yCoord + newPatch.yWidth;
			}
		}
		else {
			// no match
			std::cout << "no regex match" << std::endl;
		}


	}
	// Max size
	std::cout << "Patch is at least: " << maxX << "," << maxY << " big" << std::endl;
	// Create 2D Vector with appropriate size and Data Structure
	std::vector<std::vector<int>> patchMatrix(maxX, std::vector<int>(maxY));
	
	// Iterate through list and fill up patches in patchMatrix
	int alternateClaimedMultiple = 0;
	for (std::list<Patch>::iterator it = patches.begin(); it != patches.end(); ++it) {
		for (int x = it->xCoord; x < it->xCoord + it->xWidth; ++x) {
			for (int y = it->yCoord; y < it->yCoord + it->yWidth; ++y) {
				patchMatrix[x][y]++;
				if (patchMatrix[x][y] == 2) {
					alternateClaimedMultiple++;
				}
			}
		}
	}

	// Count number of elements in Patch Matrix with two or more claims
	int claimedMultiple = 0;
	for (int x = 0; x < maxX; ++x) {
		for (int y = 0; y < maxY; ++y) {
			if (patchMatrix[x][y] > 1) {
				claimedMultiple++;
			}
		}
	}

	std::cout << "Day3P1: There are " << claimedMultiple << " square inches which are claimed at least double"
		<< std::endl << "\talternativ method got me: " << alternateClaimedMultiple << std::endl;

	/******************************/
	/*         PART TWO           */
	/******************************/

	// Iterate through list and find patch which is left intact
	int idxPatch = 0;
	for (std::list<Patch>::iterator it = patches.begin(); it != patches.end(); ++it) {
		bool freePatch = true;
		for (int x = it->xCoord; x < it->xCoord + it->xWidth; ++x) {
			for (int y = it->yCoord; y < it->yCoord + it->yWidth; ++y) {
				if (patchMatrix[x][y] > 1) {
					freePatch = false;
				}
			}
		}
		if (freePatch) {
			// we got it
			idxPatch = it->num;
			break;
		}
	}

	std::cout << "Day3P2: Only free Patch number is : " << idxPatch << std::endl;

	return 0;
}

