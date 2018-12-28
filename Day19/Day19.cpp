#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <array>
#include <regex>
#include <bitset>

typedef long long int ll;
typedef std::array<ll, 6> register_t;

std::list<std::string> getInputPerLines(std::string);

// Part One
std::array<ll, 6> addr(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> addi(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> mulr(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> muli(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> banr(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> bani(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> borr(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> bori(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> setr(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> seti(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> gtir(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> gtri(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> gtrr(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> eqir(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> eqri(ll A, ll B, ll C, std::array<ll, 6> reg);
std::array<ll, 6> eqrr(ll A, ll B, ll C, std::array<ll, 6> reg);

// Part Two
register_t ins1_2(register_t reg);
register_t ins2(register_t reg);
register_t ins3_6(register_t reg);
register_t ins7(register_t reg);
register_t ins8_11(register_t reg);
register_t ins12_15(register_t reg);
register_t ins16(register_t reg);
register_t ins17_24(register_t reg);
register_t ins25_26(register_t reg);
register_t ins27_35(register_t reg);
register_t dummy(register_t reg);


int main() {

	std::list<std::string> instructions = getInputPerLines("input_Day19.txt");

	std::vector<std::array<ll, 6>(*)(ll, ll, ll, std::array<ll, 6>)> functionList;
	std::vector<std::string> functionNames; // DEBUG ONLY
	std::vector<std::array<ll, 3>> instructionSet;
	std::array<ll, 3> instr;
	std::array<ll, 6> register_a = { 0,0,0,0,0,0 }, register_b;

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

	register_a = { 1, 0, 0, 0, 0, 0 };
	// Skip part one
	//ip = 2;
	while (ip < N) {
		// read the value of <ip> to the register it is (permanently) bound to 
		register_a[ipReg] = ip;

		// DEBUG PRINT
		debugStr = "ip=" + std::to_string(ip);
		debugStr += "[" + std::to_string(register_a[0]) + ", " + std::to_string(register_a[1]) + ", " + std::to_string(register_a[2]);
		debugStr += ", " + std::to_string(register_a[3]) + ", " + std::to_string(register_a[4]) + ", " + std::to_string(register_a[5]) + "] ";
		debugStr += functionNames[ip] + " " + std::to_string(instructionSet[ip][0]) + " ";
		debugStr += std::to_string(instructionSet[ip][1]) + " " + std::to_string(instructionSet[ip][2]);
		

		// Execute instruction according to <ip>
		register_b = functionList[ip](instructionSet[ip][0], instructionSet[ip][1], instructionSet[ip][2], register_a);

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
		std::cout << debugStr;
		std::cin.get();

		register_a = register_b;
	}



	std::cout << "Part One: " << register_a[0] << std::endl; // solution is 1488
	
	// Part two
	/*#ip 2

	# 0, 1, 2,  3, 4, 5
	# a, b, ip, d, e, f

	0: addi 2 16 2 		# ip=ip+16: Goto 17 (1) [only executed once
	1: seti 1 1 3		# d = 1 (20)
	2: seti 1 7 5		# f = 1(21)
	3: mulr 3 5 4		# e = d*f (22)
	4: eqrr 4 1 4		# e = (e==b?1:0) (23)
	5: addr 4 2 2		# ip = ip + e (24) -> could jump to 7 if d*f==b
	6: addi 2 1 2		# ip = ip +1 : GOTO 8 (25)
	7: addr 3 0 0		# a = a + d
	8: addi 5 1 5		# f = f +1 (26)
	9: gtrr 5 1 4		# e = (f>b?1:0) (27)
	10: addr 2 4 2		# ip = ip + e (28) -> could jump to 12 if f>b
	11: seti 2 3 2		# ip = 2(29) 
	12: addi 3 1 3		# d = d+1 (30)
	13 :gtrr 3 1 4		# e = (d>b?1:0) (31)
	14: addr 4 2 2		# ip = ip + e (32) -> could jump to 16 if e>b
	15: seti 1 9 2		# ip = 1 (33) 
	16: mulr 2 2 2		# ip = ip*ip -> THIS IS THE INSTRUCTION TO HALT
	17: addi 1 2 1 		# b += 2 (2)
	18: mulr 1 1 1 		# b = b^2 (3)
	19: mulr 2 1 1 		# b = b*ip (4)
	20: muli 1 11 1 	# b = b*11 (5)
	21: addi 4 3 4		# e = e+3 (6)
	22: mulr 4 2 4		# e = e*ip (7)
	23: addi 4 13 4		# e = e+13 (8)
	24: addr 1 4 1		# b = b+e (9)
	25: addr 2 0 2		# ip = ip+a (10)
	26: seti 0 1 2		# ip = 0 ->  would restart
	27: setr 2 0 4		# e = ip (11)
	28: mulr 4 2 4		# e = e*ip (12)
	29: addr 2 4 4		# e = ip+e (13)
	30: mulr 2 4 4		# e = ip*e (14) 
	31: muli 4 14 4		# e = e*14 (15)
	32: mulr 4 2 4		# e = e*ip (16)
	33: addr 1 4 1		# b = b+e (17)
	34: seti 0 4 0		# a = 0 (18)
	35: seti 0 5 2		# ip = 0 (19) -> GOTO 1

	===

	# 0, 1, 2,  3, 4, 5
	# a, b, ip, d, e, f

	0: addi 2 16 2 	# ip=ip+16: Goto 17 (1)			INIT

	1-2: ...		# d=1, f=1						INIT


	2: ...			# f = 1							START OUTER LOOP

	3-6: ...		# if (d*f == b) {GOTO 7}		START INNER LOOP
					# else {GOTO 8}

	7: addr 3 0 0	# a = a + d 					CHANGE A

	8-11: ...		# f = f + 1						INCREMENT F
					# if (f+1>b) {GOTO 12} {		
					# else {GOTO 3}					END INNER LOOP

	12-15: ...		# d=d+1
					# if (d>b) {GOTO 16} 
					# else {GOTO 2}					END OUTER LOOP

	16: mulr 2 2 2	# ip = ip*ip	 HALT PRGRM

	17-24: ...		# e = (e+3)*22+13				INIT PART ONE AND TWO
					# b = (b+2)^2*19*11+(e+3)*22+13

	25-26: ...		# if (a==1) {GOTO 27} 			PART TWO
					# else {GOTO  1}				PART ONE

	27-35: ...		# a = 0							INIT PART TWO
					# b = b + 14*30*(29+27*28)*32
					# ip = 0 {GOTO 1}
					# e = 14*30*(29+27*28)*32		INIT LOOPS

	==========

	Program Execution: 

	1) Execute 17 to 35 -> GOTO 1 and execute ([a:0, b:10551315, ip:1, d:0, e:10550400, f:0])
	2) Execute 2 (reset f to 1)
	3) Iterate over 3 to 11 (->increment f) until [f]+1>[b], when [d]==[b], set a to a+d
	4) Execute 12 to 15 (->increment d) and start over at 2) until [d]>[b]
	5) Execute 16 -> Halt

	===
	sum all factors of (10551315):
	17427456
	*/

	std::cout << " Part TWO: still takes way too long" << std::endl;
	register_t reg = { 1, 0, 0, 0, 0, 0 };
	int regAddr = 2;
	N = 36;
	// Start OUTER LOOP: reg = [0, 10551315, 2, 1, 10550400, 0]
	// Set reg[5] = 1:
	reg = { 0, 10551315, 2, 1, 10550400, 0 };
	while (reg[3] <= reg[1]) {
		
		reg[5] = 1;
		while (reg[5] <= reg[1]) {
			if (reg[3]*reg[5] == reg[1]) {
				// This basically sums up all factors of reg[1]
				// https://www.wolframalpha.com/input/?i=sum+all+factors+of+10551315 > Result is the puzzle result
				reg[0] += reg[3];
				std::cout << "a: " << reg[0] << std::endl;
			}
			reg[5]+=1;
		}
		reg[3]+=1;
	}

	std::cout << "Part Two: " << reg[0] << std::endl; // solution is 17427456

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

// Part One: Elf Code
std::array<ll, 6> addr(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	res[C] = reg[A] + reg[B];
	return res;
}

std::array<ll, 6> addi(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	res[C] = reg[A] + B;
	return res;
}

std::array<ll, 6> mulr(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	res[C] = reg[A] * reg[B];
	return res;
}

std::array<ll, 6> muli(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	res[C] = reg[A] * B;
	return res;
}

std::array<ll, 6> banr(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	std::bitset<8> binary = (std::bitset<8>(reg[A]) & std::bitset<8>(reg[B]));
	res[C] = (ll)binary.to_ulong();
	return res;
}

std::array<ll, 6> bani(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	std::bitset<8> binary = (std::bitset<8>(reg[A]) & std::bitset<8>(B));
	res[C] = (ll)binary.to_ulong();
	return res;
}

std::array<ll, 6> borr(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	std::bitset<8> binary = (std::bitset<8>(reg[A]) | std::bitset<8>(reg[B]));
	res[C] = (ll)binary.to_ulong();
	return res;
}

std::array<ll, 6> bori(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	std::bitset<8> binary = (std::bitset<8>(reg[A]) | std::bitset<8>(B));
	res[C] = (ll)binary.to_ulong();
	return res;
}

std::array<ll, 6> setr(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	res[C] = reg[A];
	return res;
}

std::array<ll, 6> seti(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	res[C] = A;
	return res;
}

std::array<ll, 6> gtir(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	if (A > reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<ll, 6> gtri(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	if (reg[A] > B) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<ll, 6> gtrr(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	if (reg[A] > reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<ll, 6> eqir(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	if (A == reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<ll, 6> eqri(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	if (reg[A] == B) res[C] = 1;
	else res[C] = 0;
	return res;
}

std::array<ll, 6> eqrr(ll A, ll B, ll C, std::array<ll, 6> reg)
{
	std::array<ll, 6> res = reg;
	if (reg[A] == reg[B]) res[C] = 1;
	else res[C] = 0;
	return res;
}

// PART TWO Functions
register_t ins1_2(register_t reg)
{
	reg[3] = 1;
	reg[5] = 1;
	reg[2] = 3; // to account for ip increments
	return reg;
}

register_t ins2(register_t reg)
{
	reg[5] = 1;
	reg[2] = 3;
	return reg;
}

register_t ins3_6(register_t reg)
{
	
	if (reg[3] * reg[5] == reg[1]) {
		reg[4] = 1;
		reg[2] = 7; // GOTO 7
	}
	else {
		reg[4] = 0;
		reg[2] = 8; // GOTO 8
	}
	return reg;
}

register_t ins7(register_t reg)
{
	reg[0] += reg[3];
	reg[2] = 8;
	return reg;
}

register_t ins8_11(register_t reg)
{
	reg[5] = reg[5] + 1;
	if (reg[5] > reg[1]) {
		reg[4] = 1;
		reg[2] = 12;
	}
	else {
		reg[4] = 0;
		reg[2] = 3;
	}

	return reg;
}

register_t ins12_15(register_t reg)
{
	reg[3] = reg[3] + 1;
	if (reg[3] > reg[1]) {
		reg[4] = 1;
		reg[2] = 16;
	}
	else {
		reg[4] = 0;
		reg[2] = 2;
	}
	return reg;
}

register_t ins16(register_t reg)
{
	if (reg[2] > 5) {
		std::cout << "Programm Halt\n";
	}
	reg[2] = reg[2] * reg[2];
	return reg;
}

register_t ins17_24(register_t reg)
{
	reg[4] = (reg[4] + 3) * 22 + 13;
	reg[1] = (reg[1] + 2)*(reg[1] + 2) * 19 * 11 + reg[4]; // needs updated res[4];
	reg[2] = 25;
	return reg;
}

register_t ins25_26(register_t reg)
{
	if (reg[0] != 0) {
		reg[2] = reg[2] + reg[0] + 1;
	}
	else {
		reg[2] = 1;
	}
	return reg;
}

register_t ins27_35(register_t reg)
{
	reg[4] = 14 * 30 * (29 + 27 * 28) * 32;
	reg[1] = reg[1] + reg[4];
	reg[0] = 0;
	reg[2] = 1;

	return reg;
}

register_t dummy(register_t reg)
{
	std::cout << " This should not happen\n";
	return reg;
}