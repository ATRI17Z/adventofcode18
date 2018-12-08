#include "Instruction.h"



Instruction::Instruction(std::string instructionKey, std::string lockKey) {
	// Store the instruction Key: this can't be changed anymore
	instruction = instructionKey;
	// store first lock key
	addLock(lockKey);
	processTime = 60+instructionKey[0] - 64;
}

Instruction::Instruction(std::string instructionKey) {
	// Store the instruction Key: this can't be changed anymore
	instruction = instructionKey;
	lockList = "";
	processTime = 60+instructionKey[0] - 64;
}

Instruction::~Instruction() {

}

std::string Instruction::lockedWith(){
	return std::string();
}

std::string Instruction::getInstruction()
{
	return instruction;
}

int Instruction::getProcessTime()
{
	return processTime;
}

bool Instruction::isInstruction(std::string instr)
{
	/*
	std::cout << "DEBUG: Comparing stored instruction " << instruction << " to new instr " << instr 
		<< " ; Result of compare: " << instruction.compare(instr) << std::endl;
	*/

	// compare returns 0 if equal:
	if (instruction.compare(instr) == 0) {
		return true;
	}
	else {
		return false;
	}
}

void Instruction::addLock(std::string lockKey){
	// Check if lockKey is not yet part of the lockKey
	if (lockList.find(lockKey) == std::string::npos) {
		lockList.append(lockKey);
		std::sort(lockKey.begin(), lockKey.end());
	}
	
}

std::string Instruction::getLock() {
	return lockList;
}

bool Instruction::isUnlocked(std::string key) {
	// check if lockList is contained in key
	bool isUnLocked = true; // per default unlocked? WTH! ;)
	for (int i = 0; i < lockList.size(); ++i) {
		if (key.find(lockList[i]) == std::string::npos) {
			isUnLocked = false;
		}
	}
	return isUnLocked;
}

