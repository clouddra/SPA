#ifndef PATTERNROW_HEAD
#define PATTERNROW_HEAD
#include "PatternRow.h"
#endif

class PatternTable {

private:
	std::vector<PatternRow> patternTable;

public:
	PatternTable();
	int insertPatternRow(PatternRow);
	std::vector<PatternRow> getPatternTable();
	int getSize();
};