#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <climits>

#include "InstructionScheduler.h"

std::string getInputAsString(std::string);
std::vector<std::string> getInputPerLines(std::string);

/********************************************************
*  Day 7 Puzzle:
*  Using an OO approach to have objects for each task
*  and each worker. To to its morphological creation
*  the solution is not clean at all but at least it works
*  at is. Many things should be improved in multiple ways
*/

int main() {

	std::string str;
	std::ifstream input;
	std::vector<std::string> lines;

	InstructionScheduler scheduler = InstructionScheduler();

	// Load input as Single String
	//str = getInputAsString("input_Day7_test.txt");
	// Get Input as vector of lines
	lines = getInputPerLines("input_Day7.txt");
	for (int i = 0; i < lines.size(); ++i) {
		std::string before, after;
		before = lines[i][5];
		after = lines[i][36];

		// Create Instructions
		scheduler.addInstruction(before, after);
		//scheduler.printInstructions();
	}

	// Schedule Instructions PART ONE
	std::cout << std::endl << "PART ONE: " << std::endl;
	scheduler.getSchedule();

	// Schedule Instructions PART TWO
	std::cout << std::endl << "PART TWO: " << std::endl;
	//scheduler.getTimedSchedule(2); // for testData
	scheduler.getTimedSchedule(5);
	

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

std::vector<std::string> getInputPerLines(std::string fileName) {
	std::vector<std::string> lines;
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