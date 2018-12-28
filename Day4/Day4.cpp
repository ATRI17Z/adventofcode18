
#include <iomanip>
#include <chrono>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
//#include <list>
#include <regex>
#include <algorithm>

enum guardEvent { onDuty, asleep, awake, unknown };
struct guardLogEntry
{
	int id;
	int YY,MM,DD,hh,mm;
	guardEvent guardStateChange;
};
struct guardEntry {
	int id;
	int sleepTime;
};

bool myLogBookSortTime(guardLogEntry a, guardLogEntry b);
bool myLogBookSortID(guardLogEntry a, guardLogEntry b);

/*
template <typename C>
void printClockData()
{
	using namespace std;

	cout << "- precision: ";
	// if time unit is less or equal one millisecond
	typedef typename C::period P;// type of time unit
	if (ratio_less_equal<P, milli>::value) {
		// convert to and print as milliseconds
		typedef typename ratio_multiply<P, kilo>::type TT;
		cout << fixed << double(TT::num) / TT::den
			<< " milliseconds" << endl;
	}
	else {
		// print as seconds
		cout << fixed << double(P::num) / P::den << " seconds" << endl;
	}
	cout << "- is_steady: " << boolalpha << C::is_steady << endl;
}
*/

int main()
{
	/*
	// Time Point and Clock Infos (DEBUG)
	std::cout << "system_clock: " << std::endl;
	printClockData<std::chrono::system_clock>();
	std::cout << "\nhigh_resolution_clock: " << std::endl;
	printClockData<std::chrono::high_resolution_clock>();
	std::cout << "\nsteady_clock: " << std::endl;
	printClockData<std::chrono::steady_clock>();
	*/


	// Read file input
	std::string line;
	std::ifstream infile("input_Day4.txt");
	

	// Regex Preparation: 
	//	[1518 - 04 - 27 23:50] Guard #661 begins shift
	//	[1518 - 08 - 29 00:58] wakes up
	//	[1518 - 09 - 26 00:48] falls asleep
	std::regex regex_num("([\\d]+)");
	std::regex regex_awake("wakes up");
	std::regex regex_asleep("falls asleep");
	std::smatch pieces_match;
	int id=0;

	// Store intermediate datea
	std::vector<guardLogEntry> guardLogBook;
	std::vector<guardEntry> guards;
	std::map<int, int> guardSleepTime;
	std::map<int, int[60]> guardSleepMinutes;
	
	while (std::getline(infile, line)) {
		//std::cout << "Got line " << line << std::endl;

		// Get all numbers
		std::regex_iterator<std::string::iterator> reg_it(line.begin(), line.end(), regex_num);
		std::regex_iterator<std::string::iterator> reg_end;
		guardLogEntry logEntry;
		logEntry.YY = std::stoi(reg_it->str(), nullptr, 10);
		++reg_it;
		logEntry.MM = std::stoi(reg_it->str(), nullptr, 10);
		++reg_it;
		logEntry.DD = std::stoi(reg_it->str(), nullptr, 10);
		++reg_it;
		logEntry.hh = std::stoi(reg_it->str(), nullptr, 10);
		++reg_it;
		logEntry.mm = std::stoi(reg_it->str(), nullptr, 10);
		++reg_it;
		if (reg_it != reg_end) {
			// new Guard
			logEntry.id = std::stoi(reg_it->str(), nullptr, 10);
			logEntry.guardStateChange = onDuty;
		}else {
			// current (unknown) Guard
			logEntry.id = -1;
			// check if it is a "fall asleep" or "wake-up" time
			if (std::regex_search(line, pieces_match, regex_asleep)) {
				// Fell asleep
				logEntry.guardStateChange = asleep;
			}
			else if (std::regex_search(line, pieces_match, regex_awake)) {
				// awake
				logEntry.guardStateChange = awake;
			}
			else {
				logEntry.guardStateChange = unknown;
			}
		}
		
		// Add Entry to logbook
		guardLogBook.push_back(logEntry);
		
	}

	// sort vector with respect to time
	std::sort(guardLogBook.begin(), guardLogBook.end(), myLogBookSortTime); // sort for time
	// Fill in correct state changes with correcy id
	id = 0;
	for (std::vector<guardLogEntry>::iterator it = guardLogBook.begin();
		it != guardLogBook.end(); ++it) {
		if (it->id == -1) {
			it->id = id;
		}
		else {
			id = it->id;
		}
	}
	// sort vector with respect to ID
	std::stable_sort(guardLogBook.begin(), guardLogBook.end(), myLogBookSortID); // sort for ID

	// DEBUG
	/*
	std::cout << "Sorted Vector: \n";
	for (std::vector<guardLogEntry>::iterator it = guardLogBook.begin();
		it != guardLogBook.end(); ++it) {
		std::cout << "\t" << it->YY << "-" << it->MM << "-" << it->DD << " "
			<< it->hh << ":" << it->mm << "-> id:" << it->id << " state: "
			<< it->guardStateChange << std::endl;
	}
	*/

	// Get Sleep time for all guards
	id = guardLogBook.begin()->id;
	int sleepTime = 0;
	for (std::vector<guardLogEntry>::iterator it = guardLogBook.begin();
		it != guardLogBook.end(); ++it) {
		// see if guard changed
		if (id != it->id) { id = it->id; sleepTime = 0; }
		else if (it->guardStateChange == asleep) { sleepTime = it->mm; }
		else if (it->guardStateChange == awake) {
			guardSleepTime[id] += (it->mm - sleepTime);

			for (int i = sleepTime; i < it->mm; ++i) {
				guardSleepMinutes[id][i]++;
			}
		}
		// DEBUG
		/*
		if (id == 2633) {
			std::cout << "\t" << it->YY << "-" << it->MM << "-" << it->DD << " "
				<< it->hh << ":" << it->mm << "-> id:" << it->id << " state: "
				<< it->guardStateChange << std::endl;
		}
		*/

	}

	// find guard with most sleep time
	int maxSleepTime = 0;
	id = 0;
	for (std::map<int, int>::iterator it = guardSleepTime.begin();
		it != guardSleepTime.end(); ++it) {
		//std::cout << " Guard #" << it->first << " has slept " << it->second << " min " << std::endl;
		if (it->second > maxSleepTime) {
			maxSleepTime = it->second;
			id = it->first;
		}
	}

	// Result Sleep Summary
	//std::cout << " Guard #" << id << " has slept " << maxSleepTime << " min " << std::endl;

	// See which minute he slept the most
	std::map<int, int[60]>::iterator it = guardSleepMinutes.find(id);
	//std::copy(std::begin(it->second), std::end(it->second), std::ostream_iterator<int>(std::cout, " "));
	int bestMinute = 0;
	int minAsleep = 0;
	for (int i = 0; i < 60; ++i) {
		if (minAsleep < it->second[i]) { minAsleep = it->second[i]; bestMinute = i; }
	}

	std::cout << "Guard #" << id << " has slept " << maxSleepTime << " min " 
		      << "and mostly during minute " << bestMinute 
		      << " -> Part One: " << bestMinute*id <<  std::endl;


	// PART TWO
	int mostFreqId=0;
	bestMinute=0;
	int maxFreq=0;
	for (std::map<int, int[60]>::iterator it = guardSleepMinutes.begin();
		it != guardSleepMinutes.end(); ++it) {
		for (int i = 0; i < 60; ++i) {
			if (maxFreq < it->second[i]) {
				maxFreq = it->second[i];
				bestMinute = i;
				mostFreqId = it->first;
			}
		}
	}

	std::cout << "Guard #" << mostFreqId << " has slept most frequently at Minute "
		<< bestMinute << " (" << maxFreq << " times)" 
		<< " -> Part Two: " << mostFreqId*bestMinute << std::endl;
	
	return 0;
}

bool myLogBookSortTime(guardLogEntry a, guardLogEntry b) {
	int tA, tB;
	tA = a.MM * 1000000 +
		a.DD * 10000 +
		a.hh * 100 +
		a.mm * 1;
	tB = b.MM * 1000000 +
		b.DD * 10000 +
		b.hh * 100 +
		b.mm * 1;
	return (tA < tB);
}

bool myLogBookSortID(guardLogEntry a, guardLogEntry b) {
	return (a.id < b.id);
}

