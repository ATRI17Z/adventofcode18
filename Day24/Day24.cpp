#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <array>
#include <regex>
#include <algorithm>

#include "Squad.h"


/* TODO: PART TWO DOES NOT WORK ANYMORE AFTER RESTRUCTURING */

/* Puzzle Notes:
	- Two armys (immune system, infection) each consisting of groups
	- A group consists of one or more identical units
	- A group has an effective power:= number of units * attack damage of a single unit
	- Units have hit points (HP), attack damage, attack type, initiative, [weakness], [immunities]
	- Each battle has two phases: target selection, and attacking
	- Fight ends when only one army has units left

	TARGET Selection:
	- Each group choses one target, another group.
	- The group with the highest effective power chooses first, ties are broken with higher initiative value
	- Target is chosen such that the group can deal the most damage to the other group (accounting for weakness, immuniites,
	  but not accounting for enough units to recieve all the damage).
	- Ties in choosen are broken: hihger effective power first, then higher initiative
	- If a group cannot deal any damage to any group it does not choose a target
	- A defending group can only be chosen as target by one attacking group.
	  => each group has selected zero or one group to attack and is being attacked by zero or one group.

	ATTACKING Phase:
	- each group deals damage to the target it selected
	- Groups attack in decreasing order of initiative, regardless of whether they are part of the infection or the immune system
	- Groups with 0 units cannot deal any damage
	- Damage dealt = effP
		- if group is immune to attacker atack type -> no damage
		- if group is weak to attacker attack type -> double damage
	- Only whole units are killed, remaining damage is not considered


*/

/* Weaknesses and immunities
	IMMUNESYSTEM GROUPS:
	(immune to slashing)
	(weak to slashing)
	(weak to bludgeoning)
	(immune to bludgeoning)
	(weak to slashing)
	(weak to slashing)
	(weak to cold)
	(immune to bludgeoning)
	(weak to bludgeoning, slashing)

	INFECTION GROUPS (changed one line in input s.t. always first come immunities):
	(weak to radiation, bludgeoning)
	(immune to fire)
	(immune to bludgeoning; weak to radiation)
	(immune to radiation, bludgeoning; weak to slashing)
	(immune to radiation; weak to bludgeoning)
	(immune to fire, bludgeoning, cold)
	(immune to bludgeoning, cold; weak to slashing, fire)
	(weak to radiation)
*/

std::vector<Squad*> parseSquadsString(std::list<std::string>&, std::list<std::string>&);
AttackType getAttackType(std::string);
std::list<std::string> getInputPerLines(std::string);
void printDamageDealing(Squad* a, Squad* d);

