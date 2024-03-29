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

std::vector<int> UsesTable::getUsesVar(int var){
	
	std::vector<int> results ;

	if (var > int(varToStmtTable.size()-1))
		return results;

	return varToStmtTable.at(var);
}


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


std::vector<int> UsesTable::getUsesVarProc(){

	std::vector<int> results ;
	for (int i = 0; i < (int)procToVarTable.size() ; i++) {
		if (!procToVarTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}


std::vector<int> UsesTable::getUsedBy(int stmt){
	
	std::vector<int> results ;

	if (stmt > int(stmtToVarTable.size()-1))
		return results;

	return stmtToVarTable.at(stmt);
}

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


std::vector<int> UsesTable::getUsedByProc(){

	std::vector<int> results ;
	for (int i = 0; i < (int)varToProcTable.size() ; i++) {
		if (!varToProcTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}


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