#include "InstructionScheduler.h"



InstructionScheduler::InstructionScheduler()
{
}


InstructionScheduler::~InstructionScheduler()
{
}

void InstructionScheduler::addInstruction(std::string a, std::string b) {
	//std::cout << "Adding Instruction " << b << " which is locked by " << a;
	// check if <b> exists,
	bool lockAisInstr = false;
	bool instrBexists = false;
	for (std::vector<Instruction>::iterator it = unscheduledInstructions.begin();
		it != unscheduledInstructions.end(); ++it) {
		// compare each entry of the unscheduled instruction set to instruction <b>
		if (it->isInstruction(b)) {
			// <b> exists, add lockKey <a> to <b>
			it->addLock(a);
			instrBexists = true;
		}else if (it->isInstruction(a)) {
			// Check if the lock <a> even exists as a instruction, if not add it als instruction w/o lock
			lockAisInstr = true;
		}
	}
	if (!instrBexists) {
		// <b> does not exist, create <b> and lock <a> to it
		Instruction ins = Instruction(b, a);
		// add it du unscheduled list
		//std::cout << " and has a ProcTime " << ins.getProcessTime();
		unscheduledInstructions.push_back(ins);
	}
	if (!lockAisInstr) {
		Instruction lck = Instruction(a);
		//std::cout << std::endl << "Adding Instruction " << a << " unlocked and has a ProcTime " << lck.getProcessTime();
		unscheduledInstructions.push_back(lck);
	}
	//std::cout << std::endl;
	
	return;
}



void InstructionScheduler::getSchedule() {
	// Backup unscheduled Instructions if backup empty
	if (instructionCopy.size() == 0) {
		instructionCopy = unscheduledInstructions;
		//std::cout << "Backup of Instructions created" << std::endl;
	}else {
		unscheduledInstructions = instructionCopy;
	}
	
	// Search for starting point: key = ""
	std::string key = "";
	std::string newKey = "";
	// sort all unscheduled Instructions
	std::sort(unscheduledInstructions.begin(),
		unscheduledInstructions.end());
	while (unscheduledInstructions.size() > 0) {
		// find first unlockable instruction
		for (instrIter it = unscheduledInstructions.begin();
			it != unscheduledInstructions.end(); ) {
			if (it->isUnlocked(key)) {
				it = planInstruction(it, key);
				break; // to unlock next level of keys to scheduler
			}else {
				// If nothing has been erased, increment iterator
				++it;
			}
		}
		// Sort the new key
		std::sort(key.begin(), key.end());
	}
	printSchedule();
}

void InstructionScheduler::getTimedSchedule(int numW) {
	// Backup unscheduled Instructions if backup empty
	if (instructionCopy.size() == 0) {
		instructionCopy = unscheduledInstructions;
		//std::cout << "Backup of Instructions created" << std::endl;
	}
	else {
		unscheduledInstructions = instructionCopy;
	}
	// Update number of workers
	workForce = new std::vector<Worker>;
	numWorkers = numW;
	workForce->resize(numWorkers);
	currentTime = 0;

	// sort all unscheduled Instructions
	std::sort(unscheduledInstructions.begin(),
		unscheduledInstructions.end());

	// Search for starting point: key = ""
	keyChain = "";
	
	while (unscheduledInstructions.size() > 0 || WorkersBusy()) {
		planTimedInstruction();
	}

	printTimedSchedule();

	delete workForce;

}

void InstructionScheduler::printInstructions() {
	for (instrIter it = unscheduledInstructions.begin();
		 it != unscheduledInstructions.end(); ++it) {	
		std::cout << "I: " << it->getInstruction() << " with lock ["
			      << it->getLock() << "]" << std::endl;
	}
}

void InstructionScheduler::printSchedule() {
	std::cout << "Instruction Schedule:\t";
	for (instrIter it = schedule.begin();
		it != schedule.end(); ++it) {
		std::cout << it->getInstruction();
	}
	std::cout << std::endl;
}

void InstructionScheduler::printTimedSchedule() {
	std::cout << "Instruction Timed Schedule with " << numWorkers << " workers:\t";
	for (instrIter it = timedSchedule.begin();
		it != timedSchedule.end(); ++it) {
		std::cout << it->getInstruction();
	}
	std::cout << std::endl;
	std::cout << "All work is finished at: " << currentTime << std::endl;
}

