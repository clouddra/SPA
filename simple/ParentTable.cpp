#ifndef PARENT_HEAD
#define PARENT_HEAD
#include "ParentTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

ParentTable::ParentTable(){}

int ParentTable::insertParent(int stmt1, int stmt2){	

	int index = getParentIndex(stmt1, stmt2);
	if (index==-1) {
		parentTable.push_back(std::pair<int ,int>(stmt1, stmt2)) ;
		index = parentTable.size() - 1;
	}

	return index;
}


std::vector<int> ParentTable::getParent(int stmt){
	std::vector<int> stmtList;
	for (int i=0; i < (int)parentTable.size(); i++){
		if (parentTable[i].second == stmt)
			stmtList.push_back(parentTable[i].first) ;
	}

	return stmtList;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> ParentTable::getParentOf(int stmt){

	std::vector<int> varList;
	for (int i=0; i < (int)parentTable.size(); i++){
		if (parentTable[i].first == stmt)
			varList.push_back(parentTable[i].second) ;
	}

	return varList;	//not sure how to return null. I guess its good enuough if the list is empty
}

bool ParentTable::isParent(int stmt1, int stmt2){

	if (ParentTable::getParentIndex(stmt1, stmt2) == -1)
		return false;

	return true;

}
int ParentTable::getParentIndex(int stmt, int stmt2){

	std::pair<int ,int> search = std::pair<int, int>(stmt,stmt2) ;
	std::vector<std::pair<int ,int>>::iterator it;

	// by right should work
	it = std::find(parentTable.begin(), parentTable.end(), search);

	if (it != parentTable.end())	// found
		return (int) (it - parentTable.begin()) ;

	return -1;

}
std::pair <int ,int> ParentTable::extractParent (int ind){
	return parentTable.at(ind) ;
}

int ParentTable::getSize(){
	return parentTable.size();
}