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

int VarTable::insertVar(std::string varName) {

	std::vector<std::string>::iterator it;
	it = std::find (varTable.begin(), varTable.end(), varName);

	int index = getVarIndex(varName);
	if (index==-1) {
		varTable.push_back(varName) ;
		index = varTable.size() - 1;
	}

	return index;
}

int VarTable::getSize() {
	return (int) varTable.size();
}

std::string VarTable::getVarName (int ind){
	return varTable.at(ind) ;
}

int VarTable::getVarIndex (std::string varName)  {

	std::vector<std::string>::iterator it;
	it = std::find (varTable.begin(), varTable.end(), varName);

	if (it != varTable.end())	// found
		return (int) (it - varTable.begin()) ;

	else			// not found
		return -1 ;

}