int main() {
	std::vector<Squad*> squads;
	
	std::list<std::string> immuneStrings = getInputPerLines("immuneSystem_Day24.txt");
	std::list<std::string> infectionStrings = getInputPerLines("infection_Day24.txt");
	
	int immunUnits = 0, infectUnits = 0;
	int oldImmunUnits = 0, oldInfectUnits = 0;

	// Parse immune system Strings into squads
	squads = parseSquadsString(immuneStrings, infectionStrings);

	for (int i = 0; i < squads.size(); ++i) {
		if (squads[i]->getSquadType() == immuneSystem) immunUnits += squads[i]->getNUnits();
		if (squads[i]->getSquadType() == infection) infectUnits += squads[i]->getNUnits();
	}
	std::cout << "Num ImmuneUnits: " << immunUnits << "\tNum InfectUnits: " << infectUnits << std::endl;

	// Start Battle 9PART ONE)
	while (immunUnits > 0 && infectUnits > 0) {
		//////////////////////
		// Target Selection //
		//////////////////////
		std::sort(squads.begin(), squads.end(), isStronger);
		
		// Go over all squads, and assign each a target
		for (int i = 0; i < squads.size(); ++i) {
			int maxDmg = 0, dmg;
			int targetIdx = 0;
			for (int j = 0; j < squads.size(); ++j) {
				if (i != j &&
					squads[i]->getSquadType() != squads[j]->getSquadType() &&
					!squads[j]->isDefending() &&
					squads[j]->getNUnits() > 0) {
					// Keep track of the target which squad can deal most damage
					dmg = squads[i]->getDamagePotentialOn(squads[j]);
					if (maxDmg < dmg) {
						maxDmg = dmg;
						targetIdx = j;
					}
				}
			}
			if (maxDmg > 0) {
				// Valid target selected
				squads[i]->setTarget(squads[targetIdx]);
				squads[targetIdx]->setAsTarget();
			}
		}

		//////////////////////
		//   Battle Round   //
		//////////////////////
		std::sort(squads.begin(), squads.end(), hasMoreInitiative);
		for (int i = 0; i < squads.size(); ++i) {
			if (squads[i]->getNUnits() == 0) continue;
			squads[i]->attack();
		}
		
		// Check if immune system or infection have zero units left -> END CRITERION
		immunUnits = 0;
		infectUnits = 0;
		for (int i = 0; i < squads.size(); ++i) {
			if (squads[i]->getSquadType() == immuneSystem) immunUnits += squads[i]->getNUnits();
			if (squads[i]->getSquadType() == infection) infectUnits += squads[i]->getNUnits();
		}
		// DEBUG
		if (oldImmunUnits == immunUnits && oldInfectUnits == infectUnits) {
			std::cout << "TIE!" << std::endl;
			std::sort(squads.begin(), squads.end(), isStronger);
			break;
		}
		else {
			oldImmunUnits = immunUnits;
			oldInfectUnits = infectUnits;
		}
	}

	std::cout << "Num ImmuneUnits: " << immunUnits << "\tNum InfectUnits: " << infectUnits << std::endl;

	// 19974 infection units left
	std::cout << "Solution Part One: Num Immune System Units: " << immunUnits << "; Num Infect Units: " << infectUnits << std::endl;


	// PART TWO:
	int immuneBoost = 1; // Checking by hand, number must be below 100
	
	while (true) {
		///////////////////////////////////////////////
		//////////// BATTLE ITERATIONS ////////////////
		///////////////////////////////////////////////
		// Parse immune system Strings into squads
		squads.clear();
		squads = parseSquadsString(immuneStrings, infectionStrings);

		// Part Two: Boosting Immune System
		for (auto w : squads) {
			if (w->getSquadType() == immuneSystem) {
				//std::cout << "Dmg(" << immuneBoost << "): " << w->getDmg() << " -> ";
				w->addBoost(immuneBoost);
				w->updateEffPower();
				//std::cout << w->getDmg() << std::endl;
			}
		}

		immunUnits = 0;
		infectUnits = 0;
		oldImmunUnits = 0;
		oldInfectUnits = 0;
		for (int i = 0; i < squads.size(); ++i) {
			if (squads[i]->getSquadType() == immuneSystem) immunUnits += squads[i]->getNUnits();
			if (squads[i]->getSquadType() == infection) infectUnits += squads[i]->getNUnits();
		}
		//std::cout << "Boost[" << immuneBoost << "]: Num ImmuneUnits: " << immunUnits << "\tNum InfectUnits: " << infectUnits << std::endl;

		// Start Battle
		while (immunUnits > 0 && infectUnits > 0) {
			//////////////////////
			// Target Selection //
			//////////////////////
			std::sort(squads.begin(), squads.end(), isStronger);

			// Go over all squads, and assign each a target
			//std::cout << "======================================" << std::endl;
			for (int i = 0; i < squads.size(); ++i) {
				int maxDmg = 0, dmg;
				int targetIdx = 0;
				for (int j = 0; j < squads.size(); ++j) {
					if (i != j &&
						squads[i]->getSquadType() != squads[j]->getSquadType() &&
						!squads[j]->isDefending() &&
						squads[j]->getNUnits() > 0) {
						// Keep track of the target which squad can deal most damage
						dmg = squads[i]->getDamagePotentialOn(squads[j]);
						if (maxDmg < dmg) {
							maxDmg = dmg;
							targetIdx = j;
						}
						//printDamageDealing(squads[i], squads[j]);
					}
				}
				if (maxDmg > 0) {
					// Valid target selected
					squads[i]->setTarget(squads[targetIdx]);
					squads[targetIdx]->setAsTarget();
				}
			}

			//for (auto s : squads) {
			//	s->display();
			//}

			//////////////////////
			//   Battle Round   //
			//////////////////////
			std::sort(squads.begin(), squads.end(), hasMoreInitiative);
			//std::cout << "-----------------------------------" << std::endl;
			for (int i = 0; i < squads.size(); ++i) {
				if (squads[i]->getNUnits() == 0) continue;
				squads[i]->attack();
			}
			//std::cout << "-----------------------------------" << std::endl;
			// Cleanup Battle round
			for (auto s:squads) { // this seems needed, not sure why ;)
				s->attackFinished();
				s->defendingFinished();
				s->updateEffPower();
			}

			///////////////////////
			// Battle Evaluation //
			///////////////////////
			// Check if immune system or infection have zero units left -> END CRITERION
			immunUnits = 0;
			infectUnits = 0;
			for (int i = 0; i < squads.size(); ++i) {
				if (squads[i]->getSquadType() == immuneSystem) immunUnits += squads[i]->getNUnits();
				if (squads[i]->getSquadType() == infection) infectUnits += squads[i]->getNUnits();
			}
			// DEBUG
			if (oldImmunUnits == immunUnits && oldInfectUnits == infectUnits) {
				std::cout << "TIE!" << std::endl;
				std::sort(squads.begin(), squads.end(), isStronger);
				break;
			}
			else {
				oldImmunUnits = immunUnits;
				oldInfectUnits = infectUnits;
			}
		}
		std::cout << "Boost[" << immuneBoost << "]: Num ImmuneUnits: " << immunUnits << "\tNum InfectUnits: " << infectUnits << std::endl;
		///////////////////////////////////////////////
		//////////// BATTLE ITERATIONS ////////////////
		///////////////////////////////////////////////
		
		if (immunUnits > 0 && infectUnits == 0) break;
		++immuneBoost;
	}

	// With boost 43, there are 4606 immune system units left
	std::cout << "Minimum Immune Boost to cure: " << immuneBoost << std::endl;
	std::cout << "Solution Part Two: Num Immune System Units: " << immunUnits << "; Num Infect Units: " << infectUnits << std::endl;

	return 0;
}


