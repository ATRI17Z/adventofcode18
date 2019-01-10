#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <tuple>

// LOC:  [x,y]
// ROOM: [x,y,dist]
using loc = std::tuple<int, int>;
using room = std::tuple<int, int, int>;

int main();

room getNewRoom(int&, int&, int&, char);
std::list<room> processBranch(std::stringstream&, std::list<room>, std::map<loc, int>&);
void printMap(std::map<loc, int>);
void printMax(std::map<loc, int>);
std::string getInputAsString(std::string);
std::vector<std::string> getSingleWayInstructions(std::string);

int main() {

	std::vector<std::string> parsedDirections;
	
	std::string input;

	// Search for all direction expressions
	input = getInputAsString("input_Day20.txt");
	//for (auto& s : input) { std::cout << s; } std::cout << std::endl;
	//parsedDirections = getSingleWayInstructions(getInputAsString("input_Day20_test1.txt"));
	//for (auto& s : parsedDirections) { std::cout << s << std::endl; } std::cout << std::endl; // DEBUG: Sol: 18
	//parsedDirections = getSingleWayInstructions(getInputAsString("input_Day20_test2.txt"));
	//for (auto& s : parsedDirections) { std::cout << s << std::endl; } std::cout << std::endl; // DEBUG: Sol: 23
	//parsedDirections = getSingleWayInstructions(getInputAsString("input_Day20_test3.txt"));
	//for (auto& s : parsedDirections) { std::cout << s << std::endl; } std::cout << std::endl; // DEBUG: Sol: 31
	/* CASE 1: ^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$
	      
				   ENNWSWWNEWSSSSEENWNSEEESWENNNN	
				   ENNWSWWSSSEENWNSEEESWENNNN
				   ENNWSWWNEWSSSSEENEESWENNNN
				   ENNWSWWSSSEENEESWENNNN
				   ENNWSWWNEWSSSSEENWNSEEENNN
				   ENNWSWWSSSEENWNSEEENNN
				   ENNWSWWNEWSSSSEENEENNN
				   ENNWSWWSSSEENEENNN
				   
	  CASE 2: ^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$ 				   
			    
				   ESSWWNE (8 copies)
				   ESSWWNNNENNEESSWNSESSS (2 copies)
				   ESSWWNNNENNWWWSSSSESW (2 copies)
				   ESSWWNNNENNEESSSSS (2 copies)
				   ESSWWNNNENNWWWSSSSENNNE (2 copies)

	  CASE 3: ^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$

			   WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWSE)))
			    WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWNE))
				 WSSEESWWWNW(S|NENNEEEENN(ESSSSWNWSW|WSWWNE))
				  ...
				 WSSEESWWWNW(S|NENNEEEENN(ESSSSWSSEN|WSWWNE))
				  ...
				WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWNWWSE))
				 ...
			   WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWSSS)))
			    ...

				   */
	std::string directions;
	char directionStart = '^';
	char directionEnd = '$';
	size_t startPos = 0, endPos;
	startPos = input.find(directionStart, startPos);
	endPos = input.find(directionEnd, startPos);
	
	directions = (input.substr(startPos + 1, endPos - startPos - 1));
	std::cout << directions << std::endl << std::endl;

	// room = tuple<x,y,dist>
	int dist, x,y, optionDist=0, optionEDist = 0;
	room cur,tmp;
	std::map<loc,int> rooms;// change to set?
	std::stringstream ss;
	std::list<room> optionStart;
	std::list<std::list<room>> optionBranchPoints;
	std::list<room> branchPoints, branchTails, headPoints, trailsEnd;
	ss << directions;

	// inital room at x=0, y=0, dist=0:
	x = 0, y = 0;
	dist = 0;
	cur = std::make_tuple(x, y, dist);
	rooms[std::make_tuple(x, y)] = dist;

	headPoints.push_back(cur);

	
	trailsEnd =  processBranch(ss, headPoints, rooms);
	

	// Find room with maximum Distance
	auto maxD_it = std::max_element(rooms.begin(), rooms.end(),
		[](const auto& r1, const auto& r2) {
		return r1.second < r2.second; });

	std::cout << "Max room distance: " << maxD_it->second << std::endl;
	// Sol Part1: 4239

	printMap(rooms);

	// Find number of rooms with a distance of 1000 and more
	int roomCount = 0;
	for (auto it = rooms.begin(); it != rooms.end(); ++it) {
		if (it->second >= 1000) ++roomCount;
	}
	std::cout << "Rooms with dist of 1000 and more: " << roomCount << std::endl;
	
	


	return 0;
}

/******************************
*      HELPER FUNCTIONS       *
******************************/

