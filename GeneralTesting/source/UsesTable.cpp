#ifndef USESTABLE_HEAD
#define USESTABLE_HEAD
#include "UsesTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

#include <set>

UsesTable::UsesTable(){
	size = 0;
	procSize = 0;
	stmtCount = 0;
	varCount = 0;
	procCount = 0;
	procVarCount = 0;
}
/*
int UsesTable::insertUses(int stmt, int var){	

	int index = getUsesIndex(stmt, var);
	if (index==-1) {
		usesTable.push_back(std::pair<int ,int>(stmt, var)) ;
		index = usesTable.size() - 1;
	}

	return index;
}
*/


bool UsesTable::insertUses(int stmt, int var){	

	bool notInVector = false;

	// expand size if index > size of vector
	if (stmt > int(stmtToVarTable.size()-1)) {
		stmtToVarTable.resize(stmt*2+1);
		notInVector = true;
	}
	if (var > int(varToStmtTable.size()-1)) {
		varToStmtTable.resize(var*2+1);
		notInVector = true;
	}

	// notInVector = false yet found in vector
	if (!notInVector && isUses(stmt, var)) {
		return false;
	}

	stmtToVarTable[stmt].push_back(var);
	varToStmtTable[var].push_back(stmt);

	size++;
	varCount = std::max(var+1, varCount);
	stmtCount = std::max(stmt+1, stmtCount) ;
	return true;
}


bool UsesTable::insertProcUses(int proc, int var){	

	bool notInVector = false;

	// expand size if index > size of vector
	if (proc > int(procToVarTable.size()-1)) {
		procToVarTable.resize(proc*2+1);
		notInVector = true;
	}
	if (var > int(varToProcTable.size()-1)) {
		varToProcTable.resize(var*2+1);
		notInVector = true;
	}

	// notInVector = false yet found in vector
	if (!notInVector && isUsesProc(proc, var)) {
		return false;
	}

	procToVarTable[proc].push_back(var);
	varToProcTable[var].push_back(proc);

	procSize++;
	procVarCount = std::max(var+1, procVarCount) + 1;
	procCount = std::max(proc+1, procCount) + 1;

	return true;
}
/*
std::vector<int> UsesTable::getUsesVar(int var){
	
	std::vector<int> stmtList;
	for (int i=0; i < (int)usesTable.size(); i++){
		if (usesTable[i].second == var)
			stmtList.push_back(usesTable[i].first) ;
	}

	return stmtList;	//not sure how to return null. I guess its good enuough if the list is empty
}
*/
std::vector<int> UsesTable::getUsesVar(int var){
	
	std::vector<int> results ;

	if (var > int(varToStmtTable.size()-1))
		return results;

	return varToStmtTable.at(var);
}

/*
std::vector<int> UsesTable::getUsesVar(){
	std::set<int> stmtList;
	for (int i=0; i < (int)usesTable.size(); i++){
        stmtList.insert(usesTable[i].first) ;
	}
    std::vector<int> ret(stmtList.begin(), stmtList.end());

	return ret;	//not sure how to return null. I guess its good enuough if the list is empty
}
*/


std::vector<int> UsesTable::getUsesVar(){

	std::vector<int> results ;
	for (int i = 0; i < (int)stmtToVarTable.size() ; i++) {
		if (!stmtToVarTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}


std::vector<int> UsesTable::getUsesVarProc(int var){
	
	std::vector<int> results ;

	if (var > int(varToProcTable.size()-1))
		return results;

	return varToProcTable.at(var);
}



/*
std::vector<int> UsesTable::getUsedBy(int stmt){

	std::vector<int> varList;
	for (int i=0; i < (int)usesTable.size(); i++){
		if (usesTable[i].first == stmt)
			varList.push_back(usesTable[i].second) ;
	}

	return varList;	//not sure how to return null. I guess its good enough if the list is empty
}
*/

std::vector<int> UsesTable::getUsedBy(int stmt){
	
	std::vector<int> results ;

	if (stmt > int(stmtToVarTable.size()-1))
		return results;

	return stmtToVarTable.at(stmt);
}

/*
std::vector<int> UsesTable::getUsedBy(){
	std::set<int> varList;
	for (int i=0; i < (int)usesTable.size(); i++){
        varList.insert(usesTable[i].second) ;
	}
    std::vector<int> ret(varList.begin(), varList.end());

	return ret;	//not sure how to return null. I guess its good enough if the list is empty
}
*/

std::vector<int> UsesTable::getUsedBy(){

	std::vector<int> results ;
	for (int i = 0; i < (int)varToStmtTable.size() ; i++) {
		if (!varToStmtTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}



std::vector<int> UsesTable::getUsedByProc(int proc){

	std::vector<int> results ;
	if (proc > int(procToVarTable.size()-1))
		return results;

	return procToVarTable.at(proc);
}



/*
bool UsesTable::isUses(int stmt, int var){

	if (UsesTable::getUsesIndex(stmt, var) ==-1)
		return false;

	return true;

}
*/

bool UsesTable::isUses(int stmt, int var){

	std::vector<int>::iterator it;

	// by right should work
	it = std::find(stmtToVarTable.at(stmt).begin(), stmtToVarTable.at(stmt).end(), var);

	if (it != stmtToVarTable.at(stmt).end())	// found
		return true ;

	return false;
}


bool UsesTable:: isUsesProc(int proc, int var){

	std::vector<int>::iterator it;

	// by right should work
	it = std::find(procToVarTable.at(proc).begin(), procToVarTable.at(proc).end(), var);

	if (it != procToVarTable.at(proc).end())	// found
		return true ;

	return false;
}

/*
int UsesTable::getUsesIndex(int stmt, int var){

	std::pair<int ,int> search = std::pair<int, int>(stmt,var) ;
	std::vector<std::pair<int ,int>>::iterator it;

	// by right should work
	it = std::find(usesTable.begin(), usesTable.end(), search);

	if (it != usesTable.end())	// found
		return (int) (it - usesTable.begin()) ;

	return -1;

}
*/

/*
// throws out of range exception
std::pair <int ,int> UsesTable::extractUses (int ind){
	return usesTable.at(ind) ;
}
*/

int UsesTable::getSize(){
	return size;
}

int UsesTable::getSizeProc(){
	return procSize;
}

void UsesTable::compressTables(){
	procToVarTable.resize(procCount);
	varToProcTable.resize(procVarCount);
	varToStmtTable.resize(varCount);
	stmtToVarTable.resize(stmtCount);
}