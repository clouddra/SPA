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

#ifndef VVTUPLETABLE_HEAD
#define VVTUPLETABLE_HEAD
#include "ValidValueTupleTable.h"
#endif

#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

#include "AbstractWrapper.h"

class QueryProcessor {

private:
	QueryTree queryTree;
	DeclarationTable declarationTable;
    ValidValueTable vvTable;
    ValidValueTupleTable vvTupleTable;
	std::vector<std::string> result;
    void loadDeclaration(std::vector<QueryNode> tree, int* curr);
    int findTypeOf(std::string para, bool* paraIsNum, bool* paraIsEnt,bool* paraIsPlaceholder, int* paraNum);
    int evaluateFollows(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb);
    int evaluateParent(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb);
    int evaluateModifiesS(bool para1IsNum, bool para1IsPlaceholder, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, PKB pkb);
    int evaluateUsesS(bool para1IsNum, bool para1IsPlaceholder, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, PKB pkb);
    int evaluateType(PKB pkb, std::string target);
	int evaluatePattern(std::string pattern, std::string var, std::string expr, bool varIsEnt, bool varIsPlaceholder, PKB pkb);

public:
	QueryProcessor();
	int insertNode(std::string, std::string, int parent);
	void processQuery(PKB pkb);
    void printTree();
	void printResult();
    std::list<std::string> getResult();
};
