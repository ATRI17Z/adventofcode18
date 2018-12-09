#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <math.h>
#include <chrono>
#include <algorithm>

unsigned long long playElvesMarbleGame(const int, const int);
int playFastElvesMarbleGame(const int, const int);
size_t getCyclicIndex(size_t, int, size_t);

//430 players; last marble is worth 71588 points
int main() {

	const int players = 430;
	const int last = 71588;
	unsigned long long score = 0;

	// Rules
	// - place new marble one right (cw) of last one
	// - placed marble becomes current marble
	// - mod23 marble is not placed and the one 7 left of the current one is removed -> add score to player,
	//   the marble right (cw) of the removed one becomes the current marble

	// Modulo of cyclic index test case
	//std::cout << getCyclicIndex(0, -1, 3) << std::endl;
	
	// Test Cases
	// OLD IMPLEMENTATION
	score = playElvesMarbleGame(9, 25);
	std::cout << "Winning Score Test 1: " << score << std::endl;
	score = playElvesMarbleGame(10, 1618);
	std::cout << "Winning Score Test 2: " << score << std::endl;
	score = playElvesMarbleGame(13, 7999);
	std::cout << "Winning Score Test 3: " << score << std::endl;
	score = playElvesMarbleGame(17, 1104);
	std::cout << "Winning Score Test 4: " << score << std::endl;
	score = playElvesMarbleGame(21, 6111);
	std::cout << "Winning Score Test 5: " << score << std::endl;
	score = playElvesMarbleGame(30, 5807);
	std::cout << "Winning Score Test 6: " << score << std::endl;

	score = playElvesMarbleGame(players, last);
	std::cout << std::endl << "Winning Score Part One: " << score << std::endl;

	score = playElvesMarbleGame(players, last*100);
	std::cout << std::endl << "Winning Score Part Two: " << score << std::endl;
	

	/*
	score = playFastElvesMarbleGame(9, 25);
	std::cout << "Winning Score Test 1: " << score << std::endl;
	score = playFastElvesMarbleGame(10, 1618);
	std::cout << "Winning Score Test 2: " << score << std::endl;
	score = playFastElvesMarbleGame(13, 7999);
	std::cout << "Winning Score Test 3: " << score << std::endl;
	score = playFastElvesMarbleGame(17, 1104);
	std::cout << "Winning Score Test 4: " << score << std::endl;
	score = playFastElvesMarbleGame(21, 6111);
	std::cout << "Winning Score Test 5: " << score << std::endl;
	score = playFastElvesMarbleGame(30, 5807);
	std::cout << "Winning Score Test 6: " << score << std::endl;

	score = playFastElvesMarbleGame(players, last);
	std::cout << std::endl << "Winning Score Part One: " << score << std::endl;

	score = playFastElvesMarbleGame(players, last * 100);
	std::cout << std::endl << "Winning Score Part Two: " << score << std::endl;
	*/
	
}


int  playFastElvesMarbleGame(const int players, const int last) {
	std::vector<int> playerScore(players, 0);
	std::deque<int> circle;
	const int divi = 23;
	const int off = -7;
	int curP;

	// init
	circle.push_back(0);
	curP = 0;

	for (int curM = 1; curM <= last; ++curM) {
		if (curM%divi == 0) {
			std::rotate(circle.begin(), circle.end() - 7, circle.end());
			playerScore[curP] += curM + circle.back();
			circle.pop_back();
			std::rotate(circle.begin(), circle.begin() + 1, circle.end());
		}else {
			if (circle.size() > 1) {
				std::rotate(circle.begin(), circle.begin() + 1, circle.end());
			}
			circle.push_back(curM);
		}

		// DEBUG Prints
		/*
		std::cout << "[" << divi % players << "] ";
		for (std::deque<int>::iterator it = circle.begin(); it != circle.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
		*/

		// Assign new player for next round
		curP = ++curP % players; // player 1 has number 0

	}
	// Get Max score
	int maxScore = 0;
	for (std::vector<int>::iterator it = playerScore.begin(); it != playerScore.end(); ++it) {
		if (*it > maxScore) {
			maxScore = *it;
		}
	}

	return maxScore;

}