std::list<room> processBranch(std::stringstream& ss, std::list<room> trailHeads, std::map<loc, int>& rooms) {
	bool optionEnd = false;
	bool optionStart = false;
	char c;
	int x, y, dist, recurDist;
	room cur, tmp;
	std::list<room> branchPoints, branchStart, branchTails, optionPoints;
	
	branchStart = trailHeads;

	while (ss.get(c)) {
		for (std::list<room>::iterator it = trailHeads.begin(); it != trailHeads.end(); ++it) {
			cur = *it; // Get current trail head
			std::tie(x, y, dist) = cur;
		

			if (c == 'N' ||
				c == 'E' ||
				c == 'S' ||
				c == 'W') {
				// Get next room according to move direction
				tmp = getNewRoom(x, y, dist, c);
				// Check if we already have this room in our map
				std::map<loc, int>::iterator iter = rooms.find(std::make_tuple(x, y));
				if (iter == rooms.end()) {
					// If this room is not known yet: Add it to rooms map and set cur room to the new one
					rooms[std::make_tuple(x, y)] = dist;
					cur = tmp;
				}
				else {
					// This room has been visited already:
					//	- if new distance is shorter replace map entry
					//  - if old distance is shorter keep map entry
					recurDist = iter->second;
					if (recurDist < dist) {
						// Remove old room entry from list and add new
						rooms.erase(iter);
						rooms[std::make_tuple(x, y)] = recurDist;
						// Reset Distance:
						// THIS DOES NOT ACCOUNT FOR MAX DISTANCE IN CYCLIC LOOP!
						dist = recurDist;
					}
					else {//old entry of room in map is shorter : keep
						rooms.erase(iter);
						rooms[std::make_tuple(x, y)] = dist;
					}
					
					// Update location
					cur = tmp;
					

				}
				// Keep all new positions for all trailHeads
				branchPoints.push_back(cur);
			}
			else if (c == '(') {
				// start of an optional route
				optionStart = true;
			}
			else if (c == '|') {
				// option change indicator
				// save endpoint as new start point for continuation after option
				branchTails.push_back(cur);
				// Restart current option
				branchPoints = branchStart;
			}
			else if (c == ')') {
				// end of an optional route
				// save endpoint as new start point for continuation after option
				branchTails.push_back(cur);
				// Use branch tails as current branch points
				optionEnd = true;
				
			}
			//printMap(rooms);
			//printMax(rooms);
			//std::cout << "===========================" << std::endl;
			//std::cin.get();
		}

		if (optionStart) {
			// Start processBranch Recursion
			optionPoints = processBranch(ss, trailHeads, rooms);
			optionStart = false;
			branchPoints.insert(branchPoints.end(), optionPoints.begin(), optionPoints.end());
		}

		if (optionEnd) {
			// Remove duplicate trail heads
			branchTails.sort(); // TODO: This does not work due to different dist at same room! CHANGE!
			int xT, yT, distT;
			int xTprev, yTprev, distTprev;

			xTprev = INT_MAX;
			yTprev = INT_MAX;
			distTprev = 0;
			for (std::list<room>::iterator it = branchTails.begin(); it != branchTails.end();) {
				std::tie(xT, yT, distT) = *it;
				if (xT == xTprev && yT == yTprev) {
					// delete current element
					it = branchTails.erase(it);
				}
				else {
					++it;
					xTprev = xT;
					yTprev = yT;
					distTprev = distT;
				}
			}
			return branchTails;
		}

		trailHeads.clear();
		trailHeads = branchPoints;
		branchPoints.clear();

	}


	return branchTails;
}

room getNewRoom(int& x, int& y, int& dist, char c) {
	room tmp;
	dist += 1;
	if (c == 'N') {
		y += 1;
		tmp = std::make_tuple(x, y, dist);
	}
	else if (c == 'E') {
		x += 1;
		tmp = std::make_tuple(x, y, dist);
	}
	else if (c == 'S') {
		y -= 1;
		tmp = std::make_tuple(x, y, dist);
	}
	else if (c == 'W') {
		x -= 1;
		tmp = std::make_tuple(x, y, dist);
	}
	else {
		std::cout << "WARNING: Unknown room direction" << std::endl;
	}
	return tmp;
}

