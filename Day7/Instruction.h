#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>



class Instruction
{
public:
	Instruction(std::string instructionKey, std::string lockKey);
	Instruction(std::string instructionKey);
	~Instruction();

	bool operator< (const Instruction &other) const {
		return instruction < other.instruction;
	}

	std::string lockedWith();
	std::string getInstruction();
	int getProcessTime();
	bool isInstruction(std::string instruction);
	void addLock(std::string lockKey);
	bool isUnlocked(std::string key);
	std::string getLock();

private:
	std::string instruction;
	std::string lockList;
	int processTime;

	

};

