#ifndef PATTERNROW_HEAD
#define PATTERNROW_HEAD
#include "PatternRow.h"
#endif

PatternRow::PatternRow(std::string _pattern, std::string _declaredVar, std::string _varName, std::vector<int> _stmtNumList)
{
	pattern = _pattern;
	declaredVar = _declaredVar;
	varName = _varName;
	stmtNumList = _stmtNumList;
}

std::string PatternRow::getPattern()
{
	return pattern;
}

std::string PatternRow::getDeclaredVar()
{
	return declaredVar;
}

std::string PatternRow::getVarName()
{
	return varName;
}

std::vector<int> PatternRow::getStmtNumList()
{
	return stmtNumList;
}