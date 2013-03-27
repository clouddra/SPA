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

	int index = getVarIndex(varName);
	if (index==-1) {
		numToVar.push_back(varName) ;
		index = numToVar.size() - 1;
	}
	varToNum.emplace(std::make_pair(varName, index));
	
	return index;
}

int VarTable::getSize() {
	return (int) numToVar.size();
}

std::string VarTable::getVarName (int ind){

	return numToVar.at(ind) ;
}

int VarTable::getVarIndex (std::string varName)  {
	int ret;
	try
	{
		 ret = varToNum.at(varName);
	}
	catch (const std::out_of_range& oor) {
		return -1;
	}
	return ret;
}

std::vector<std::string> VarTable::getTable() {
	return numToVar;
}