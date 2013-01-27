#ifndef PARENT_HEAD
#define PARENT_HEAD
#include "ParentTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

#include <set>

ParentTable::ParentTable(){}

int ParentTable::insertParent(int stmt1, int stmt2) {	

	int index = getParentIndex(stmt1, stmt2);
	if (index==-1) {
		parentTable.push_back(std::pair<int ,int>(stmt1, stmt2)) ;
		index = parentTable.size() - 1;
	}

	return index;
}


int ParentTable::getParent(int stmt){
	for (int i=0; i < (int)parentTable.size(); i++) {
		if (parentTable[i].second == stmt)
			return parentTable[i].first;
	}

	return -1;
}

std::vector<int> ParentTable::getParent(){
    std::set<int> temp;
	for (int i=0; i < (int)parentTable.size(); i++) {
        temp.insert(parentTable[i].first);
	}
    std::vector<int> ret(temp.begin(), temp.end());
	return ret;
}

std::vector<int> ParentTable::getChild(int stmt) {
	std::vector<int> temp;
	for (int i=0; i < (int)parentTable.size(); i++){
		if (parentTable[i].first == stmt)
			temp.push_back(parentTable[i].second) ;
	}

	return temp;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> ParentTable::getChild() {
    std::set<int> temp;
	for (int i=0; i < (int)parentTable.size(); i++) {
        temp.insert(parentTable[i].second);
	}
    std::vector<int> ret(temp.begin(), temp.end());
	return ret;
}

bool ParentTable::isParent(int stmt1, int stmt2) {

	if (ParentTable::getParentIndex(stmt1, stmt2) == -1)
		return false;

	return true;

}

int ParentTable::getParentIndex(int stmt, int stmt2) {

	std::pair<int ,int> search = std::pair<int, int> (stmt,stmt2) ;
	std::vector<std::pair<int ,int>>::iterator it;

	// by right should work
	it = std::find(parentTable.begin(), parentTable.end(), search);

	if (it != parentTable.end())	// found
		return (int) (it - parentTable.begin()) ;

	return -1;

}

std::pair <int, int> ParentTable::extractParent (int ind){
	return parentTable.at(ind) ;
}

int ParentTable::getSize() {
	return parentTable.size();
}