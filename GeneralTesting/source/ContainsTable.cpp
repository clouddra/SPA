#ifndef CONTAINS_HEAD
#define CONTAINS_HEAD
#include "ContainsTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

ContainsTable::ContainsTable(){ 
	size = 0;
}

bool ContainsTable::insertContains(int stmt1, int stmt2){	

	bool notInVector = false;

	// expand size if index > size of vector
	if (stmt2 > int(containerTable.size()-1)) {
		containerTable.resize(stmt2*2+1);
		notInVector = true;
	}
	if (stmt1 > int(containedInTable.size()-1)) {
		containedInTable.resize(stmt1*2+1);
		notInVector = true;
	}

	// notInVector = false yet found in vector
	if (!notInVector && isContains(stmt1, stmt2)) {
		return false;
	}

	containerTable[stmt2].push_back(stmt1);
	containedInTable[stmt1].push_back(stmt2);

	size++;
	containerCount = std::max(stmt1 + 1, containerCount);
	containeeCount = std::max(stmt2 + 1, containeeCount);

	return true;
}


std::vector<int> ContainsTable::getContainer(int stmt2){
	
	std::vector<int> results ;
	if (stmt2> int(containerTable.size()-1))
		return results;

	return containerTable.at(stmt2);
}

std::vector<int> ContainsTable::getContainer(){
	
	std::vector<int> results ;
	for (int i = 0; i < (int)containerTable.size() ; i++) {
		if (!containerTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}



std::vector<int> ContainsTable::getContainedIn(int stmt1){

	std::vector<int> results ;
	if (stmt1> int(containedInTable.size()-1))
		return results;

	return containedInTable.at(stmt1);
}


std::vector<int> ContainsTable::getContainedIn(){

	std::vector<int> results ;
	for (int i = 0; i < (int)containedInTable.size() ; i++) {
		if (!containedInTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}

bool ContainsTable:: isContains(int stmt1, int stmt2){

	std::vector<int>::iterator it;

	// by right should work
	it = std::find(containedInTable.at(stmt1).begin(), containedInTable.at(stmt1).end(), stmt2);

	if (it != containedInTable.at(stmt1).end())	// found
		return true ;

	return false;
}

int ContainsTable::getSize(){
	return size;
}

void ContainsTable::compressTables(){
    if (containeeCount > 0)
	    containerTable.resize(containeeCount);

    if (containerCount > 0)
	    containedInTable.resize(containerCount);
}