/***************************************
*   PUZZLE SPECIFIC HELPER FUNCTIONS   *
****************************************/
std::vector<Squad*> parseSquadsString(std::list<std::string>& immuneStr, std::list<std::string>& infectStr) {
	std::regex regexGroup("([0-9]+)\\s*units\\s*each\\s*with\\s*([0-9]+)\\s*hit\\s*points\\s*(\\(.*\\))?\\s*with\\s*an\\s*attack\\s*that\\s*does\\s*([0-9]+)\\s*([a-z]+)\\s*damage\\s*at\\s*initiative\\s*([0-9]+)");
	std::regex regexWeakImmune("\\((?:immune to ([a-z]*)(?:, ([a-z]*)(?:, ([a-z]*))?)?)?(?:; )?(?:weak to ([a-z]*)(?:, ([a-z]*))?)?\\)");
	std::smatch m;
	std::smatch g;

	int numUnits, hp, damage, initiative;
	AttackType attackType, immunity, weakness;
	std::list<AttackType> weaknesses, immunities;

	int immunID = 0;
	int infectID = 0;

	Squad* newSquadPtr;
	std::vector<Squad*> squads;


	for (std::list<std::string>::iterator it = immuneStr.begin(); it != immuneStr.end(); ++it) {
		if (std::regex_search(*it, m, regexGroup)) {
			numUnits = std::stoi(m[1], nullptr, 0);
			hp = std::stoi(m[2], nullptr, 0);
			damage = std::stoi(m[4], nullptr, 0);
			initiative = std::stoi(m[6], nullptr, 0);
			attackType = getAttackType(m[5]);

			// extract weakness and immunities
			if (std::regex_search(*it, g, regexWeakImmune)) {
				// Check for first immunity
				if (g[1] != std::string().empty()) {
					immunity = getAttackType(g[1]);
					if (immunity != none) {
						immunities.push_back(immunity);
					}
				}
				// Check for second immunity
				if (g[2] != std::string().empty()) {
					immunity = getAttackType(g[2]);
					if (immunity != none) {
						immunities.push_back(immunity);
					}
				}
				// Check for third immunity
				if (g[3] != std::string().empty()) {
					immunity = getAttackType(g[3]);
					if (immunity != none) {
						immunities.push_back(immunity);
					}
				}
				// Check for first weakness
				if (g[4] != std::string().empty()) {
					weakness = getAttackType(g[4]);
					if (weakness != none) {
						weaknesses.push_back(weakness);
					}
				}
				// Check for second weakness
				if (g[5] != std::string().empty()) {
					weakness = getAttackType(g[5]);
					if (weakness != none) {
						weaknesses.push_back(weakness);
					}
				}
			}

			// Create Squad of Immune System and add it to list
			newSquadPtr = new Squad(numUnits, hp, damage, attackType, initiative, immunities, weaknesses, immuneSystem);
			newSquadPtr->groupID = immunID; ++immunID;
			squads.push_back(newSquadPtr);
			immunities.clear();
			weaknesses.clear();

		}
	}

	// Parse infection Strings into squads
	for (std::list<std::string>::iterator it = infectStr.begin(); it != infectStr.end(); ++it) {
		if (std::regex_search(*it, m, regexGroup)) {
			numUnits = std::stoi(m[1], nullptr, 0);
			hp = std::stoi(m[2], nullptr, 0);
			damage = std::stoi(m[4], nullptr, 0);
			initiative = std::stoi(m[6], nullptr, 0);
			attackType = getAttackType(m[5]);

			// extract weakness and immunities
			if (std::regex_search(*it, g, regexWeakImmune)) {
				// Check for first immunity
				if (g[1] != std::string().empty()) {
					immunity = getAttackType(g[1]);
					if (immunity != none) {
						immunities.push_back(immunity);
					}
				}
				// Check for second immunity
				if (g[2] != std::string().empty()) {
					immunity = getAttackType(g[2]);
					if (immunity != none) {
						immunities.push_back(immunity);
					}
				}
				// Check for third immunity
				if (g[3] != std::string().empty()) {
					immunity = getAttackType(g[3]);
					if (immunity != none) {
						immunities.push_back(immunity);
					}
				}
				// Check for first weakness
				if (g[4] != std::string().empty()) {
					weakness = getAttackType(g[4]);
					if (weakness != none) {
						weaknesses.push_back(weakness);
					}
				}
				// Check for second weakness
				if (g[5] != std::string().empty()) {
					weakness = getAttackType(g[5]);
					if (weakness != none) {
						weaknesses.push_back(weakness);
					}
				}
			}

			// Create Squad of Immune System and add it to list
			newSquadPtr = new Squad(numUnits, hp, damage, attackType, initiative, immunities, weaknesses, infection);
			newSquadPtr->groupID = infectID; ++infectID;
			squads.push_back(newSquadPtr);
			immunities.clear();
			weaknesses.clear();
		}
	}

	return squads;
}


