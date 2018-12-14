#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <climits>

#include "Instruction.h"
#include "Worker.h"

class InstructionScheduler
{
public:
	InstructionScheduler();
	~InstructionScheduler();

	// Typedef
	typedef std::vector<Instruction>::iterator instrIter;
	typedef std::vector<Instruction> instrVec;

	// Public Methods
	void addInstruction(std::string a, std::string b); // do a before b
	void getSchedule();
	void getTimedSchedule(int numWorkers);

	void printInstructions();


private:
	// Private Methods
	instrIter planInstruction(instrIter it, std::string& key);
	void planTimedInstruction();
	
	bool WorkersBusy();

	void printSchedule();
	void printTimedSchedule();

	// Private Fields
	int currentTime;
	int numWorkers;
	std::string keyChain;
	std::vector<Worker>* workForce;
	instrVec instructionCopy;
	instrVec unscheduledInstructions;
	instrVec schedule;
	instrVec timedSchedule;
};

