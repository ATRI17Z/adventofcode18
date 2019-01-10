#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void printMap(const std::vector<std::vector<int>>&);

int main() {


	// Access file with input data : input.txt
	std::ifstream inputfile("input_Day18.txt"); //open the file
	if (inputfile.is_open() && inputfile.good()) {
		// all good
		std::cout << "File open" << std::endl;
	}
	else {
		std::cout << "Failed to open input" << std::endl;
		return 0;
	}

	std::string line;
	//int nCols = 10; // Test input
	//int nRows = 10;
	int nCols = 50;
	int nRows = 50;
	std::vector<std::vector<int>> map(nCols, std::vector<int>(nRows, 0));
	std::vector<std::vector<int>> map_next(nCols, std::vector<int>(nRows, 0));
	int row = 0;
	while (getline(inputfile, line)) {
		int col = 0;
		for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
			//std::cout << "Adding element : " << col << " x " << row << std::endl;
			if (*it == ' ')
			{
			}
			else if (*it == '.') {
				map[col][row] = 1;
				//std::cout << "open" << std::endl;
			}
			else if (*it == '|') {
				map[col][row] = 2;
				//std::cout << "tree" << std::endl;
			}
			else if (*it == '#') {
				map[col][row] = 3;
				//std::cout << "lumber" << std::endl;
			}
			++col;
		}
		++row;
	}

	// Print Map
	printMap(map);

	// update map
	int dx[] = { -1,-1,0,1,1,1,0,-1 };
	int dy[] = { 0,1,1,1,0,-1,-1,-1 };
	int newX, newY, compVal, compNum, compCount, compCount2;
	int numT, numL;
	int oldNumT = 0, oldNumL = 0;

	//for (int i = 0; i < 10; ++i) { //Part one
	for (int i = 0; i < 1000000000; ++i) { // Part two
		numT = 0; numL = 0;
		for (int c = 0; c < nCols; ++c) {
			for (int r = 0; r < nRows; ++r) {
				compCount = 0;
				compCount2 = 0;
				if (map[c][r] == 1) { // if we have an open space
					compVal = 2;
					compNum = 3;
					for (int d = 0; d < 8; ++d) {
						newX = c + dx[d];
						newY = r + dy[d];
						if (newX >= 0 && newY >= 0 && newX < nCols && newY < nRows) {
							// we can count
							if (map[newX][newY] == compVal) ++compCount;
						}
					}
					if (compCount >= compNum) {
						map_next[c][r] = compVal;
						++numT;
					}
					else {
						map_next[c][r] = map[c][r];
					}

				}
				else if (map[c][r] == 2) { // if we have a tree
					compVal = 3;
					compNum = 3;
					for (int d = 0; d < 8; ++d) {
						newX = c + dx[d];
						newY = r + dy[d];
						if (newX >= 0 && newY >= 0 && newX < nCols && newY < nRows) {
							// we can count
							if (map[newX][newY] == compVal) ++compCount;
						}
					}
					if (compCount >= compNum) {
						map_next[c][r] = compVal;
						++numL;
					}
					else {
						map_next[c][r] = map[c][r];
						++numT;
					}
				}
				else if (map[c][r] == 3) { // if we have a lumberyard
					// stays if there is at least a lumberyard adjacent
					// and a tree, otherwise open
					for (int d = 0; d < 8; ++d) {
						newX = c + dx[d];
						newY = r + dy[d];
						if (newX >= 0 && newY >= 0 && newX < nCols && newY < nRows) {
							// we can count
							if (map[newX][newY] == 3) ++compCount;
							else if (map[newX][newY] == 2) ++compCount2;
						}
					}
					if (compCount >= 1 && compCount2 >= 1) {
						map_next[c][r] = map[c][r];
						++numL;
					}
					else {
						map_next[c][r] = 1;
					}
				}

			}
		}
		map = map_next; // Update map
		//std::cout << "Map after " << i+1 << " min: " << std::endl;
		//printMap(map);
		if (!(i % 10000)) {
			std::cout << "Minute: " << i << std::endl;
		}

		// Print intermediate results
		//std::cout << "Min[" << i << "]:";
		//std::cout << "T: " << oldNumT << ">" << numT << " delta: " << numT - oldNumT;
		//std::cout << "\tL: " << oldNumL << ">" << numL << " delta: " << numL - oldNumL;
		//std::cout <<"\tprod:" << oldNumL * oldNumT << ">" << numL * numT << " delta: " << (numL * numT)-(oldNumL * oldNumT)<< std::endl;


		if (numL*numT == 207080) {
			std::cout << "Min[" << i << std::endl;
		}
		// Part Two done by hand: The results has a returning pattern of 28 iterations
		// after the 483 minute
		if (i > 482) {
			std::cout << "Min[" << i << "Res: " << numL * numT << std::endl;
		}


		oldNumL = numL;
		oldNumT = numT;
	}

	// count result
	/*
	numT = 0; numL = 0;
	for (int c = 0; c < nCols; ++c) {
		for (int r = 0; r < nRows; ++r) {
			if (map[c][r] == 2) ++numT;
			else if (map[c][r] == 3) ++numL;
		}
	}
	*/
	//std::cout << "P1: trees("<<numT<<")*lumberyard("<<numL<<") = " << numT * numL << std::endl;

	return 0;
}


void printMap(const std::vector<std::vector<int>>& map) {
	int nCols = (int)map.size();
	int nRows = (int)map.front().size();
	for (int i = 0; i < nRows; ++i) {
		for (int j = 0; j < nCols; ++j) {
			if (map[j][i] == 1) { // free space
				std::cout << ".";
			}
			else if (map[j][i] == 2) { // tree
				std::cout << "|";
			}
			else if (map[j][i] == 3) { // lumberyard
				std::cout << "#";
			}
			else { // undef
				// do nothing
				std::cout << map[j][i];
			}
		}
		std::cout << std::endl;
	}

}