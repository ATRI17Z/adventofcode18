#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>


std::string getInputAsString(std::string);
std::vector<std::string> getInputPerLines(std::string);

int getRootValue(std::list<int>, std::list<int>::iterator&);
int getMetaData(std::list<int>, std::list<int>::iterator&);

//DEBUG
//int level = 0;

int main() {
	std::string fileName = "input_Day8.txt";
	std::list<int> dataList;
	std::list<int>::iterator dataIter;
	int data;
	int sumMeta = 0;
	int valueRoot = 0;
	

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
		return 0;
	}

	// Read data to list
	while (in >> data) {
		dataList.push_back(data);		
	}

	dataIter = dataList.begin();
	sumMeta = getMetaData(dataList, dataIter);

	std::cout << "PART ON: Meta Data Sum: " << sumMeta << std::endl;

	dataIter = dataList.begin();
	valueRoot = getRootValue(dataList, dataIter);

	std::cout << "PART TWO: Root Node Value: " << valueRoot << std::endl;
}

int getRootValue(std::list<int> list, std::list<int>::iterator& it) {
	// get head
	int numChildren;
	int numMeta;
	int rootValue = 0;
	numChildren = *it;
	++it;
	numMeta = *it;
	++it;

	std::vector<int> RootValueChildren;
	// Process Children
	for (int i = 0; i < numChildren; ++i) {
		//++level;//DEBUG
		RootValueChildren.push_back(getRootValue(list, it));
		//--level;//DEBUG
		//std::cout << "DEBUG["<<level<<"]:  RVC[" << i << "]: " << RootValueChildren.back() << std::endl;
	}
	//std::cout << "DEBUG[" << level << "]:  RVC size: " << RootValueChildren.size() << std::endl;
	// Process MetaData
	int idx;
	if (numChildren == 0) {
		for (int i = 0; i < numMeta; ++i) {
			rootValue += *it;
			++it;
		}
		//std::cout << "DEBUG[" << level << "]: Adding metaData as sum: " << rootValue << std::endl;
	}else{
		for (int i = 0; i < numMeta; ++i) {
			idx = *it-1;
			// add RootValueChildren at idx to current rootValue
			if (idx < RootValueChildren.size()) {
				// add value of children to rootValue
				rootValue += RootValueChildren[idx];
				//std::cout << "DEBUG[" << level << "]: Adding Child Value of " << i << " to rootvalue: " << RootValueChildren[idx] << std::endl;
			}else {
				//skip

			}

			++it;
		}
	}

	return rootValue;
}

int getMetaData(std::list<int> list, std::list<int>::iterator& it) {
	//get head
	int numChildren;
	int numMeta;
	int sumMeta = 0;

	numChildren = *it;
	++it;
	numMeta = *it;
	++it;
	// Process Children
	for (int i = 0; i < numChildren; ++i) {
		sumMeta += getMetaData(list, it);
	}
	// Process MetaData
	for (int i = 0; i < numMeta; ++i) {
		sumMeta += *it;
		++it;
	}
	return sumMeta;
}



/******************************
*   INPUT HELPER FUNCTIONS   *
******************************/

std::string getInputAsString(std::string fileName) {
	std::string str;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
		return 0;
	}

	// Create String
	str.assign((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	in.close();
	return str;
}

std::vector<std::string> getInputPerLines(std::string fileName) {
	std::vector<std::string> lines;
	std::string line;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
	}

	// Create Vector of lines
	while (getline(in, line)) {
		lines.push_back(line);
	}
	in.close();
	return lines;
}