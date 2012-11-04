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

#ifndef VVTABLE_HEAD
#define VVTABLE_HEAD
#include "ValidValueTable.h"
#endif

#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

class QueryProcessor {

private:
	QueryTree queryTree;
	DeclarationTable declarationTable;
    ValidValueTable vvTable;
	std::vector<std::string> result;
    void loadDeclaration(std::vector<QueryNode> tree, int* curr);
    int findTypeOf(std::string para, bool* paraIsNum, bool* paraIsEnt, int* paraNum);
    int evaluateFollows(bool T, bool para1IsNum, bool para2IsNum, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb);
    int evaluateParent(bool T, bool para1IsNum, bool para2IsNum, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb);
    int evaluateModifiesS(bool para1IsNum, bool para2IsEnt, std::string para1, std::string para2, int para1Num, PKB pkb);
    int evaluateUsesS(bool para1IsNum, bool para2IsEnt, std::string para1, std::string para2, int para1Num, PKB pkb);
    int evaluateType(PKB pkb, std::string target, bool insertStmt);

public:
	QueryProcessor();
	int insertNode(std::string, std::string, int parent);
	void processQuery(PKB pkb);
    void printTree();
	void printResult();
};