unsigned long long playElvesMarbleGame(const int players, const int last) {
	std::vector<unsigned long long> playerScore(players, 0);
	std::vector<int> circle;
	const int divi = 23;
	const int off = -7;
	int curP;	// Current player
	int curM;	// Current marble
	size_t curMidx;// Index of current marble in vector
	size_t newIdx; // tmp Index
	std::vector<int>::iterator it; // tmp Iterator
	

	// init
	circle.push_back(0);
	curP = 0;	
	curM = 1;
	curMidx = 0;

	// start game
	auto start = std::chrono::high_resolution_clock::now();
	for (int curM = 1; curM <= last; ++curM) {
		// Check if we have a scoring player
		if (curM % divi == 0) { // got multiple of divisor divi -> scores are made
			//std::cout << "Player Scores\n";
			newIdx = getCyclicIndex(curMidx, off, circle.size());
			playerScore[curP] += curM + circle[newIdx];

			//DEBUG
			//std::cout << "P[" << curP << "]="<<playerScore[curP]<<" += " << curM << " + " << circle[newIdx] << " = " << curM + circle[newIdx];
			//std::cout << " at curMidx = " << curMidx << std::endl;

			it = circle.erase(circle.begin() + newIdx);
			curMidx = it - circle.begin();
		}else {
			// place marble
			newIdx = getCyclicIndex(curMidx, 2,  circle.size());
			if (newIdx == 0) {
				circle.push_back(curM);
				curMidx = circle.size()-1;
			}else {
				it = circle.insert(circle.begin() + newIdx, curM);
				curMidx = it - circle.begin();
			}
		}

		// DEBUG Prints
		/*
		std::cout << "[" << curP << "] ";
		for (std::vector<int>::iterator it = circle.begin(); it != circle.end(); ++it) {
			if (curMidx == it - circle.begin()) {
				std::cout << "(" << *it << ") ";
			}else {
				std::cout << *it << " ";
			}
		}
		std::cout << std::endl;
		*/


		// Assign new player for next round and get new marble
		curP = ++curP % players; // player 1 has number 0
		/*
		if (curM % 71588 == 0) {
			auto lapTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = lapTime - start;
			start = lapTime; // reset for relative counter
			std::cout << ". " << elapsed.count() << "s" << std::endl;;
		}
		*/
	}
	

	// Get Max score
	unsigned long long maxScore = 0;
	for (auto it = playerScore.begin(); it != playerScore.end(); ++it) {
		if (*it > maxScore) {
			maxScore = *it;
		}
	}

	return maxScore;
}

// returns the index to the new element respecting the size of the vector, as if it
// were a cyclic buffer
size_t getCyclicIndex(size_t idx, int off, size_t arrSize) {
	/*
	size_t newIdx;
	if ((int)idx + off < 0) {
		newIdx = arrSize + (((int)idx + off) % arrSize);
		std::cout << "New cyclic index from arr[" << arrSize << "] at (" << (int)idx + off << ") -> " << newIdx << std::endl;
	}else {
		newIdx = ((int)idx + off) % arrSize;
	}
	return newIdx;
	*/

	bool wasNeg = false;
	int nidx = (int)idx + off;
	if (nidx < 0) {
		wasNeg = true;
		nidx = -nidx;
	}
	int offset = nidx % arrSize;
	return (wasNeg) ? (arrSize - offset) : (offset);
}

/* NICE SOLUTION POSTED ON REDDIT by willkill07*/
/*
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

int main() {
  int players, marbles;
  scanf("%d players; last marble is worth %d points", &players, &marbles);
  // uncomment for part 2
  // marbles *= 100;
  std::list<int> m;
  m.push_back(0);
  auto next = [&] (auto i) {
                if (++i == m.end())
                  return m.begin();
                return i;
              };
  auto prev = [&] (auto i) {
                if (i == m.begin())
                  i = m.end();
                return --i;
              };

  std::vector<unsigned long long> p (players);
  auto curr = m.begin();
  for (int i = 1; i < marbles; ++i) {
    if (i % 23 == 0) {
      curr = prev(prev(prev(prev(prev(prev(prev(curr)))))));
      p[i % players] += i + *curr;
      curr = m.erase(curr);
    } else {
      curr = m.insert(next(next(curr)), i);
    }
  }
  std::cout << *std::max_element(p.begin(), p.end()) << '\n';
  return 0;
}
*/