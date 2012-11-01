#ifndef PROCTABLE_HEAD
#define PROCTABLE_HEAD
#include "ProcTable.h"
#endif


ProcTable::ProcTable() {
}

int ProcTable::insertProc(std::string procName, int start, int end) {

	int index = getProcIndex(procName);

	if (index==-1) {
		procTable.push_back(Procedure(procName, start, end)) ;
		index = procTable.size() - 1;
	}

	return index;
}

int ProcTable::getSize() {
	return (int) procTable.size();
}

//throws exception if out of bounds
std::string ProcTable::getProcName (int ind){
	return procTable.at(ind).getProcName() ;
}

int ProcTable::getProcIndex (std::string procName)  {

	for (int i=0; i < (int)procTable.size(); i++){
		if (procTable[i].getProcName() == procName)
			return i;
	}

	return -1 ;
}

int ProcTable::getProcFirstln(int ind){
	return procTable.at(ind).getFirstLine() ;
}
int ProcTable::getProcLastln(int ind){
	return procTable.at(ind).getLastLine() ;
}