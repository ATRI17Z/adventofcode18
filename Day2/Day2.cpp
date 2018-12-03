#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <list>

int compareMyStrings(std::string a, std::string b, int &pos);

int main() {
	int sumTwos = 0;
	int sumTrees = 0;
	std::string line;
	std::list<std::string> ids;
	
	// Access file with input data: input.txt
	std::ifstream inputfile("input_Day2.txt"); //open the file
	if (inputfile.is_open() && inputfile.good()) {
		std::cout << "File is now open\n Start processing..." << std::endl;
	}
	else {
		std::cout << "Failed to open input.txt" << std::endl;
	}

	/******************************/
	/*          Part one          */
	/******************************/
	// Get Line
	while (getline(inputfile,line)) {
		// Check each line if there are letters with multiple of three or multiple of two
		std::map<char, int> letterMap;
		std::stringstream ls(line);
		char ch;
		bool twoBlocker = false;
		bool threeBlocker = false;

		// Get letters of each line
		ids.push_back(line);
		std::sort(line.begin(), line.end());
		//std::cout << "ID: " << line << std::endl;
		while (ls >> ch) {
			letterMap[ch]++;
		}
		
		// Evaluate letterMap
		for (std::map<char, int>::iterator it = letterMap.begin(); it != letterMap.end(); ++it) {
			if (it->second == 2 && twoBlocker == false) {
				sumTwos++;
				twoBlocker = true;
			}
			else if (it->second == 3 && threeBlocker == false) {
				sumTrees++;
				threeBlocker = true;
			}
		}



		
	}

	// Create Checksum:
	std::cout << "Part One Checksum: " << sumTrees * sumTwos << std::endl;

	/******************************/
	/*          Part two          */
	/******************************/
	for (std::list<std::string>::iterator it = ids.begin(); it != ids.end(); ++it) {
		for (std::list<std::string>::iterator tat = std::next(ids.begin(),1); tat != ids.end(); ++tat) {
			int numDiffs = 0;
			int pos;
			numDiffs = compareMyStrings(*it, *tat, pos);
			if (numDiffs == 1) {
				std::string a = *it;
				a.erase(pos, 1);
				std::cout << "Part Two: " << a << std::endl;
				return 0;
			}
		}
	}

	return 0;
}

int compareMyStrings(std::string a, std::string b, int &pos) {
	// Check both have equal length
	if (a.length() != b.length()) {
		return -1;
	}

	int numDiffs = 0;
	for (int i = 0; i < a.size(); i++) {
		if (a[i] != b[i]) {
			pos = i;
			numDiffs++;
		}
	}

	return numDiffs;
}