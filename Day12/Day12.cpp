#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <regex>



std::string getInputAsString(std::string);
std::list<std::string> getInputPerLines(std::string);
long long getScore(std::string str, int fPidx);

/* First part is straight forward, second part would need
   50'000'000'000 iterations if done the same way as part one.
   This is likely not to be the way to go. First thought was,
   that the pattern of plants would stabilize at some point and
   no changes happen anymore. However, this did not seem to be
   the case during the first couple of ten-thousands of 
   iterations. So checking the resulting sum after each iteration
   shows a gradual growth. Looking at the growth speed
   shows that after a very short time the speed remains constant
   which means that the solution of part two can be calculated
   analytically as soon as the speed remains constant. This assumption
   turned out to be true.*/


int main() {

	std::list<std::string> rules = getInputPerLines("input_Day12.txt");
	std::string state = getInputAsString("input_Day12init.txt");
	//std::list<std::string> rules = getInputPerLines("input_Day12_test.txt");
	//std::string state = getInputAsString("input_Day12init_test.txt");


	int ruleSize = 5;
	int numGenP1 = 20; // Part 1
	unsigned long long numGen = 50000000000; // Part 2
	long long scoreP1=0, scoreP2=0;

	// Parsing input
	std::map<std::string, std::string> rulesMap;
	std::regex regexpr("([.#])+");
	for (std::list<std::string>::iterator it = rules.begin(); it != rules.end(); ++it) {
		std::string key,value;
		std::regex_iterator<std::string::iterator> reg_it(it->begin(), it->end(), regexpr);
		key = reg_it->str();
		++reg_it;
		value = reg_it->str();
		rulesMap[key] = value;
	}
	
	// Pad initial state with four dots (empty Pots)
	int firtPotIdx = -4;
	state = "...." + state + "....";
	
	std::string subStr;
	std::string newSubState;
	std::string newState;
	long long score = 0, oldScore = 0, scoreDiff = 0, oldScoreDiff = 0;
	for (unsigned long long i = 0; i < numGen ; ++i) {
		// check if first four dots are having plants -> need to pad more at beginning
		if (state.compare(0, 4, "....")) {
			state = "...." + state;
			firtPotIdx -= 4;
		}
		while (!state.compare(0, 5, ".....")) {
			// remove unneccessary dots in beginning
			state.erase(0, 1);
			firtPotIdx += 1;
		}
		// check if last four dots are having plants -> need to pad more at end
		// compare returns '0' if match
		if (state.compare(state.size()-4, 4, "....")) { // true if not 4 dots are present
			state = state + "....";
		}
		while (!state.compare(state.size() - 5, 5, ".....")) {
			// remove unnecessary trailing dots
			state.erase(state.size() - 1, 1);
		}

		// make copy of current state for new state but w/o any plants
		newState = std::string(state.size(),'.');
		for (int j = 0; j < state.size()-ruleSize+1; ++j) {
			// get a substring of <ruleSize> to check all our rules on
			subStr = state.substr(j, ruleSize);
			newSubState = rulesMap[subStr];

			if (!newSubState.empty()) {
				newState.replace(j + 2, 1, newSubState);
				//newSubState = "";
			}
		}

		// Did plant pattern stabilized
		if (!(newState.compare(state))) {			
			std::cout << "Pattern stabilized at " << i << ": " << std::endl;
			std::cout << state << std::endl;
			std::cout << newState << std::endl << std::endl;
			break; // didn't seem to be the case
		}

		state = newState;
		//score = 0;
		score = getScore(state, firtPotIdx);
		scoreDiff = score - oldScore;

		// Get Score of Part one
		if (i == numGenP1 - 1) {
			scoreP1 = score;
		}

		// Did the plant pattern score stabilize
		if (score == oldScore) {
			std::cout << "Stable Score["<<i<<"]: " << score << std::endl;
		}

		// Did the plant pattern score derivative stabilize
		if (scoreDiff == oldScoreDiff) {
			// [130] Score: 8684 Old: 8632 Diff: -52
			scoreP2 = score + (50000000000 - (i + 1)) * scoreDiff;
			std::cout << "P1 Score:" << scoreP1 << "\tP2 Score Estimate: " << scoreP2 << std::endl;
		}
		oldScore = score;
		oldScoreDiff = scoreDiff;
	}
	// Count the number
	score = getScore(state,firtPotIdx);

	std::cout << "P2: Score " << score << std::endl;

	return 0;
}

long long getScore(std::string str, int fPidx) {
	// Count the number
	long long score = 0;
	for (unsigned long long i = 0; i < str.size(); ++i) {
		if (str.compare(i, 1, "#") == 0) {
			score += i + fPidx;
		}
	}
	return score;
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
