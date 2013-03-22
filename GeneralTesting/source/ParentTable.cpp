#ifndef PARENT_HEAD
#define PARENT_HEAD
#include "ParentTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

ParentTable::ParentTable(){ 
	size = 0;
}

bool ParentTable::insertParent(int stmt1, int stmt2){	

	bool notInVector = false;

	// expand size if index > size of vector
	if (stmt2> int(parentTable.size()-1)) {
		parentTable.resize(stmt2*2+1);
		notInVector = true;
	}
	if (stmt1> int(childTable.size()-1)) {
		childTable.resize(stmt1*2+1);
		notInVector = true;
	}

	// notInVector = false yet found in vector
	if (!notInVector && isParent(stmt1, stmt2)) {
		return false;
	}

	parentTable[stmt2].push_back(stmt1);
	childTable[stmt1].push_back(stmt2);

	size++;
	parentCount = std::max(stmt1 + 1, parentCount);
	childCount = std::max(stmt2 + 1, childCount);

	return true;
}


std::vector<int> ParentTable::getParent(int stmt2){
	
	std::vector<int> results ;
	if (stmt2> int(parentTable.size()-1))
		return results;

	return parentTable.at(stmt2);
}

std::vector<int> ParentTable::getParent(){
	
	std::vector<int> results ;
	for (int i = 0; i < (int)parentTable.size() ; i++) {
		if (!parentTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}



std::vector<int> ParentTable::getChild(int stmt1){

	std::vector<int> results ;
	if (stmt1> int(childTable.size()-1))
		return results;

	return childTable.at(stmt1);
}


std::vector<int> ParentTable::getChild(){

	std::vector<int> results ;
	for (int i = 0; i < (int)childTable.size() ; i++) {
		if (!childTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}

bool ParentTable:: isParent(int stmt1, int stmt2){

	std::vector<int>::iterator it;

	// by right should work
	it = std::find(childTable.at(stmt1).begin(), childTable.at(stmt1).end(), stmt2);

	if (it != childTable.at(stmt1).end())	// found
		return true ;

	return false;
}

int ParentTable::getSize(){
	return size;
}

void ParentTable::compressTables(){
	parentTable.resize(childCount);
	childTable.resize(parentCount);
}

/*
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
*/