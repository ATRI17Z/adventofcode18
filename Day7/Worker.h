#pragma once

#include <string>
#include <vector>

#include "Instruction.h"

class Worker
{
public:
	Worker();
	~Worker();

	void assignTask(std::string task, int duration);
	void taskEnded();
	int isBusyUntil();
	int isBusy();
	void stopWorking();
	std::string currentTask();

private:
	int busyUntil;
	bool isWorking;
	std::string workingOn;
	std::vector<Instruction>::iterator instrIter;
};

