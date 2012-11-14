#ifndef USESTABLE_HEAD
#define USESTABLE_HEAD
#include "UsesTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

#include <set>

UsesTable::UsesTable(){}

int UsesTable::insertUses(int stmt, int var){	

	int index = getUsesIndex(stmt, var);
	if (index==-1) {
		usesTable.push_back(std::pair<int ,int>(stmt, var)) ;
		index = usesTable.size() - 1;
	}

	return index;
}


std::vector<int> UsesTable::getUsesVar(int var){
	
	std::vector<int> stmtList;
	for (int i=0; i < (int)usesTable.size(); i++){
		if (usesTable[i].second == var)
			stmtList.push_back(usesTable[i].first) ;
	}

	return stmtList;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> UsesTable::getUsesVar(){
	std::set<int> stmtList;
	for (int i=0; i < (int)usesTable.size(); i++){
        stmtList.insert(usesTable[i].first) ;
	}
    std::vector<int> ret(stmtList.begin(), stmtList.end());

	return ret;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> UsesTable::getUsedBy(int stmt){

	std::vector<int> varList;
	for (int i=0; i < (int)usesTable.size(); i++){
		if (usesTable[i].first == stmt)
			varList.push_back(usesTable[i].second) ;
	}

	return varList;	//not sure how to return null. I guess its good enough if the list is empty
}

std::vector<int> UsesTable::getUsedBy(){
	std::set<int> varList;
	for (int i=0; i < (int)usesTable.size(); i++){
        varList.insert(usesTable[i].second) ;
	}
    std::vector<int> ret(varList.begin(), varList.end());

	return ret;	//not sure how to return null. I guess its good enough if the list is empty
}

bool UsesTable::isUses(int stmt, int var){

	if (UsesTable::getUsesIndex(stmt, var) ==-1)
		return false;

	return true;

}
int UsesTable::getUsesIndex(int stmt, int var){

	std::pair<int ,int> search = std::pair<int, int>(stmt,var) ;
	std::vector<std::pair<int ,int>>::iterator it;

	// by right should work
	it = std::find(usesTable.begin(), usesTable.end(), search);

	if (it != usesTable.end())	// found
		return (int) (it - usesTable.begin()) ;

	return -1;

}
// throws out of range exception
std::pair <int ,int> UsesTable::extractUses (int ind){
	return usesTable.at(ind) ;
}

int UsesTable::getSize(){
	return usesTable.size();
}