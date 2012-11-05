#ifndef PATTERNTABLE_HEAD
#define PATTERNTABLE_HEAD
#include "PatternTable.h"
#endif

PatternTable::PatternTable() {}

int PatternTable::insertPatternRow(PatternRow temp)
{
	patternTable.push_back(temp);
	return getSize()-1;
}

std::vector<PatternRow> PatternTable::getPatternTable()
{
	return patternTable;
}

int PatternTable::getSize()
{
	return (int)patternTable.size();
}
