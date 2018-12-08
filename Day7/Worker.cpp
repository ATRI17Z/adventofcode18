#include "Worker.h"



Worker::Worker()
{
	busyUntil = 0;
	workingOn = "";
	isWorking = false;
}


Worker::~Worker()
{
}

void Worker::assignTask(std::string task, int duration) {
	busyUntil = duration;
	workingOn = task;
	isWorking = true;
}

void Worker::taskEnded() {
	busyUntil = 0;
	workingOn = "";
	isWorking = false;
}

int Worker::isBusyUntil() {
	return busyUntil;
}

int Worker::isBusy()
{
	return isWorking;
}

void Worker::stopWorking()
{
	isWorking = false;
}

std::string Worker::currentTask() {
	return workingOn;
}
