#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <math.h>
#include <chrono>
#include <algorithm>

typedef std::list<int> intList; // 2.7ms, 1.2s
//typedef std::vector<int> intList; // ??
//typedef std::deque<int> intList; // 4.7ms 28543.1s

unsigned long long playElvesMarbleGame(const int, const int);
intList::iterator getCyclicIterator(intList::iterator&, int, intList&);
intList::iterator next(intList::iterator&, intList&);
intList::iterator prev(intList::iterator&, intList&);

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

	// Test Cases
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
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << std::endl << "Winning Score Part One: " << score << std::endl;
	auto finishONE = std::chrono::high_resolution_clock::now();

	score = playElvesMarbleGame(players, last*100);
	auto finishTWO = std::chrono::high_resolution_clock::now();
	std::cout << std::endl << "Winning Score Part Two: " << score << std::endl;

	std::chrono::duration<double> elapsedONE = finishONE - start;
	std::chrono::duration<double> elapsedTWO = finishTWO - finishONE;
	std::cout << "Elapsed time: P1: " << elapsedONE.count()*1000 << "ms\t P2: " << elapsedTWO.count()*1000 << "ms" << std::endl;
	
	return 0;
}

unsigned long long playElvesMarbleGame(const int players, const int last) {
	std::vector<unsigned long long> playerScore(players, 0);
	intList circle;
	intList::iterator curMidx;// Index of current marble in vector
	const int divi = 23;
	const int off = -7;
	int curP;	// Current player
	
	// init
	circle.push_back(0);
	curP = 0;	
	curMidx = circle.begin();

	// start game
	auto start = std::chrono::high_resolution_clock::now();
	for (int curM = 1; curM <= last; ++curM) {
		// Check if we have a scoring player
		if (curM % divi == 0) { // got multiple of divisor divi -> scores are made
			curMidx = getCyclicIterator(curMidx, off,circle);
			playerScore[curP] += curM + *curMidx;
			curMidx = circle.erase(curMidx);
		}else {
			// place marble
			curMidx = getCyclicIterator(curMidx, 2, circle);
			curMidx = circle.insert(curMidx, curM);
		}
		// Assign new player for next round
		curP = ++curP % players; // player 1 has number 0
	}
	
	// Get Max score
	return *std::max_element(playerScore.begin(), playerScore.end());
}

intList::iterator getCyclicIterator(intList::iterator& curMidx, int off, intList& c) {

	if (off > 0){
		for (int i = 0; i < off; ++i) {
			curMidx = next(curMidx, c);
		}
	}else {	
		for (int i = 0; i < -off ; ++i) {
			curMidx = prev(curMidx, c);
		}
	}
	return curMidx;
}

intList::iterator next(intList::iterator& curMidx, intList& c) {
	if (++curMidx == c.end()) {
		return c.begin();
	}
	else {
		return curMidx;
	}
}

intList::iterator prev(intList::iterator& curMidx, intList& c) {
	if (curMidx == c.begin()) {
		curMidx =  c.end();
	}
	return --curMidx;
}



/* NICE SOLUTION using lambda's POSTED ON REDDIT by willkill07*/
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