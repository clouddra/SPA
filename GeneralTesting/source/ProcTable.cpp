#ifndef PROCTABLE_HEAD
#define PROCTABLE_HEAD
#include "ProcTable.h"
#endif


ProcTable::ProcTable() {
}

int ProcTable::insertProc(std::string procName) {
	int index = getProcIndex(procName);

	if (index==-1) {
		numToProc.push_back(Procedure(procName, -1, -1)) ;
		index = numToProc.size() - 1;
	}
	procToNum.emplace(std::make_pair(procName, index));
	return index;
}

void ProcTable::updateProc(int index, int first, int last) {
    if (index < 0 || index >= (int)numToProc.size()) {
        return;
    }
    else {
        numToProc[index].setFirstAndLast(first, last);
    }
}

int ProcTable::getSize() {
	return (int) numToProc.size();
}

//throws exception if out of bounds
std::string ProcTable::getProcName (int ind) {
	return numToProc.at(ind).getProcName() ;
}

int ProcTable::getProcIndex (std::string procName) {

	int ret;
	try
	{
		 ret = procToNum.at(procName);
	}
	catch (const std::out_of_range& oor) {
		return -1;
	}
	return ret;
}

int ProcTable::getProcFirstln(int ind) {
	return numToProc.at(ind).getFirstLine() ;
}

int ProcTable::getProcLastln(int ind) {
	return numToProc.at(ind).getLastLine() ;
}


std::vector<std::string> ProcTable::getAllProc() {
    std::vector<std::string> ret;

    for (int i = 0; i < (int)numToProc.size(); i++) {
        ret.push_back(numToProc[i].getProcName());
    }
    return ret;
}

Procedure ProcTable::getProc(int ind){
	return numToProc.at(ind);
}

void ProcTable::setCFGStart(int ind, int start){
	numToProc.at(ind).setCFGStart(start);
}

void ProcTable::setCFGEnd(int ind, int end){
	numToProc.at(ind).setCFGEnd(end);
}