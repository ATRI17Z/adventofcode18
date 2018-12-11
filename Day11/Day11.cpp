#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>


int getPowerLevel(int x, int y, int sid);


// TODO: last grid size not respected
// TODO: Indices one too low
int main() {


	int input = 6042;
	int gridSize = 300;
	int border,offMinus, offPlus;
	int minConvSize = 1;
	int maxConvSize = 300;

	

	// Test cases P1
	int result;
	result = getPowerLevel(3, 5, 8);
	std::cout << "T1 (4): " << result << std::endl;
	result = getPowerLevel(122, 79, 57);
	std::cout << "T2 (-5): " << result << std::endl;
	result = getPowerLevel(217,196,39);
	std::cout << "T3 (0): " << result << std::endl;
	result = getPowerLevel(101,153,71);
	std::cout << "T4 (4): " << result << std::endl;

	// Test Cases part 2
	//input = 18; // 90,269,16
	//input = 42; // 232,251,12

	// Real Task
	std::vector<std::vector<int>> data(300, std::vector<int>(300, 0));
	int maxP33 = INT_MIN;
	int iX, iY, nS;
	for (int s = minConvSize; s <= maxConvSize; ++s) { //DEBUG: should be from 1 to 300
		std::cout << "Grid: " << s << " ";
		std::vector<std::vector<int>> conv(300, std::vector<int>(300, 0)); // make sure of corner cases
		border = s / 2; // just need to check if it is even
		if (s % 2) { // odd grid size
			offMinus = -border;
			offPlus = border;
		}else {
			offMinus = -border;
			offPlus = border-1; // of even grid sizes the  square extends one more to the left and up
		}
		std::cout << "s: " << s << " border: " << border << std::endl;
		std::cout << "Summing for the range: " << border << " - " << gridSize - border << std::endl;
		std::cout << "Convolution size: " << offMinus << " -> " << offPlus << std::endl << std::endl;

		for (int i = 0; i < gridSize; ++i) {
			for (int j = 0; j < gridSize; ++j) {
				data[i][j] = getPowerLevel(i + 1, j + 1, input);
				
				// grid size 300 is not calculated here
				if (i < gridSize - border && j < gridSize - border && i>border && j>border) {
					for (int n = offMinus; n <= offPlus; ++n) {
						for (int m = offMinus; m <= offPlus; ++m) {
							conv[i + n][j + m] += data[i][j];
						}
					}
				}
			}
		}
		//find max number

		for (int i = border; i < gridSize-border; ++i) {
			for (int j = border; j < gridSize-border; ++j) {
				if (maxP33 < conv[i][j]) {
					maxP33 = conv[i][j];
					iX = i - border+1;
					iY = j - border+1;
					nS = s;

					if (s % 2) { // odd grid size
						iX += 1;
						iY += 1;
					}
				}
			}
		}
		std::cout << "Max: " << maxP33 << " at " << iX << "," << iY << "," << nS << std::endl;
	}

	
	//std::cout << "Max " << maxP33 << std::endl;
	//std::cout << "x,y: " << iX << "," << iY << std::endl;
	return 0;
}

int getPowerLevel(int x, int y, int sid) {
	int pLevel;
	int rackID;

	// Get Rack ID (x+10)
	rackID = x + 10;
	// Power = rackID *y
	pLevel = rackID * y;
	// Power = Power +SID
	pLevel += sid;
	// Power = Power * rackID
	pLevel *= rackID;
	// Keep hundret digit
	if (pLevel >= 100) {
		//std::cout << "P = " << pLevel << " ->";
		int rem = pLevel % 100;
		int num = pLevel - rem;
		num = num / 100;
		num = num % 10;
		pLevel = num;
		//std::cout << " 100-digit: " << pLevel << std::endl;
	}
	else {
		pLevel = 0;
	}


	// subtract five
	pLevel -= 5;

	return pLevel;
}

