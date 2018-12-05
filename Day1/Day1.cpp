#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

int main() {
	int instruction;
	int numInstructions = 0;
	//std::list<int> instrList;
	std::vector<int> instrList;


	// Access file with input data: input.txt
	std::ifstream inputfile("input_Day1.txt"); //open the file
	if (inputfile.is_open() && inputfile.good()) {
		std::cout << "File is now open\n Start processing..." << std::endl;
	}
	else {
		std::cout << "Failed to open input.txt" << std::endl;
	}

	// Put all instructions into a list
	while (inputfile >> instruction) {
		//std::cout << instruction << std::endl;
		instrList.push_back(instruction);
	}

	/******************************/
	/*          Part one          */
	/******************************/

	// Start Frequency Counting
	int pos = 0;
	int freq = 0;
	while (pos >= 0 && pos < instrList.size()) {
		// add frequencies
		freq += instrList[pos];
		++pos;
	}
	
	std::cout << "Final Frequency of Day1 Puzzle 1 is " << freq << std::endl;

	/******************************/
	/*          Part two          */
	/******************************/
	std::map<std::string, int> freqs;
	pos = 0;
	freq = 0;
	freqs[std::to_string(freq)]++;
	while (true) {
		// get new freq
		freq += instrList[pos];
		// add frequencies to map
		freqs[std::to_string(freq)]++;
		// check if we have seen this freq already
		if (freqs[std::to_string(freq)] > 1) {
			// we got first double
			std::cout << "First double frquency is " << freq << std::endl;
			break;
		}
		// Increment counter
		++pos;

		// check if pos > instrList.size()
		if (pos >= instrList.size()) {
			pos = 0;
			//std::cout << "Reached Instruction End" << std::endl;
		}

	}


	return 0;
}