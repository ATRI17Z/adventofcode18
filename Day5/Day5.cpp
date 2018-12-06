#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <iterator>
#include <algorithm>
#include <chrono>

std::string::iterator removeStringMatches(std::string& str, std::string::iterator it);

int main() {
	auto start = std::chrono::high_resolution_clock::now();

	std::string str;
	std::string strBackup;
	std::size_t stringLength;

	// Access file with input data: input.txt
	std::ifstream inputfile("input_Day5.txt"); //open the file
	if (inputfile.is_open() && inputfile.good()) {
		str.assign((std::istreambuf_iterator<char>(inputfile)),
					std::istreambuf_iterator<char>());
	}
	else {
		std::cout << "Failed to open input.txt" << std::endl;
		return 0;
	}

	strBackup = str;
	stringLength = str.size();

	//std::cout << "String is " << stringLength << " long" << std::endl;

	/* WITH REGEX, easy to code but not efficient at all
	 *		... and I'm sure there is a better regex expression than mine
	 */
	/*
	std::regex regex_exp("(aA|Aa|bB|Bb|cC|Cc|dD|Dd|eE|Ee|fF|Ff|gG|Gg|hH|Hh|iI|Ii|jJ|Jj|kK|Kk|lL|Ll|mM|Mm|nN|Nn|oO|Oo|pP|Pp|qQ|Qq|rR|Rr|sS|Ss|tT|Tt|uU|Uu|vV|Vv|wW|Ww|xX|Xx|yY|Yy|zZ|Zz)");
	// Replace matched strings
	while (true)
	{
		str = std::regex_replace(str, regex_exp, "");
		if (stringLength > str.size()) {
			stringLength = str.size();
		}else {	break; }
		std::cout << "String is now " << stringLength << " long" << std::endl;
	}
	*/
	std::string::iterator strIter;
	for (strIter = str.begin(); strIter < --str.end(); ) {
		strIter = removeStringMatches(str, strIter);
		if (str.size() == 0) break;
	}

	/* PART ONE */
	std::cout << "PART ONE: Final string is " << str.size() <<" long " << std::endl;
	strBackup = str; // not sure if this is safe to do
	auto finishONE = std::chrono::high_resolution_clock::now();

	/* PART TWO */
	std::size_t minLenght = strBackup.size();
	for (int i = 65; i < 91; ++i) {
		// start fresh
		str = strBackup;
		// erase one character set
		str.erase(std::remove(str.begin(), str.end(), i), str.end());
		str.erase(std::remove(str.begin(), str.end(), i+32), str.end());
		// do algo for all new substrings
		std::string::iterator strIter;
		for (strIter = str.begin(); strIter < --str.end(); ) {
			strIter = removeStringMatches(str, strIter);
			if (str.size() == 0) break;
		}
		//std::cout << "Having removed " << (char)i << " leads to string with length: " << str.size() << std::endl;
		if (minLenght > str.size()) {
			minLenght = str.size();
		}
	}

	std::cout << "PART TWO: Shortest string is " << minLenght << " long" << std::endl;
	auto finishTWO = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsedONE = finishONE - start;
	std::chrono::duration<double> elapsedTWO = finishTWO - finishONE;
	std::cout << "Elapsed time: P1: " << elapsedONE.count()*1000 << "ms P2: " << elapsedTWO.count()*1000 <<"ms"<< std::endl;
}


// THIS METHOD DOES NOT HANLDE EMPTY STRINGS!
std::string::iterator removeStringMatches(std::string& str, std::string::iterator it) {

	// sanity check
	if (it >= str.begin() && (it + 1) < str.end()) {
		// this does not restrict itself to letters 
		if (*it - *(it + 1) == 32 || *it - *(it + 1) == -32) {
			// we have two adjacent matches
			// Erase character at position 'it' and the one after 'it'
			str.replace(it, it + 2, "");
			// check if new position matches the previous one
			if (it != str.begin()) {
				it = removeStringMatches(str, --it);
			}
		}else {
			++it;
		}
	}
	return it;
}