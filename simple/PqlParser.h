#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

class PqlParser {
	
private:

public:
	PqlParser();
	int parseQuery(std::string, QueryProcessor* queryProcessor);
};