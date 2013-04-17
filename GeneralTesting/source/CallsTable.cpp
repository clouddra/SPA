#ifndef CALLS_HEAD
#define CALLS_HEAD
#include "CallsTable.h"
#endif

#ifndef ALGORITHM_HEAD
#define ALGORITHM_HEAD
#include <algorithm>
#endif


CallsTable::CallsTable(){ 
	size = 0;
    callerCount = 0;
    calleeCount = 0;
}

bool CallsTable::insertCalls(int proc1, int proc2){	

	bool notInVector = false;

	// expand size if index > size of vector
	if (proc2> int(callsTable.size()-1)) {
		callsTable.resize(proc2*2+1);
		notInVector = true;
	}
	if (proc1> int(calledByTable.size()-1)) {
		calledByTable.resize(proc1*2+1);
		notInVector = true;
	}

	// notInVector = false yet found in vector
	if (!notInVector && isCalls(proc1, proc2)) {
		return false;
	}

	callsTable[proc2].push_back(proc1);
	calledByTable[proc1].push_back(proc2);

	size++;
	callerCount = std::max(proc1 + 1, callerCount);
	calleeCount = std::max(proc2 + 1, calleeCount);

	return true;
}


std::vector<int> CallsTable::getCalls(int proc2){
	
	std::vector<int> results ;
	if (proc2> int(callsTable.size()-1))
		return results;

	return callsTable.at(proc2);
}

std::vector<int> CallsTable::getCalls(){
	
	std::vector<int> results ;
	for (int i = 0; i < (int)calledByTable.size() ; i++) {
		if (!calledByTable.at(i).empty())
			results.push_back(i);
	}

	return results;
}



std::vector<int> CallsTable::getCalledBy(int proc1){

	std::vector<int> results ;
	if (proc1> int(calledByTable.size()-1))
		return results;

	return calledByTable.at(proc1);
}


std::vector<int> CallsTable::getCalledBy(){

	std::vector<int> results ;
	for (int i = 0; i < (int)callsTable.size() ; i++) {
		if (!callsTable.at(i).empty())
			results.push_back(i);
	}

	return results;
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

void CallsTable::compressTables(){
    if (calleeCount > 0)
	    callsTable.resize(calleeCount);

    if (callerCount > 0)
	    calledByTable.resize(callerCount);
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