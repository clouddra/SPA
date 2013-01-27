#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

class PQLParser {
	
private:

public:
    int parseQuery(std::string storage, QueryProcessor* queryProcessor);
    std::vector<std::string> splitQuery(std::string input);
};