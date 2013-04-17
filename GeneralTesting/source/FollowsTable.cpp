#ifndef FOLLOWS_HEAD
#define FOLLOWS_HEAD
#include "FollowsTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif


FollowsTable::FollowsTable(){ 
	size = 0;
}

bool FollowsTable::insertFollows(int stmt1, int stmt2){	

	bool notInVector = false;

	// expand size if index > size of vector
	if (stmt2> int(followsTable.size()-1)) {
		followsTable.resize(stmt2*2+1);
		notInVector = true;
	}
	if (stmt1> int(followedByTable.size()-1)) {
		followedByTable.resize(stmt1*2+1);
		notInVector = true;
	}

	// notInVector = false yet found in vector
	if (!notInVector && isFollows(stmt1, stmt2)) {
		return false;
	}

	followsTable[stmt2].push_back(stmt1);
	followedByTable[stmt1].push_back(stmt2);

	size++;
	leaderCount = std::max(stmt1 + 1, leaderCount);
	followerCount = std::max(stmt2 + 1, followerCount);

	return true;
}


std::vector<int> FollowsTable::getFollows(int stmt2){
	
	std::vector<int> results ;
	if (stmt2> int(followsTable.size()-1))
		return results;

	return followsTable.at(stmt2);
}

std::vector<int> FollowsTable::getFollows(){
	
	std::vector<int> results ;
	for (int i = 0; i < (int)followedByTable.size() ; i++) {
		if (!followedByTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}



std::vector<int> FollowsTable::getFollowedBy(int stmt1){

	std::vector<int> results ;
	if (stmt1> int(followedByTable.size()-1))
		return results;

	return followedByTable.at(stmt1);
}


std::vector<int> FollowsTable::getFollowedBy(){

	std::vector<int> results ;
	for (int i = 0; i < (int)followsTable.size() ; i++) {
		if (!followsTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}

bool FollowsTable:: isFollows(int stmt1, int stmt2){

	std::vector<int>::iterator it;

	// by right should work
	it = std::find(followedByTable.at(stmt1).begin(), followedByTable.at(stmt1).end(), stmt2);

	if (it != followedByTable.at(stmt1).end())	// found
		return true ;

	return false;
}

int FollowsTable::getSize(){
	return size;
}

void FollowsTable::compressTables(){
    if (followerCount > 0)
	    followsTable.resize(followerCount);

    if (leaderCount > 0)
	    followedByTable.resize(leaderCount);
}

/*
#ifndef FOLLOWS_HEAD
#define FOLLOWS_HEAD
#include "FollowsTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

#include <set>

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

std::vector<int> FollowsTable::getFollows(){
	std::set<int> stmtList;
	for (int i=0; i < (int)followsTable.size(); i++){
        stmtList.insert(followsTable[i].first);
	}
    std::vector<int> ret(stmtList.begin(), stmtList.end());
	return ret;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> FollowsTable::getFollowedBy(int stmt){

	std::vector<int> varList;
	for (int i=0; i < (int)followsTable.size(); i++){
		if (followsTable[i].first == stmt)
			varList.push_back(followsTable[i].second) ;
	}

	return varList;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> FollowsTable::getFollowedBy(){
	std::set<int> stmtList;
	for (int i=0; i < (int)followsTable.size(); i++){
        stmtList.insert(followsTable[i].second);
	}
    std::vector<int> ret(stmtList.begin(), stmtList.end());
	return ret;	//not sure how to return null. I guess its good enuough if the list is empty
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
*/