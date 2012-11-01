#ifndef STMTNODE_HEAD
#define STMTNODE_HEAD
#include "StmtNodeTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

StmtNodeTable::StmtNodeTable(){
    //Insert dummy value to make table start at index 1
    stmtNodeTable.push_back(std::pair<int, int>(-1, -1));
}

int StmtNodeTable::insertStmtNode(int stmt, int astNode, int nodeType){	
    if (stmtNodeTable.size() == stmt) {
        stmtNodeTable.push_back(std::pair<int, int>(astNode, nodeType));
    }
    else { // Should not reach here, throw exception?
        return -1;
    } 
	return stmt;
}

// Note: only works for stmt types, ie assign/while/if/call
std::vector<int> StmtNodeTable::getNodeWithType(int nodeType){
	std::vector<int> stmtList;
	for (int i=0; i < (int)stmtNodeTable.size(); i++){
		if (stmtNodeTable[i].second == nodeType)
			stmtList.push_back(stmtNodeTable[i].first) ;
	}

	return stmtList;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> StmtNodeTable::getStmtWithType(int nodeType){
	std::vector<int> stmtList;
	for (int i=0; i < (int)stmtNodeTable.size(); i++){
		if (stmtNodeTable[i].second == nodeType)
			stmtList.push_back(i) ;
	}

	return stmtList;	//not sure how to return null. I guess its good enuough if the list is empty
}

int StmtNodeTable::getNode(int stmt){
    return stmtNodeTable[stmt].first;
}

int StmtNodeTable::getType(int stmt){
    return stmtNodeTable[stmt].second;
}

// Special note: Includes dummy entry, maybe should minus 1? Can be used to find number of statements in program
int StmtNodeTable::getSize(){
	return stmtNodeTable.size();
}