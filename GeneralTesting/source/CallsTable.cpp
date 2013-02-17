#ifndef CALLS_HEAD
#define CALLS_HEAD
#include "CallsTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif

#include <set>



CallsTable::CallsTable(){ 
	size = 0;
}

bool CallsTable::insertCalls(int proc1, int proc2){	

	bool notInVector = false;

	// expand size if index > size of vector
	if (proc2> int(callsTable.size()-1)) {
		callsTable.resize(proc2*2);
		notInVector = true;
	}
	if (proc1> int(calledByTable.size()-1)) {
		calledByTable.resize(proc1*2);
		notInVector = true;
	}

	// notInVector = false yet found in vector
	if (!notInVector && isCalls(proc1, proc2)) {
		return false;
	}

	callsTable[proc2].push_back(proc1);
	calledByTable[proc1].push_back(proc2);

	size++;
	return true;
}


std::vector<int> CallsTable::getCalls(int proc2){
	return callsTable.at(proc2);
}


std::vector<int> CallsTable::getCalledBy(int proc1){
	return calledByTable.at(proc1);
}

bool CallsTable:: isCalls(int proc1, int proc2){

	std::vector<int>::iterator it;

	// by right should work
	it = std::find(calledByTable.at(proc1).begin(), calledByTable.at(proc1).end(), proc2);

	if (it != calledByTable.at(proc1).end())	// found
		return true ;

	return false;
}

int CallsTable::getSize(){
	return size;
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