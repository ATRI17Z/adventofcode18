#include "InstructionScheduler.h"



InstructionScheduler::InstructionScheduler()
{
}


InstructionScheduler::~InstructionScheduler()
{
}

void InstructionScheduler::addInstruction(std::string a, std::string b) {
	std::cout << "Adding Instruction " << b << " which is locked by " << a << std::endl;
	// check if <b> exists,
	bool lockAisInstr = false;
	bool instrBexists = false;
	for (std::vector<Instruction>::iterator it = unscheduledInstructions.begin();
		it != unscheduledInstructions.end(); ++it) {
		// compare each entry of the unscheduled instruction set to instruction <b>
		if (it->isInstruction(b)) {
			// <b> exists, add lockKey <a> to <b>
			//std::cout << "DEBUG: adding to existing " << b << " a new lock key: " << a << std::endl;
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
		//std::cout << "DEBUG: adding " << b << " as new Instruction with lock key " << a << std::endl;
		unscheduledInstructions.push_back(ins);
	}
	if (!lockAisInstr) {
		Instruction lck = Instruction(a);
		unscheduledInstructions.push_back(lck);
	}
	
	return;
}



void InstructionScheduler::getSchedule() {
	// Backup unscheduled Instructions if backup empty
	if (instructionCopy.size() == 0) {
		instructionCopy = unscheduledInstructions;
		std::cout << "Backup of Instructions created" << std::endl;
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
			//std::cout << "DEBUG: Iteration" << std::endl;
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

		/*
		std::cout << "DEBUG PROGRESS: " << std::endl;
		std::cout << "\t Unscheduled: " << std::endl;
		printUnscheduled();
		std::cout << "\t Scheduled: " << std::endl;
		printScheduled();
		std::cout << "\t newKEY: " << key << std::endl << std::endl;;
		*/

		
	}
	printSchedule();
}

void InstructionScheduler::getTimedSchedule(int numW) {
	// Backup unscheduled Instructions if backup empty
	if (instructionCopy.size() == 0) {
		instructionCopy = unscheduledInstructions;
		std::cout << "Backup of Instructions created" << std::endl;
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
	std::string key = "";
	std::string lastTask = "";
	
	while (unscheduledInstructions.size() > 0 || WorkersBusy()) {
		//std::cout << "DEBUG: unschSize: " << unscheduledInstructions.size() << " and isWorking " << WorkersBusy() << std::endl;
		// plann tasks
		lastTask = planTimedInstruction(key);
	}
	printTimedSchedule(lastTask);
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

void InstructionScheduler::printTimedSchedule(std::string lastTask) {
	std::cout << "Instruction Timed Schedule with " << numWorkers << " workers:\t";
	for (instrIter it = timedSchedule.begin();
		it != timedSchedule.end(); ++it) {
		std::cout << it->getInstruction();
	}
	std::cout << lastTask;
	std::cout << std::endl;
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

std::string InstructionScheduler::planTimedInstruction(std::string& key) {
	// - Add key only after having finished the job
	// - Don't care about timing as long as there is an idling worker

	// 1) Get time when next free worker is available
	// 2) Check which task(s) have finished by then and add its key to the keychain and the task to the schedule
	// 2) Check what tasks are now available
	// 3) Assign first task, if none is available shift time to next task finished

	int timeNextFreeWorker = INT_MAX;
	std::string lastTask = "";
	std::vector<Worker>::iterator lastWorkerFinished;

	// 1)
	int n = 0,idx=0;//DEBUG
	for (std::vector<Worker>::iterator workerIter = workForce->begin();
		workerIter != workForce->end(); ++workerIter) {
		//std::cout << "Worker[" << n << "] is busy("<< workerIter->isBusy() <<") until " << workerIter->isBusyUntil() << std::endl;//DEBUG
		
		if (!workerIter->isBusy()) {
			timeNextFreeWorker = currentTime; // workerIter->isBusyUntil();
			lastWorkerFinished = workerIter;
			idx = n;//DEBUG
			
		}
		n++;//DEBUG
	}
	//if no last worker assigned -> all are working
	// find next to be free
	if (timeNextFreeWorker == INT_MAX) {
		int timeNextTaskDone = INT_MAX;
		for (std::vector<Worker>::iterator workerIter = workForce->begin();
			workerIter != workForce->end(); ++workerIter) {
			if (timeNextTaskDone > workerIter->isBusyUntil()) {
				timeNextTaskDone = workerIter->isBusyUntil();
				lastWorkerFinished = workerIter;
			}
		}
		currentTime = timeNextTaskDone;
		lastWorkerFinished->stopWorking();
		//std::cout << "No task available, shifting time to " << timeNextTaskDone << std::endl;
	}else {
		currentTime = timeNextFreeWorker;
	}
	

	
	//std::cout << "Current Time  " << currentTime << std::endl;

	// 2)
	// add new Key to KeyChain
	std::string newKey = lastWorkerFinished->currentTask();
	lastWorkerFinished->taskEnded(); // Basically clear
	// Add finished task to schedule
	if (newKey.size() > 0) {
		//std::cout << "Adding new Key from finished task: " << newKey << std::endl;
		key += newKey;
		std::sort(key.begin(), key.end());
		timedSchedule.push_back(Instruction(newKey));
	}


	// 3) 
	bool noWorkAssigned = true;
	for (instrIter it = unscheduledInstructions.begin();
		it != unscheduledInstructions.end(); ) {
		// Check which tasks are unlocked
		if (it->isUnlocked(key)) {
			// task unlocked -> assign
			lastWorkerFinished->assignTask(it->getInstruction(),it->getProcessTime()+currentTime);
			//std::cout << "Assinging task " << it->getInstruction() << " with time " << it->getProcessTime() + currentTime << " to worker " << std::endl;
			// remove from unscheduled
			it = unscheduledInstructions.erase(it);
			noWorkAssigned = false;
			break; // to unlock next level of keys to scheduler
		}
		else {
			// If nothing has been erased, increment iterator
			++it;
		}
	}

	if (noWorkAssigned) {
		// shift time to next task getting done
		int timeNextTaskDone = INT_MAX;
		std::vector<Worker>::iterator nextWorkerToBeFinished;
		for (std::vector<Worker>::iterator workerIter = workForce->begin();
			workerIter != workForce->end(); ++workerIter) {
			if (workerIter->isBusy()) {
				timeNextTaskDone = workerIter->isBusyUntil();
				nextWorkerToBeFinished = workerIter;
				std::cout << "Intermediate Time Next Task Done " << timeNextTaskDone << std::endl;
			}
		}
		currentTime = timeNextTaskDone;
		std::cout << "Time Next Task Done " << currentTime << std::endl;
		nextWorkerToBeFinished->stopWorking();
		lastTask = nextWorkerToBeFinished->currentTask();
		std::cout << "No task available, shifting time to " << timeNextTaskDone << std::endl;
		//std::cout << "Next Task " << lastTask << std::endl;
	}
	

	/*
	std::cout << "DEBUG PROGRESS: " << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "Unscheduled: " << std::endl;
	printUnscheduled();
	std::cout << "----------------" << std::endl;
	std::cout << "Scheduled: " << std::endl;
	printTimedScheduled();
	std::cout << "----------------" << std::endl;
	std::cout << "newKEY: " << key << std::endl;
	std::cout << "----------------" << std::endl;
	std::cout << "time: " << currentTime << std::endl;
	printCurrentWork();
	std::cout << "----------------" << std::endl << std::endl;
	*/
	return lastTask;
}

bool InstructionScheduler::WorkersBusy() {
	bool someonesWorking = false;
	for (std::vector<Worker>::iterator workerIter = workForce->begin();
		workerIter != workForce->end(); ++workerIter) {
		if (currentTime < workerIter->isBusyUntil()) {
			someonesWorking = true;
		}
	}
	return someonesWorking;
}


// DEBUG
void InstructionScheduler::printUnscheduled() {
	for (instrIter it = unscheduledInstructions.begin();
		it != unscheduledInstructions.end(); ++it) {
		std::cout << "I: " << it->getInstruction() << " with lock ["
			<< it->getLock() << "]" << std::endl;
	}
}
void InstructionScheduler::printScheduled() {
	for (instrIter it = schedule.begin();
		it != schedule.end(); ++it) {
		std::cout << "I: " << it->getInstruction() << " with lock ["
			<< it->getLock() << "]" << std::endl;
	}
}

void InstructionScheduler::printTimedScheduled() {
	for (instrIter it = timedSchedule.begin();
		it != timedSchedule.end(); ++it) {
		std::cout << "I: " << it->getInstruction() << " with lock ["
			<< it->getLock() << "]" << std::endl;
	}
}

void InstructionScheduler::printCurrentWork() {
	int n = 0;
	for (std::vector<Worker>::iterator workerIter = workForce->begin();
		workerIter != workForce->end(); ++workerIter) {
		//Woker: n Task: x Until: y
		std::string task;
		int until;
		if (workerIter->isBusy()) {
			task = workerIter->currentTask();
			until = workerIter->isBusyUntil();
		}else {
			task = ".";
			until = 0;
		}
		std::cout << "Worker: " << n << " Task: " << task << " Until: " << until << std::endl;
	}
}