AttackType getAttackType(std::string s) {
	AttackType attackType = none;
	if (s == "bludgeoning") attackType = bludgeoning;
	else if (s == "cold") attackType = cold;
	else if (s == "fire") attackType = fire;
	else if (s == "radiation") attackType = radiation;
	else if (s == "slashing") attackType = slashing;

	return attackType;
}





/******************************
*   INPUT HELPER FUNCTIONS   *
******************************/


std::list<std::string> getInputPerLines(std::string fileName) {
	std::list<std::string> lines;
	std::string line;

	// Open File
	std::ifstream in(fileName);
	if (!in.is_open() || !in.good()) {
		std::cout << "Failed to open input" << std::endl;
		lines.push_back(std::string()); // empty string
		return lines;
	}

	// Create Vector of lines
	while (getline(in, line)) {
		lines.push_back(line);
	}
	in.close();
	return lines;
}

void printDamageDealing(Squad * a, Squad * d)
{
	int dmg = a->getDamagePotentialOn(d);
	if (a->getSquadType() == immuneSystem) {
		std::cout << "Immune System group ";
	}
	else {
		std::cout << "Infection group ";
	}
	std::cout << a->groupID + 1 << " [" << a->getNUnits() << "u] would deal defending group ";
	std::cout << d->groupID + 1 << " [" << d->getNUnits() << "u] " << dmg << " damage" << std::endl;
}
