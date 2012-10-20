#ifndef MODIFIES_HEAD
#define MODIFIES_HEAD
#include "ModifiesTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

ModifiesTable::ModifiesTable(){}

int ModifiesTable::insertModifies(int stmt, int var){	

	int index = getModifiesIndex(stmt, var);
	if (index==-1) {
		modifiesTable.push_back(std::pair<int ,int>(stmt, var)) ;
		index = modifiesTable.size() - 1;
	}

	return index;
}


std::vector<int> ModifiesTable::getModifiesVar(int var){
	std::vector<int> stmtList;
	for (int i=0; i < (int)modifiesTable.size(); i++){
		if (modifiesTable[i].second == var)
			stmtList.push_back(modifiesTable[i].first) ;
	}

	return stmtList;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> ModifiesTable::getModifiedBy(int stmt){

	std::vector<int> varList;
	for (int i=0; i < (int)modifiesTable.size(); i++){
		if (modifiesTable[i].first == stmt)
			varList.push_back(modifiesTable[i].second) ;
	}

	return varList;	//not sure how to return null. I guess its good enuough if the list is empty
}

bool ModifiesTable::isModifies(int stmt, int var){

	if (ModifiesTable::getModifiesIndex(stmt, var) ==-1)
		return false;

	return true;

}
int ModifiesTable::getModifiesIndex(int stmt, int var){

	std::pair<int ,int> search = std::pair<int, int>(stmt,var) ;
	std::vector<std::pair<int ,int>>::iterator it;

	// by right should work
	it = std::find(modifiesTable.begin(), modifiesTable.end(), search);

	if (it != modifiesTable.end())	// found
		return (int) (it - modifiesTable.begin()) ;

	return -1;

}
std::pair <int ,int> ModifiesTable::extractModifies (int ind){
	return modifiesTable.at(ind) ;
}

int ModifiesTable::getSize(){
	return modifiesTable.size();
}