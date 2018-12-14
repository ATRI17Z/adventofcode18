#include <iostream>
#include <list>
#include <string>


int main() {

	int input = 505961;
	//int input   = 594142;
	size_t inputLength = 6;

	std::list<int> scoreboard;
	std::list<int> pattern;
	std::list<int>::iterator it,iter;



	// Init
	std::list<int>::iterator curIdx1, curIdx2, newIdx1, newIdx2;
	scoreboard.push_back(3);
	scoreboard.push_back(7);
	curIdx1 = scoreboard.begin();
	curIdx2 = scoreboard.begin(); ++curIdx2;

	// iterate
	int numRec = 2,numSteps1,numSteps2;
	int newScore;
	//while (numRec < input + 10) // Part 1
	bool doubleAdd = false;
	while (true)
	{
		// combine recipe score
		// Add new recipie to board
		newScore = *curIdx1 + *curIdx2;
		
		if (newScore > 9) {
			// separate and add
			scoreboard.push_back(newScore / 10); ++numRec;
			scoreboard.push_back(newScore % 10); ++numRec;
			//std::cout << "Score>9: " << (newScore / 10) << " and " << (newScore % 10) << std::endl;
			doubleAdd = true;
		}
		else {
			scoreboard.push_back(newScore); ++numRec;
			doubleAdd = false;
		}
		
		// draw new recipies
		numSteps1 = *curIdx1 + 1;
		numSteps2 = *curIdx2 + 1;
		newIdx1 = curIdx1;
		newIdx2 = curIdx2;
		for (int i = 0; i < numSteps1; ++i) {
			++newIdx1;
			if (newIdx1 == scoreboard.end()) {
				newIdx1 = scoreboard.begin();
			}
		}
		/*
		while (newIdx1 == curIdx1 || newIdx1 == curIdx2) {
			++newIdx1;
			if (newIdx1 == scoreboard.end()) {
				newIdx1 = scoreboard.begin();
			}
		}
		*/
		for (int i = 0; i < numSteps2; ++i) {
			++newIdx2;
			if (newIdx2 == scoreboard.end()) {
				newIdx2 = scoreboard.begin();
			}
		}
		while ( newIdx2 == newIdx1) { //newIdx2 == curIdx1 || newIdx2 == curIdx2 ||
			++newIdx2;
			if (newIdx2 == scoreboard.end()) {
				newIdx2 = scoreboard.begin();
			}
		}

		// switch iteraotrs
		curIdx1 = newIdx1;
		curIdx2 = newIdx2;

		// Show scoreboard
		/*
		for (std::list<int>::iterator it = scoreboard.begin(); it != scoreboard.end(); ++it) {
			if (it == curIdx1) {
				std::cout << "(" << *it << ") ";
			}
			else if (it == curIdx2) {
				std::cout << "[" << *it << "] ";
			}
			else {
				std::cout << *it << " ";
			}
			
		}
		std::cout << std::endl;
		*/
		if (scoreboard.size() == inputLength) {
			iter = scoreboard.begin();
			for (int i = 0; i < inputLength;++i) {
				pattern.push_back(*iter++);
			}
			std::cout << "Initial pattern done"<<std::endl;
		}else if (scoreboard.size() > inputLength){
			int key;
			it = scoreboard.end();
			--it;
			if (it == iter) std::cout << "it=iter" << std::endl;
			key = *it;--it;
			key += *it * 10; --it;
			key += *it * 100; --it;
			key += *it * 1000; --it;
			key += *it * 10000; --it;
			key += *it * 100000;

			if (key == input) {
				std::cout << "Part 2: " << scoreboard.size() - 6 << std::endl;
				break;
				//270'157'146 (too high)
			}

			if (doubleAdd) {
				it = scoreboard.end();
				--it; --it;
				key = *it; --it;
				key += *it * 10; --it;
				key += *it * 100; --it;
				key += *it * 1000; --it;
				key += *it * 10000; --it;
				key += *it * 100000;

				if (key == input) {
					std::cout << "Part 2 DD: " << scoreboard.size() - 6 << std::endl;
					break;
					//20'231'867 (too high)
				}
			}
		}

	}

	// Display last ten receipies
	it = scoreboard.end();
	for (int i = 0; i < 10; ++i) {
		--it;
	}
	for (; it != scoreboard.end(); ++it) {
		std::cout << *it;
	}
	std::cout << std::endl;

	// create string from list
	std::string str = "";
	for (it = scoreboard.begin(); it != scoreboard.end(); ++it) {
		str += std::to_string(*it);
	}
	size_t pos = str.find("505961", 0); // 20231866 TEST
	std::cout << "P2 Pos: " << pos << std::endl;
	

	/*
	for (std::list<int>::iterator it = scoreboard.begin(); it != scoreboard.end(); ++it) {
		if (it == curIdx1) {
			std::cout << "(" << *it << ") ";
		}
		else if (it == curIdx2) {
			std::cout << "[" << *it << "] ";
		}
		else {
			std::cout << *it << " ";
		}

	}
	std::cout << std::endl;
	*/
}


