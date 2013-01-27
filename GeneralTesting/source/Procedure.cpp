#ifndef PROCEDURE
#define PROCEDURE
#include "Procedure.h"
#endif

Procedure::Procedure(std::string pName, int first, int last)
{
	procName = pName;
	firstLine = first;
	lastLine =  last;
}

std::string Procedure::getProcName(){
	return procName;
}
int Procedure::getFirstLine(){
	return firstLine;
}

int Procedure::getLastLine(){
	return lastLine;
}

void Procedure::setLastLine(int last) {
    lastLine = last;
}