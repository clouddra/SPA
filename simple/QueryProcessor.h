#ifndef DECLARATION_HEAD
#define DECLARATION_HEAD
#include "DeclarationTable.h"
#endif

#ifndef QUERYTREE_HEAD
#define QUERYTREE_HEAD
#include "QueryTree.h"
#endif

class QueryProcessor {

private:
	QueryTree queryTree;
	DeclarationTable declarationTable;
	std::vector<std::string> result;

public:
	QueryProcessor();
	int insertNode(std::string, std::string, int parent);
	void processQuery();
	void printResult();
};