InstructionScheduler::instrIter InstructionScheduler::planInstruction(instrIter it, std::string& key)
{
	// Add Instruction to schedule
	schedule.push_back(*it);
	// add this to schedule and remove from unscheduled
	key += it->getInstruction();
	// remove from unscheduledList
	it = unscheduledInstructions.erase(it);
	return it;
}

void InstructionScheduler::planTimedInstruction() {
	// - Add key only after having finished the job
	// - Don't care about timing as long as there is an idling worker

	// 1) Assign Tasks to all free workers as long there are unlocked tasks and free workers
	//    - if no free workers:			-> shift time to next job done
	//    - if no unlocked tasks:		-> shift time to next job done
	// 2) Check if task(s) have finished
	//    - if yes: add key(s) to keyChain
	//    - if no:						-> shift time to next job done


	// 1)
	int timeNextTaskFinished = INT_MAX;
	std::vector<Worker>::iterator nextTaskToFinish;
	std::vector<std::vector<Worker>::iterator> freeWorkers;
	std::vector<std::vector<Worker>::iterator> finishedTasks;
	std::vector<InstructionScheduler::instrVec::iterator> unlockedTasks;
	

	// Get List of free workers
	//std::cout << "Time: " << currentTime << std::endl;
	//std::cout << "Workers: ";
	for (std::vector<Worker>::iterator it = workForce->begin();	 it != workForce->end(); ++it) {
		//std::cout << it->isBusyUntil() << " ";
		if (!it->isBusy()) {
			freeWorkers.push_back(it);
		}else {
			if (timeNextTaskFinished > it->isBusyUntil()) {
				timeNextTaskFinished = it->isBusyUntil();
				nextTaskToFinish = it;
			}
		}
	}
	//std::cout << std::endl;
	// Get List of unlocked Tasks
	//std::cout << "UnschedTasks: ";
	for (instrIter it = unscheduledInstructions.begin(); it != unscheduledInstructions.end(); ++it) {
		// Check which tasks are unlocked
		if (it->isUnlocked(keyChain)) {
			unlockedTasks.push_back(it);
			//std::cout << "U ";
		}else {
			//std::cout << "L ";
		}
	}
	//std::cout << std::endl;

	// check if there are no free workers or no unlocked tasks
	if (freeWorkers.empty() || unlockedTasks.empty()) {
		// shift to next task to finish
		//std::cout << "Time Shift from "<< currentTime << " to " << nextTaskToFinish->isBusyUntil() << std::endl;
		currentTime = nextTaskToFinish->isBusyUntil();
		// HANDLE FINISHED TASK:
	}else {
		// Assign tasks as long as there are free Workers and unlocked tasks
		for (int i = 0; i < std::min(freeWorkers.size(), unlockedTasks.size()); ++i) {
			//std::cout << "Assigned " << unlockedTasks[i]->getInstruction();
			//std::cout << "(" << unlockedTasks[i]->getProcessTime() + currentTime << ")\n";
			// assign Tasks
			freeWorkers[i]->assignTask(unlockedTasks[i]->getInstruction(), unlockedTasks[i]->getProcessTime() + currentTime);
			// remove from unscheduled
			//printUnscheduled();
			unscheduledInstructions.erase(unlockedTasks[i]); // this renders iterator invalid
			break; // since all iterators in unlockedTasks are invalid now: there should be a better way
		}
		return;
	}

	// 2) Tasks finished
	// Get list of finished tasks
	for (std::vector<Worker>::iterator it = workForce->begin(); it != workForce->end(); ++it) {
		if (it->isBusy() && it->isBusyUntil() == currentTime) {
			finishedTasks.push_back(it);
		}
	}
	for (int i = 0; i < finishedTasks.size(); ++i) {
		// free worker, move task to scheduled, and add key to keyChain
		std::string newKey = finishedTasks[i]->currentTask();
		finishedTasks[i]->taskEnded();
		timedSchedule.push_back(Instruction(newKey));
		keyChain += newKey;
		//std::cout << "Adding Key: " << newKey << " -> " << keyChain << std::endl;
	}
	std::sort(keyChain.begin(), keyChain.end());
	if (finishedTasks.empty()) {
		//std::cout << "No finished Tasks\n";
	}
	//std::cout << std::endl << "=================================" << std::endl;
}

bool InstructionScheduler::WorkersBusy() {
	bool someonesWorking = false;
	for (std::vector<Worker>::iterator workerIter = workForce->begin();
		workerIter != workForce->end(); ++workerIter) {
		if (workerIter->isBusy()) {
			someonesWorking = true;
		}
	}
	return someonesWorking;
}

