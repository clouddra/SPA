#ifndef PROCEDURE
#define PROCEDURE
#include "Procedure.h"
#endif

Procedure::Procedure(std::string pName, int first, int last)
{
	procName = pName;
	firstLine = first;
	lastLine =  last;
	cfgEnd = -1;
	cfgStart = -1;
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

int Procedure::getCFGStart()
{
	return cfgStart;
}

int Procedure::getCFGEnd()
{
	return cfgEnd;
}

void Procedure::setFirstAndLast(int first, int last) {
    if (firstLine != -1 || lastLine != -1)
        return;

    firstLine = first;
    lastLine = last;
}

void Procedure::setCFGEnd(int end)
{
	cfgEnd = end;
}

void Procedure::setCFGStart(int start)
{
	cfgStart = start;
}