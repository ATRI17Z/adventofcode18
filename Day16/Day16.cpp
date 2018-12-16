#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <array>
#include <regex>
#include <bitset>

std::list<std::string> getInputPerLines(std::string);

std::array<int, 4> addr(int A, int B, int C, std::array<int,4> reg);
std::array<int, 4> addi(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> mulr(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> muli(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> banr(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> bani(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> borr(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> bori(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> setr(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> seti(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> gtir(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> gtri(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> gtrr(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> eqir(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> eqri(int A, int B, int C, std::array<int, 4> reg);
std::array<int, 4> eqrr(int A, int B, int C, std::array<int, 4> reg);



int main() {

	std::list<std::string> instructions = getInputPerLines("input_Day16.txt");
	                   //Before: [1, 1, 3, 0]
	std::regex regexprA("After:[ ]+\\[([0-9]+),[ ]?([0-9]+),[ ]?([0-9]+),[ ]?([0-9]+)\\]");
	std::regex regexprB("Before:[ ]+\\[([0-9]+),[ ]?([0-9]+),[ ]?([0-9]+),[ ]?([0-9]+)\\]");
	std::regex regexpr("([0-9]+)[ ]+([0-9]+)[ ]?([0-9]+)[ ]?([0-9]+)");
	
	std::smatch m;

	std::list<std::array<int,4>> before, instr, after, remaining;
	std::array<int, 4> regi;
	bool beforeWasLast = false;
	// Get instructions (first part)
	for (std::list<std::string>::iterator it = instructions.begin(); it != instructions.end();) {	
		//std::cout << "matching: " << *it << std::endl;
		if (std::regex_search(*it, m, regexprB)) {
			regi[0] = std::stoi(m[1], nullptr, 0);
			regi[1] = std::stoi(m[2], nullptr, 0);
			regi[2] = std::stoi(m[3], nullptr, 0);
			regi[3] = std::stoi(m[4], nullptr, 0);
			before.push_back(regi);
			beforeWasLast = true;
			//std::cout << "\tMatched Before" << std::endl;
		}
		else if (std::regex_search(*it, m, regexprA)) {
			regi[0] = std::stoi(m[1], nullptr, 0);
			regi[1] = std::stoi(m[2], nullptr, 0);
			regi[2] = std::stoi(m[3], nullptr, 0);
			regi[3] = std::stoi(m[4], nullptr, 0);
			after.push_back(regi);
			beforeWasLast = false;
			//std::cout << "\tMatched After" << std::endl;
		}else if (std::regex_search(*it, m, regexpr)) {
			regi[0] = std::stoi(m[1], nullptr, 0);
			regi[1] = std::stoi(m[2], nullptr, 0);
			regi[2] = std::stoi(m[3], nullptr, 0);
			regi[3] = std::stoi(m[4], nullptr, 0);
			if (beforeWasLast) {
				instr.push_back(regi);
				beforeWasLast = false;
				//std::cout << "\tMatched Instruction" << std::endl;
			}
			else {
				remaining.push_back(regi);
			}
		}
		++it;
	}

	std::cout << "Before:  " << before.size() << std::endl;
	std::cout << "Operand: " << instr.size() << std::endl;
	std::cout << "After:   " << after.size() << std::endl;
	std::cout << "Remain:  " << remaining.size() << std::endl;

	// Creat list of functions to call
	std::list<std::array<int, 4>(*)(int, int, int, std::array<int, 4>)> functionList;
	functionList.push_back(&addr);
	functionList.push_back(&addi);
	functionList.push_back(&mulr);
	functionList.push_back(&muli);
	functionList.push_back(&banr);
	functionList.push_back(&bani);
	functionList.push_back(&borr);
	functionList.push_back(&bori);
	functionList.push_back(&setr);
	functionList.push_back(&seti);
	functionList.push_back(&gtir);
	functionList.push_back(&gtri);
	functionList.push_back(&gtrr);
	functionList.push_back(&eqir);
	functionList.push_back(&eqri);
	functionList.push_back(&eqrr);

	int countMoreThanTwo = 0;
	std::array<int, 4> res; 
	//std::array<int, 16> ident; ident.fill(0); // check which functions are identified
	std::list<std::array<int, 4>>::iterator insit = instr.begin();
	std::list<std::array<int, 4>>::iterator aftit = after.begin();
	std::list<std::array<int, 4>(*)(int, int, int, std::array<int, 4>)>::iterator lastFcnCall;
	std::vector<std::array<int, 4>(*)(int, int, int, std::array<int, 4>)> sortedFcnList(16,&addr);
	for (std::list<std::array<int, 4>>::iterator it = before.begin(); it != before.end();++it) {
		int localCounter = 0;
		for (std::list<std::array<int, 4>(*)(int, int, int, std::array<int, 4>)>::iterator at = functionList.begin();
			at != functionList.end();++at) {
			res = (*at)((*insit)[1], (*insit)[2], (*insit)[3],*it);
			// check if res is correct
			if (res == *aftit) {
				// correct
				++localCounter;	
				lastFcnCall = at;
			}
		}
		// comment out this section for part one:
		if (localCounter == 1) {
			std::cout << "There are unique cases: " << (*insit)[0] << std::endl;
			sortedFcnList[(*insit)[0]] = *lastFcnCall;
			// remove this function call from the functionList
			functionList.erase(lastFcnCall);
		}

		// part one counter
		if (localCounter > 2) ++countMoreThanTwo;
		if (instr.end() != insit++) {};
		if (after.end() != aftit++) {};
	}

	std::cout << "P1: " << countMoreThanTwo << std::endl;

	// Part two: run test program
	//reg = ? ? ; // start register
	for (std::list<std::array<int, 4>>::iterator it = remaining.begin(); it != remaining.end(); ++it) {
		res = sortedFcnList[(*it)[0]]((*it)[1], (*it)[2], (*it)[3], res);
	}

	std::cout << "P2: " << res[0] << std::endl;

	return 0;
}


/******************************
*   INPUT HELPER FUNCTIONS   *
******************************/

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

std::array<int, 4> addr(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	res[C] = reg[A] + reg[B];
	return res;
}

std::array<int, 4> addi(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	res[C] = reg[A] + B;
	return res;
}

std::array<int, 4> mulr(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	res[C] = reg[A] * reg[B];
	return res;
}

std::array<int, 4> muli(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	res[C] = reg[A] * B;
	return res;
}

std::array<int, 4> banr(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	std::bitset<8> binary = (std::bitset<8>(reg[A]) & std::bitset<8>(reg[B]));
	res[C] = (int)binary.to_ulong();
	return res;
}

std::array<int, 4> bani(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	std::bitset<8> binary = (std::bitset<8>(reg[A]) & std::bitset<8>(B));
	res[C] = (int)binary.to_ulong();
	return res;
}

std::array<int, 4> borr(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	std::bitset<8> binary = (std::bitset<8>(reg[A]) | std::bitset<8>(reg[B]));
	res[C] = (int)binary.to_ulong();
	return res;
}

std::array<int, 4> bori(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	std::bitset<8> binary = (std::bitset<8>(reg[A]) | std::bitset<8>(B));
	res[C] = (int)binary.to_ulong();
	return res;
}

std::array<int, 4> setr(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	res[C] = reg[A];
	return res;
}

std::array<int, 4> seti(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	res[C] = A;
	return res;
}

std::array<int, 4> gtir(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	if (A > reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<int, 4> gtri(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	if (reg[A] > B) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<int, 4> gtrr(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	if (reg[A] > reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<int, 4> eqir(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	if (A == reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<int, 4> eqri(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	if (reg[A] == B) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<int, 4> eqrr(int A, int B, int C, std::array<int, 4> reg)
{
	std::array<int, 4> res = reg;
	if (reg[A] == reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