void printMap(std::map<loc, int> rooms) {
	int maxX = 0, maxY = 0, minX=INT_MAX, minY=INT_MAX;
	int x, y, dist;
	for (std::map<loc, int>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
		std::tie(x, y) = it->first;
		dist = it->second;
		if (x > maxX) maxX = x;
		if (x < minX) minX = x;
		if (y > maxY) maxY = y;
		if (y < minY) minY = y;
	}

	int dX = maxX - minX+1;
	int dY = maxY - minY+1;
	int offX = minX, offY = minY;
	std::vector<std::vector<int>> charta(dY, std::vector<int>(dX, 0));
	for (std::map<loc, int>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
		std::tie(x, y) = it->first;
		charta[(size_t)minY-(size_t)y+dY-1][(size_t)x- (size_t)minX] = it->second;
	}

	for (std::vector<std::vector<int>>::iterator it = charta.begin(); it != charta.end(); ++it) {
		for (std::vector<int>::iterator at = it->begin(); at != it->end(); ++at) {
			std::cout << std::setw(2) << *at << " ";
		}
		std::cout << std::endl;
	}


	return;
}

void printMax(std::map<loc, int> rooms) {
	int maxDist = 0;

	for (auto& e : rooms) {
		if (e.second > maxDist) maxDist = e.second;
	}
	std::cout << "Current Max Room Distance: " << maxDist << std::endl;

	return;
}

// This function extracts all possible path instructions separating each branch in a new instruction
// This works in theory but gets way too slow and too memory intensive to use on normal problem size
std::vector<std::string> getSingleWayInstructions(std::string input) {
	std::vector<std::string> parsedDirections;
	std::list<std::string> rawDirections;

	char directionStart = '^';
	char directionEnd = '$';
	char closing = ')';
	char opening = '(';
	char option = '|';
	size_t startPos = 0, endPos, optionsPos;
	std::list<std::string> optionsStrings;
	std::string bracket, optionStr, newOption;

	// This While loop is not needed since only one directions in the input
	// so also rawDirections would not need to be a list!
	while (true) {
		startPos = input.find(directionStart, startPos);
		if (startPos == std::string::npos) break;
		endPos = input.find(directionEnd, startPos);
		if (endPos == std::string::npos) break;

		rawDirections.push_back(input.substr(startPos + 1, endPos - startPos - 1));
		// Start after found directions
		startPos = endPos + 1;
	}

	// Parse instruction to generate all different possibilities

	// Find and clear brackets
	for (std::list<std::string>::iterator it = rawDirections.begin(); it != rawDirections.end(); ) {
		while (true) {
			startPos = it->rfind(opening, std::string::npos); // Find first '(' from back
			if (startPos == std::string::npos) break; // no more brackets
			endPos = it->find(closing, startPos);
			if (endPos == std::string::npos) break;

			// content of found bracket
			bracket = it->substr(startPos + 1, endPos - startPos - 1);


			while (true) {

				optionsPos = bracket.find(option, 0);
				if (optionsPos == std::string::npos) {
					it = rawDirections.erase(it);
					break;
				}
				optionStr = bracket.substr(0, optionsPos);
				if (!(optionStr.size() == 0)) {
					// replace <bracket> with <optionStr> in <s> and put it back into the rawDirections list
					newOption = *it;
					newOption = newOption.replace(startPos, endPos - startPos + 1, optionStr);
					rawDirections.push_back(newOption);

					// remove options string <optionsStr> from <bracket> string.
					bracket.replace(0, optionStr.size(), "");
				}
				else {
					// Check if there is an non-empty option after the last '|' 
					if (bracket.size() > 1) {
						// replace the <bracket> with the part after the '|' in <s> and put it back into the rawDirections list
						optionStr = bracket.replace(0, 1, "");
						newOption = *it;
						newOption = newOption.replace(startPos, endPos - startPos + 1, optionStr);
						rawDirections.push_back(newOption);
					}
					else {
						newOption = *it;
						newOption = newOption.erase(startPos, endPos - startPos + 1);
						rawDirections.push_back(newOption);

						// remove options string <optionsStr> from <bracket> string. // not needed
						bracket.replace(0, 1, "");
					}
				}
			}
		}
		++it;
	}

	// REMOVE similar strings
	// Copy to vector
	for (auto& s : rawDirections) {
		parsedDirections.push_back(s);
	}

	// Sort for easier removal
	std::sort(parsedDirections.begin(), parsedDirections.end());

	// Remove duplicates
	for (std::vector<std::string>::iterator it = parsedDirections.begin(); it != parsedDirections.end();) {
		if ((it + 1) != parsedDirections.end()) {
			//std::cout << "Comparing " << *it << " with " << *(it + 1);
			if (it->compare(*(it + 1)) == 0) { // equal
				//std::cout << " -> removing" << std::endl;
				it = parsedDirections.erase(it);
				continue;
			}
			else {
				//std::cout << " -> keeping" << std::endl;
			}
		}
		++it;
	}
	return parsedDirections;
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
		return std::string();
	}

	// Create String
	str.assign((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	in.close();
	return str;
}
