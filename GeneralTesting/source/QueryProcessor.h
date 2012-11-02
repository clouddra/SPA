#ifndef DECLARATION_HEAD
#define DECLARATION_HEAD
#include "DeclarationTable.h"
#endif

#ifndef QUERYTREE_HEAD
#define QUERYTREE_HEAD
#include "QueryTree.h"
#endif

#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

class QueryProcessor {

private:
	QueryTree queryTree;
	DeclarationTable declarationTable;
	std::vector<std::string> result;

public:
	QueryProcessor();
	int insertNode(std::string, std::string, int parent);
	void processQuery(PKB pkb);
    void printTree();
	void printResult();
};
