#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <array>
#include <regex>
#include <bitset>

// Defining my integers as 4 bytes
#define  reg_size 32
typedef int ll;
typedef std::array<ll, 6> register_t;

std::list<std::string> getInputPerLines(std::string);

// Part One
register_t addr(ll A, ll B, ll C, register_t reg);
register_t addi(ll A, ll B, ll C, register_t reg);
register_t mulr(ll A, ll B, ll C, register_t reg);
register_t muli(ll A, ll B, ll C, register_t reg);
register_t banr(ll A, ll B, ll C, register_t reg);
register_t bani(ll A, ll B, ll C, register_t reg);
register_t borr(ll A, ll B, ll C, register_t reg);
register_t bori(ll A, ll B, ll C, register_t reg);
register_t setr(ll A, ll B, ll C, register_t reg);
register_t seti(ll A, ll B, ll C, register_t reg);
register_t gtir(ll A, ll B, ll C, register_t reg);
register_t gtri(ll A, ll B, ll C, register_t reg);
register_t gtrr(ll A, ll B, ll C, register_t reg);
register_t eqir(ll A, ll B, ll C, register_t reg);
register_t eqri(ll A, ll B, ll C, register_t reg);
register_t eqrr(ll A, ll B, ll C, register_t reg);

void printRegister(register_t);

/*
Wasted a lot of time to convert the Elf-Code in C++ (efficient) code
but it seemed cumbersome. So I started the brute force attack (as below)
while figuring out the Elf-Code translation.
But before I was done the brute force attack returned the result ;)
*/

int main() {
	
	std::cout << "Size of INT: " << sizeof(int) << std::endl;

	std::list<std::string> instructions = getInputPerLines("input_Day21.txt");

	std::vector<std::array<ll, 6>(*)(ll, ll, ll, std::array<ll, 6>)> functionList;
	std::vector<std::string> functionNames; // DEBUG ONLY
	std::vector<std::array<ll, 3>> instructionSet;
	std::array<ll, 3> instr;
	std::array<ll, 6> register_a, register_b;

	std::regex regexprA("#ip ([0-9])");
	std::regex regexprB("([a-z]+)[ ]+([0-9]+)[ ]?([0-9]+)[ ]?([0-9]+)");
	std::smatch m;
	ll ipReg, ip = 0;

	std::regex_match(instructions.front(), m, regexprA);
	ipReg = std::stoi(m[1], nullptr, 0);

	// read instruction set and store it in accessible vector
	for (std::list<std::string>::iterator it = instructions.begin(); it != instructions.end(); ++it) {
		if (std::regex_match(*it, m, regexprB)) {
			//std::cout << m[1] << "\t" << m[2] << "\t" << m[3] << "\t" << m[4] << std::endl;
			if (m[1] == "addr") {
				functionList.push_back(&addr);
				functionNames.push_back("addr"); // DEBUG only
			}
			else if (m[1] == "addi") {
				functionList.push_back(&addi);
				functionNames.push_back("addi"); // DEBUG only
			}
			else if (m[1] == "mulr") {
				functionList.push_back(&mulr);
				functionNames.push_back("mulr"); // DEBUG only
			}
			else if (m[1] == "muli") {
				functionList.push_back(&muli);
				functionNames.push_back("muli"); // DEBUG only
			}
			else if (m[1] == "banr") {
				functionList.push_back(&banr);
				functionNames.push_back("banr"); // DEBUG only
			}
			else if (m[1] == "bani") {
				functionList.push_back(&bani);
				functionNames.push_back("bani"); // DEBUG only
			}
			else if (m[1] == "borr") {
				functionList.push_back(&borr);
				functionNames.push_back("borr"); // DEBUG only
			}
			else if (m[1] == "bori") {
				functionList.push_back(&bori);
				functionNames.push_back("bori"); // DEBUG only
			}
			else if (m[1] == "setr") {
				functionList.push_back(&setr);
				functionNames.push_back("setr"); // DEBUG only
			}
			else if (m[1] == "seti") {
				functionList.push_back(&seti);
				functionNames.push_back("seti"); // DEBUG only
			}
			else if (m[1] == "gtir") {
				functionList.push_back(&gtir);
				functionNames.push_back("gtir"); // DEBUG only
			}
			else if (m[1] == "gtri") {
				functionList.push_back(&gtri);
				functionNames.push_back("gtri"); // DEBUG only
			}
			else if (m[1] == "gtrr") {
				functionList.push_back(&gtrr);
				functionNames.push_back("gtrr"); // DEBUG only
			}
			else if (m[1] == "eqir") {
				functionList.push_back(&eqir);
				functionNames.push_back("eqir"); // DEBUG only
			}
			else if (m[1] == "eqri") {
				functionList.push_back(&eqri);
				functionNames.push_back("eqri"); // DEBUG only
			}
			else if (m[1] == "eqrr") {
				functionList.push_back(&eqrr);
				functionNames.push_back("eqrr"); // DEBUG only
			}
			instr[0] = std::stoi(m[2], nullptr, 0);
			instr[1] = std::stoi(m[3], nullptr, 0);
			instr[2] = std::stoi(m[4], nullptr, 0);
			instructionSet.push_back(instr);
		}
	}

	// INIT 
	ll N = (ll)instructionSet.size(); // Number of instructions
	std::cout << N << " instructions in program" << std::endl;
	//for (auto& i : functionNames) {
	//	std::cout << i << std::endl;
	//}

	std::string debugStr;

	long long instrCounter = 0;
	int alreadIn = 0;
	// Solution Part ONE: 8797248 (done it by inspection: what is reg[2] when inst 28 is executed for the first time
	register_a = { 0, 0, 0, 0, 0, 0 };
	// Part TWO: Solution is 3007673 (brute force approach, couldn't figure out another way)
	std::vector<ll> haltingNumbers;
	while (ip < N) {
		// read the value of <ip> to the register it is (permanently) bound to 
		register_a[ipReg] = ip;

		// DEBUG PRINT
		debugStr = "[" + std::to_string(instrCounter+1) + "]:ip=" + std::to_string(ip);
		debugStr += "[" + std::to_string(register_a[0]) + ", " + std::to_string(register_a[1]) + ", " + std::to_string(register_a[2]);
		debugStr += ", " + std::to_string(register_a[3]) + ", " + std::to_string(register_a[4]) + ", " + std::to_string(register_a[5]) + "] ";
		debugStr += functionNames[ip] + " " + std::to_string(instructionSet[ip][0]) + " ";
		debugStr += std::to_string(instructionSet[ip][1]) + " " + std::to_string(instructionSet[ip][2]);


		// Execute instruction according to <ip>
		if (ip == 28) {
			// Part two
			//std::cout << ".";
			// Check if register_a[2] is not already in the vector
			auto it = std::lower_bound(haltingNumbers.begin(), haltingNumbers.end(), register_a[2]);
			if (haltingNumbers.size() == 0) {
				// first time here
				std::cout << "Part One: " << register_a[2] << std::endl; // solution is 8797248
			}
			if (it == haltingNumbers.end() || *it != register_a[2]) {
				// number not yet inside halting numbers
				//std::cout << "IP=" << ip << ": reg[2]: " << register_a[instructionSet[ip][0]] << std::endl;
				haltingNumbers.push_back(register_a[2]);
				std::sort(haltingNumbers.begin(), haltingNumbers.end());
				alreadIn = 0;
			}
			else {
				++alreadIn;
			}
			if (alreadIn > 2) {
				std::cout << "Part 2: Solution: " << register_a[2] << std::endl;
			}
			
		}

			
		register_b = functionList[ip](instructionSet[ip][0], instructionSet[ip][1], instructionSet[ip][2], register_a);
		++instrCounter;

		// Read out the value of the register <ip> is bound to and add one to move to the next instruction
		ip = register_b[ipReg] + 1;

		// Abort if the value of the register which <ip> is bound to points outside the program
		if (ip >= N) {

			// DEBUG
			// DEBUG Print
			std::cout << " [" << register_b[0] << ", " << register_b[1] << ", " << register_b[2]
				<< ", " << register_b[3] << ", " << register_b[4] << ", " << register_b[5] << "] " << std::endl;

			register_a = register_b;
			register_a[ipReg] = ip; // in case <ip> is bound to register 0
			// DEBUG PRINT
			std::cout << "ip=" << ip;//DEBUG PRINT
			std::cout << "[" << register_a[0] << ", " << register_a[1] << ", " << register_a[2]
				<< ", " << register_a[3] << ", " << register_a[4] << ", " << register_a[5] << "] " << std::endl;
			std::cout << "IP(" << ip << ") value too high, abort" << std::endl;

			break;
		}

		// DEBUG Print
		debugStr += " [" + std::to_string(register_b[0]) + ", " + std::to_string(register_b[1]) + ", " + std::to_string(register_b[2]);
		debugStr += ", " + std::to_string(register_b[3]) + ", " + std::to_string(register_b[4]) + ", " + std::to_string(register_b[5]) + "]";

		// DEBUG
		//std::cout << debugStr;
		//std::cout << std::endl;
		//std::cin.get();

		register_a = register_b;
	}

	
	
}

