#include "Squad.h"



// Operator definition for Class Type Squad
inline bool operator==(const Squad& lhs, const Squad& rhs) {
	// return true if same X and Y coordinates
	if (lhs.getEffPower() == rhs.getEffPower() && lhs.getInitiative() == rhs.getInitiative()) return true;
	return false;
}
inline bool operator!=(const Squad& lhs, const Squad& rhs) { return !(lhs == rhs); }
inline bool operator< (const Squad& lhs, const Squad& rhs) {
	// returns true if this Coordinates gets picke first in a 'reading sense' 
	if (lhs.getEffPower() < rhs.getEffPower()) return true;
	else if (lhs.getEffPower() == rhs.getEffPower() && lhs.getInitiative() < rhs.getInitiative()) return true;
	else return false;
}
inline bool operator> (const Squad& lhs, const Squad& rhs) { return rhs < lhs; }
inline bool operator<=(const Squad& lhs, const Squad& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Squad& lhs, const Squad& rhs) { return !(lhs < rhs); }



// Checks wether a Squad has the weakness w
bool Squad::hasWeakness(AttackType w)
{
	for (auto weakness : this->weaknesses) {
		if (weakness == w) return true;
	}
	return false;
}

// Checks wether a Squad has the immunity w
bool Squad::hasImmunity(AttackType w)
{
	for (auto immunity : this->immunities) {
		if (immunity == w) return true;
	}
	return false;
}

// Calculates the damage Potential of a possible target
int Squad::getDamagePotentialOn(Squad * target)
{
	// Sanity Check
	if (this->getSquadType() == target->getSquadType()) return 0;

	int totalDmg = this->effP;
	if (target->hasImmunity(this->aType)) return 0;
	if (target->hasWeakness(this->aType)) totalDmg *= 2;

	return totalDmg;
}

// Executes the attack on the stored Squad
void Squad::attack() {
	if (!this->isAttacking() || this->getTarget() == nullptr) return;

	//DEBUG
	this->updateEffPower();

	int dmg = this->getDamagePotentialOn(this->getTarget());
	int killed = this->getTarget()->getsAttacked(dmg, this->getAType());
	//printAttackReport(this,killed);
	

	this->getTarget()->defendingFinished();
	this->getTarget()->updateEffPower();
	this->attackFinished();
	
}

// Returns the number of killed units after the attack
// It also stores the updated number of units in the Squad
int Squad::getsAttacked(int effP, AttackType aType)
{
	int totalDmg = effP;
	int killedUnits, numU;
	
	killedUnits = totalDmg / this->hp;
	numU = this->numUnits;
	this->numUnits -= killedUnits;
	if (this->numUnits < 0) {
		this->numUnits = 0;
		killedUnits = numU;
	}
	return killedUnits;
}

// Just print Squad info on command line
void Squad::display()
{
	std::string attackTypeStr = returnAttackTypeStr(this->aType);
	std::cout << "============================================" << std::endl;
	std::cout << "Squad Typ: ";
	if (this->squadType == immuneSystem) std::cout << "Immune System" << std::endl;
	else std::cout << "Infection" << std::endl;
	std::cout << "Num Units: " << this->numUnits << std::endl;
	std::cout << "Eff Power: " << this->effP << std::endl;
	std::cout << "Initiativ: " << this->initiative << std::endl;
	std::cout << "Hit Point: " << this->hp << std::endl;
	std::cout << "Damage   : " << this->damage << std::endl;
	std::cout << "AttackTyp: " << this->aType << " (" << attackTypeStr << ")" << std::endl;
	std::cout << "Immune to: ";
	for (auto i : this->immunities) {
		std::cout << returnAttackTypeStr(i) << " ";
	}
	std::cout << std::endl;
	std::cout << "Weak to:   ";
	for (auto i : this->weaknesses) {
		std::cout << returnAttackTypeStr(i) << " ";
	}
	std::cout << std::endl;
	std::cout << "--------------------------------------------" << std::endl << std::endl;
	std::cout << "Battle State(" << this << "):" << std::endl;
	if (this->attacking) {
		std::cout << "Attacking " << this->getTarget()->groupID << " (" << this->getTarget() << ")" << std::endl;
	}
	if (this->defending) {
		std::cout << "Is Defending " << std::endl;
	}
	std::cout << std::endl;
	std::cout << "--------------------------------------------" << std::endl << std::endl;

}

// Non-Member Function
std::string returnAttackTypeStr(AttackType aT) {
	std::string aTStr = "none";
	if (aT == bludgeoning) aTStr = "bludgeoning";
	else if (aT == cold) aTStr = "cold";
	else if (aT == fire) aTStr = "fire";
	else if (aT == radiation) aTStr = "radiation";
	else if (aT == slashing) aTStr = "slashing";

	return aTStr;
}

bool isStronger(const Squad* lhs, const Squad* rhs) {
	// returns true if lhs Squad can choose first
	if (lhs->getEffPower() > rhs->getEffPower()) return true;
	else if (lhs->getEffPower() == rhs->getEffPower() && lhs->getInitiative() > rhs->getInitiative()) return true;
	else return false;
}

bool hasMoreInitiative(const Squad* lhs, const Squad* rhs) {
	// returns true if lhs Squad can attack first
	if (lhs->getInitiative() > rhs->getInitiative()) return true;
	else return false;
}

void printAttackReport(Squad* a, int killed) {
	if (a->getSquadType() == immuneSystem) {
		std::cout << "Immune System group ";
	}
	else {
		std::cout << "Infection group ";
	}
	std::cout << a->groupID + 1 << " attacks defending group " << a->getTarget()->groupID + 1 << ", killing " << killed << " units" << std::endl;
}