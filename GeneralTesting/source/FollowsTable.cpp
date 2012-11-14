#ifndef FOLLOWS_HEAD
#define FOLLOWS_HEAD
#include "FollowsTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

FollowsTable::FollowsTable(){}

int FollowsTable::insertFollows(int stmt1, int stmt2){	

	int index = getFollowsIndex(stmt1, stmt2);
	if (index==-1) {
		followsTable.push_back(std::pair<int ,int>(stmt1, stmt2)) ;
		index = followsTable.size() - 1;
	}

	return index;
}


std::vector<int> FollowsTable::getFollows(int stmt){
	std::vector<int> stmtList;
	for (int i=0; i < (int)followsTable.size(); i++){
		if (followsTable[i].second == stmt)
			stmtList.push_back(followsTable[i].first) ;
	}

	return stmtList;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> FollowsTable::getFollowedBy(int stmt){

	std::vector<int> varList;
	for (int i=0; i < (int)followsTable.size(); i++){
		if (followsTable[i].first == stmt)
			varList.push_back(followsTable[i].second) ;
	}

	return varList;	//not sure how to return null. I guess its good enuough if the list is empty
}

bool FollowsTable::isFollows(int stmt1, int stmt2){

	if (FollowsTable::getFollowsIndex(stmt1, stmt2) == -1)
		return false;

	return true;

}
int FollowsTable::getFollowsIndex(int stmt, int stmt2){

	std::pair<int ,int> search = std::pair<int, int>(stmt,stmt2) ;
	std::vector<std::pair<int ,int>>::iterator it;

	it = std::find(followsTable.begin(), followsTable.end(), search);

	if (it != followsTable.end())	// found
		return (int) (it - followsTable.begin()) ;

	return -1;

}
std::pair <int ,int> FollowsTable::extractFollows (int ind){
	return followsTable.at(ind) ;
}

int FollowsTable::getSize(){
	return followsTable.size();
}