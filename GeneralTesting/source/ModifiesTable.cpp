#ifndef MODIFIES_HEAD
#define MODIFIES_HEAD
#include "ModifiesTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

#include <set>

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
	std::set<int> stmtList;
	for (int i=0; i < (int)modifiesTable.size(); i++){
        stmtList.insert(modifiesTable[i].first) ;
	}
    std::vector<int> ret(stmtList.begin(), stmtList.end());
	return ret;	//not sure how to return null. I guess its good enuough if the list is empty
}

//throews exception if procIndex >= size
std::vector<int> ModifiesTable::getModifiesVarProc(int procIndex){
	return modifiesProcTable.at(procIndex);
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

	std::set<int> varList;
	for (int i=0; i < (int)modifiesTable.size(); i++){
        varList.insert(modifiesTable[i].second) ;
	}
    std::vector<int> ret(varList.begin(), varList.end());

	return ret;	//not sure how to return null. I guess its good enuough if the list is empty
}


std::vector<int> ModifiesTable::getModifiedByProc(int var){

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