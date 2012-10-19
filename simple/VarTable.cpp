#ifndef VARTABLE_HEAD
#define VARTABLE_HEAD
#include "VarTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

VarTable::VarTable() {
}

std::string VarTable::insertVar(std::string varName) {
	varTable.push_back(varName);
	return varName;
}

int VarTable::getSize() {
	return (int) varTable.size();
}

std::string VarTable::getVarName (int ind) {
	return varTable.at(ind) ;
}

int VarTable::getVarIndex (std::string varName) throw(std::out_of_range) {

	std::vector<std::string>::iterator it;
	it = std::find (varTable.begin(), varTable.end(), varName);

	if (it != varTable.end())	// found
		return (int) (it - varTable.begin()) ;

	else			// not found
		return -1 ;

}