#ifndef SIBLING_HEAD
#define SIBLING_HEAD
#include "SiblingTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

SiblingTable::SiblingTable(){ 
	size = 0;
}

bool SiblingTable::insertSibling(int stmt1, int stmt2){
	
	bool notInVector = false;

	//expand table if necessary
	int currSize = siblingTable.size();
	if(stmt1>currSize)
		siblingTable.resize(stmt1+1);

	currSize = siblingTable.size();
	if(stmt2>currSize)
		siblingTable.resize(stmt2+1);

	siblingTable[stmt1].push_back(stmt2);
	siblingTable[stmt2].push_back(stmt1);
	
	return true;

}
std::vector<int> SiblingTable::getSiblings(int stmt)
{
	std::vector<int> results;
	if(stmt>int(siblingTable.size()-1))
		return results;

	return siblingTable.at(stmt);

}

bool SiblingTable::isSibling(int stmt1, int stmt2){
	std::vector<int>::iterator it;

	it = std::find(siblingTable.at(stmt1).begin(), siblingTable.at(stmt1).end(), stmt2);
	if(it != siblingTable.at(stmt1).end())
		return true;

	return false;
}
/*

int SiblingTable::insertSibling(int stmt1, int stmt2){	
	int index = getSiblingIndex(stmt1, stmt2);
	int index2 = getSiblingIndex(stmt2, stmt1);
	if (index==-1 && index2==-1) {
		siblingTable.push_back(std::pair<int ,int>(stmt1, stmt2)) ;
		index = siblingTable.size() - 1;
	}
	else if(index==-1)
		return index2;

	return index;
}

std::vector<int> SiblingTable::getSiblings(int stmt){
	std::vector<int> temp;
	for (int i=0; i < (int)siblingTable.size(); i++){
		if (siblingTable[i].first == stmt)
			temp.push_back(siblingTable[i].second);
		else if (siblingTable[i].second == stmt)
			temp.push_back(siblingTable[i].first);
	}
	return temp;
}

bool SiblingTable::isSibling(int stmt1, int stmt2) {

	if (SiblingTable::getSiblingIndex(stmt1, stmt2) == -1)
		return false;
	return true;
}

int SiblingTable::getSiblingIndex(int stmt, int stmt2) {

	std::pair<int ,int> search = std::pair<int, int> (stmt,stmt2) ;
    std::pair<int ,int> search2 = std::pair<int, int> (stmt2,stmt) ;
	std::vector<std::pair<int ,int>>::iterator it, it2;

	// by right should work
	it = std::find(siblingTable.begin(), siblingTable.end(), search);

	if (it != siblingTable.end())	// found
		return (int) (it - siblingTable.begin()) ;
    else {
        it2 = std::find(siblingTable.begin(), siblingTable.end(), search2);
        if (it2 != siblingTable.end())	// found
		    return (int) (it - siblingTable.begin()) ;
    }

	return -1;

}
*/
int SiblingTable::getSize() {
	return siblingTable.size();
}