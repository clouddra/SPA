#ifndef MODIFIESTABLE_HEAD
#define MODIFIESTABLE_HEAD
#include "ModifiesTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

#include <set>

ModifiesTable::ModifiesTable(){
	size = 0;
	procSize = 0;
	stmtCount = 0;
	varCount = 0;
	procCount = 0;
	procVarCount = 0;
}


bool ModifiesTable::insertModifies(int stmt, int var){	

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
	if (!notInVector && isModifies(stmt, var)) {
		return false;
	}

	stmtToVarTable[stmt].push_back(var);
	varToStmtTable[var].push_back(stmt);

	size++;
	varCount = std::max(var+1, varCount);
	stmtCount = std::max(stmt+1, stmtCount) ;
	return true;
}


bool ModifiesTable::insertProcModifies(int proc, int var){	

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
	if (!notInVector && isModifiesProc(proc, var)) {
		return false;
	}

	procToVarTable[proc].push_back(var);
	varToProcTable[var].push_back(proc);

	procSize++;
	procVarCount = std::max(var+1, procVarCount) + 1;
	procCount = std::max(proc+1, procCount) + 1;

	return true;
}

std::vector<int> ModifiesTable::getModifiesVar(int var){
	
	std::vector<int> results ;

	if (var > int(varToStmtTable.size()-1))
		return results;

	return varToStmtTable.at(var);
}



