#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <regex>



std::string getInputAsString(std::string);
std::list<std::string> getInputPerLines(std::string);

int main() {

	std::list<std::string> rules = getInputPerLines("input_Day12.txt");
	std::string state = getInputAsString("input_Day12init.txt");

	int ruleSize = 5;
	//int numGen = 20; // Part 1
	unsigned long long numGen = 50000000000; // Part 2

	// Parsing input
	std::map<std::string, std::string> rulesMap;
	std::regex regexpr("([.#])+");
	for (std::list<std::string>::iterator it = rules.begin(); it != rules.end(); ++it) {
		//std::cout << *it << std::endl;
		std::string key,value;
		std::regex_iterator<std::string::iterator> reg_it(it->begin(), it->end(), regexpr);
		std::cout << reg_it->str() << " -> ";
		key = reg_it->str();
		++reg_it;
		value = reg_it->str();
		std::cout << reg_it->str() << std::endl;
		rulesMap[key] = value;
	}
	std::cout  << std::endl << std::endl << state << std::endl;

	// Pad initial state with four dots (empty Pots)
	//std::cout << state << std::endl;
	int firtPotIdx = -4;
	state = "...." + state + "....";
	//std::cout << state << std::endl;


	std::string subStr;
	std::string newSubState;
	std::string newState;
	for (int i = 0; i < numGen ; ++i) {
		//std::cout << "Generation " << i << ": " << state << std::endl;
		// check if first two dots are having plants -> need to pad more at beginning
		if (state.compare(0, 4, "....")) {
			//std::cout << "\t\t NEED INITIAL PADDING" << std::endl;
			// Implement only if needed
			state = "...." + state;
			firtPotIdx -= 4;
		}
		// check if last two dots are having plants -> need to pad more at end
		// compare returns '0' if match
		if (state.compare(state.size()-4, 4, "....")) { // true if not 4 dots are present
			//std::cout << "\t\t NEED TRAILING PADDING" << std::endl;
			// Implement only if needed
			state = state + "....";
		}
		// make copy of current state for new state but w/o any plants
		newState = std::string(state.size(),'.');
		for (int j = 0; j < state.size()-ruleSize+1; ++j) {
			subStr = state.substr(j, ruleSize);
			newSubState = rulesMap[subStr];

			//DEBUG
			//std::cout << "\t" << subStr << " -> " << newSubState << std::endl;

			if (!newSubState.empty()) {
				newState.replace(j + 2, 1, newSubState);
				newSubState = "";
			}
		}

		// newState becomes old state
		state = newState;
		 

	}
	std::cout << "Generation " << numGen << ": " << state << std::endl;

	// Count the number
	unsigned long long score = 0;
	//std::cout << "firstPotIdx: " << firtPotIdx << std::endl;
	for (int i = 0; i < state.size(); ++i) {
		if (state.compare(i, 1, "#") == 0) {
			//std::cout << "[" << i + firtPotIdx << "] ";
			score += i + firtPotIdx;
		}
	}

	std::cout << "P1orP2: Score " << score << std::endl;





	return 0;
}




/******************************
*   INPUT HELPER FUNCTIONS   *
******************************/

std::string getInputAsString(std::string fileName) {
	std::string str;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
		return 0;
	}

	// Create String
	str.assign((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	in.close();
	return str;
}

std::list<std::string> getInputPerLines(std::string fileName) {
	std::list<std::string> lines;
	std::string line;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
	}

	// Create Vector of lines
	while (getline(in, line)) {
		lines.push_back(line);
	}
	in.close();
	return lines;
}