void printRegister(register_t r) {
	for (auto i : r) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
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

// Part One: Elf Code
register_t addr(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	res[C] = reg[A] + reg[B];
	return res;
}

register_t addi(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	res[C] = reg[A] + B;
	return res;
}

register_t mulr(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	res[C] = reg[A] * reg[B];
	return res;
}

register_t muli(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	res[C] = reg[A] * B;
	return res;
}

register_t banr(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	std::bitset<reg_size> binary = (std::bitset<reg_size>(reg[A]) & std::bitset<reg_size>(reg[B]));
	res[C] = (ll)binary.to_ullong();
	return res;
}

register_t bani(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	std::bitset<reg_size> binary = (std::bitset<reg_size>(reg[A]) & std::bitset<reg_size>(B));
	res[C] = (ll)binary.to_ullong();
	return res;
}

register_t borr(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	std::bitset<reg_size> binary = (std::bitset<reg_size>(reg[A]) | std::bitset<reg_size>(reg[B]));
	res[C] = (ll)binary.to_ullong();
	return res;
}

register_t bori(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	std::bitset<reg_size> binary = (std::bitset<reg_size>(reg[A]) | std::bitset<reg_size>(B));
	res[C] = (ll)binary.to_ullong();
	return res;
}

register_t setr(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	res[C] = reg[A];
	return res;
}

register_t seti(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	res[C] = A;
	return res;
}

register_t gtir(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	if (A > reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

register_t gtri(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	if (reg[A] > B) res[C] = 1;
	else res[C] = 0;
	return res;
}

register_t gtrr(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	if (reg[A] > reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

register_t eqir(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	if (A == reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

register_t eqri(ll A, ll B, ll C, register_t reg)
{
	register_t res = reg;
	if (reg[A] == B) res[C] = 1;
	else res[C] = 0;
	return res;
}

register_t eqrr(ll A, ll B, ll C, register_t reg)
{
	// PART ONE: Instruction which causes halt
	//std::cout << "reg[2]!=reg[0]: " << reg[A] << " = " << reg[B] << std::endl;
	//std::cin.get();
	register_t res = reg;
	if (reg[A] == reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}