std::vector<int> ModifiesTable::getModifiesVar(){

	std::vector<int> results ;
	for (int i = 0; i < (int)stmtToVarTable.size() ; i++) {
		if (!stmtToVarTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}


std::vector<int> ModifiesTable::getModifiesVarProc(int var){
	
	std::vector<int> results ;

	if (var > int(varToProcTable.size()-1))
		return results;

	return varToProcTable.at(var);
}


std::vector<int> ModifiesTable::getModifiesVarProc(){

	std::vector<int> results ;
	for (int i = 0; i < (int)procToVarTable.size() ; i++) {
		if (!procToVarTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}


std::vector<int> ModifiesTable::getModifiedBy(int stmt){
	
	std::vector<int> results ;

	if (stmt > int(stmtToVarTable.size()-1))
		return results;

	return stmtToVarTable.at(stmt);
}



std::vector<int> ModifiesTable::getModifiedBy(){

	std::vector<int> results ;
	for (int i = 0; i < (int)varToStmtTable.size() ; i++) {
		if (!varToStmtTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}



std::vector<int> ModifiesTable::getModifiedByProc(int proc){

	std::vector<int> results ;
	if (proc > int(procToVarTable.size()-1))
		return results;

	return procToVarTable.at(proc);
}


std::vector<int> ModifiesTable::getModifiedByProc(){

	std::vector<int> results ;
	for (int i = 0; i < (int)varToProcTable.size() ; i++) {
		if (!varToProcTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}


bool ModifiesTable::isModifies(int stmt, int var){

	std::vector<int>::iterator it;

	// by right should work
	it = std::find(stmtToVarTable.at(stmt).begin(), stmtToVarTable.at(stmt).end(), var);

	if (it != stmtToVarTable.at(stmt).end())	// found
		return true ;

	return false;
}


bool ModifiesTable:: isModifiesProc(int proc, int var){

	std::vector<int>::iterator it;

	// by right should work
	it = std::find(procToVarTable.at(proc).begin(), procToVarTable.at(proc).end(), var);

	if (it != procToVarTable.at(proc).end())	// found
		return true ;

	return false;
}

int ModifiesTable::getSize(){
	return size;
}

int ModifiesTable::getSizeProc(){
	return procSize;
}

void ModifiesTable::compressTables(){
	procToVarTable.resize(procCount);
	varToProcTable.resize(procVarCount);
	varToStmtTable.resize(varCount);
	stmtToVarTable.resize(stmtCount);
}
/*
ModifiesTable::ModifiesTable(){}

int ModifiesTable::insertModifies(int stmt, int var){	

	int index = getModifiesIndex(stmt, var);
	if (index==-1) {
		modifiesTable.push_back(std::pair<int ,int>(stmt, var)) ;
		index = modifiesTable.size() - 1;
	}

	return index;
}

// return the index of the var in the procedure
int ModifiesTable::insertProcModifies(int procIndex, int var){
	
	int index = -1;

	// expand size if index > size of vector
	if (procIndex > (int)modifiesProcTable.size()-1)
		 modifiesProcTable.resize(procIndex*2+1);
	else
		index = getModifiesProcIndex(procIndex, var);

	if (index == -1) {
		modifiesProcTable[procIndex].push_back(var) ;
		index = modifiesProcTable[procIndex].size() - 1;
	}

	return index;
}

std::vector<int> ModifiesTable::getModifiesVar(int var){
	std::vector<int> stmtList;
	for (int i=0; i < (int)modifiesTable.size(); i++){
		if (modifiesTable[i].second == var)
			stmtList.push_back(modifiesTable[i].first) ;
	}

	return stmtList;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> ModifiesTable::getModifiesVar(){
	std::set<int> stmtSet;
	for (int i=0; i < (int)modifiesTable.size(); i++){
        stmtSet.insert(modifiesTable[i].first) ;
	}
    std::vector<int> ret(stmtSet.begin(), stmtSet.end());
	return ret;	//not sure how to return null. I guess its good enuough if the list is empty
}

//throews exception if procIndex >= size
std::vector<int> ModifiesTable::getModifiesVarProc(int var){
	std::vector<int> procList;

	// for each procedure and all variables in each procedure
	for (int i=0; i<(int)modifiesProcTable.size(); i++) {		
		for (int j=0; j<(int)modifiesProcTable[i].size(); j++) {
			if (modifiesProcTable[i][j] == var) {
				procList.push_back(i) ;
				break;
			}
		}
	}
	return procList;
}

std::vector<int> ModifiesTable::getModifiesVarProc(){
	std::vector<int> ret;
	for (int i=0; i < (int)modifiesProcTable.size(); i++){
        if (modifiesProcTable[i].size() > 0) {
            ret.push_back(i);
        }
	}  
	return ret;	
}

std::vector<int> ModifiesTable::getModifiedBy(int stmt){

	std::vector<int> varList;
	for (int i=0; i < (int)modifiesTable.size(); i++){
		if (modifiesTable[i].first == stmt)
			varList.push_back(modifiesTable[i].second) ;
	}

	return varList;	//not sure how to return null. I guess its good enuough if the list is empty
}

std::vector<int> ModifiesTable::getModifiedBy(){

	std::set<int> varSet;
	for (int i=0; i < (int)modifiesTable.size(); i++){
        varSet.insert(modifiesTable[i].second) ;
	}
    std::vector<int> ret(varSet.begin(), varSet.end());

	return ret;	//not sure how to return null. I guess its good enuough if the list is empty
}


std::vector<int> ModifiesTable::getModifiedByProc(int procIndex){
    return modifiesProcTable.at(procIndex);
}

std::vector<int> ModifiesTable::getModifiedByProc(){
    std::set<int> varSet;
    for (int i=0; i < (int)modifiesProcTable.size(); i++){
        std::vector<int> temp = modifiesProcTable[i];
        varSet.insert(temp.begin(), temp.end());
	}
    std::vector<int> ret(varSet.begin(), varSet.end());
    return ret;
}

bool ModifiesTable::isModifies(int stmt, int var){

	if (ModifiesTable::getModifiesIndex(stmt, var) == -1)
		return false;

	return true;
}


int ModifiesTable::getModifiesProcIndex(int procIndex, int var) {

	if (procIndex > (int)modifiesProcTable.size()-1)
		 return -1;

	for (int j=0; j<(int)modifiesProcTable[procIndex].size(); j++) {
			if (modifiesProcTable[procIndex][j] == var) {
				return j;
			}
		}
	return -1;
}


bool ModifiesTable::isModifiesProc(int procIndex, int var){

	if (getModifiesProcIndex(procIndex, var)==-1)
		return false;

	return true;
}

int ModifiesTable::getModifiesIndex(int stmt, int var){

	std::pair<int ,int> search = std::pair<int, int>(stmt,var) ;
	std::vector<std::pair<int ,int>>::iterator it;

	// by right should work
	it = std::find(modifiesTable.begin(), modifiesTable.end(), search);

	if (it != modifiesTable.end())	// found
		return (int) (it - modifiesTable.begin()) ;

	return -1;
}

std::pair <int ,int> ModifiesTable::extractModifies (int ind){
	return modifiesTable.at(ind) ;
}

std::vector<int> ModifiesTable::extractModifiesProc (int ind){
	return modifiesProcTable.at(ind);
}

int ModifiesTable::getModifiesTableSize(){
	return modifiesTable.size();
}
*/
// code for testing modifies procedure in case anyone needs
/*

	ModifiesTable test;
	test.insertProcModifies(1,1) ;
	test.insertProcModifies(1,2);
		test.insertProcModifies(1,2);
		test.insertProcModifies(1,3);
	test.insertProcModifies(2,1);
	test.insertProcModifies(1,1) ;
	test.insertProcModifies(3,2) ;
	test.insertProcModifies(100,1) ;
	std::vector<int> list, list2 ;

		std::cout << test.getModifiedByProc(1).size() << std::endl;

	list =	test.getModifiedByProc(2) ;
	for (int i=0; i<list.size(); i++)
		std::cout << list[i] << " " ;

	std::cout << std::endl;
	list2 = test.getModifiesVarProc(1) ;
	std::cout << "proc ind" << test.getModifiesProcIndex(1,3) << std::endl;
	for (int i=0; i<list2.size(); i++)
		std::cout << list2[i] << " " ;

    system("pause");
	return 0;

*/