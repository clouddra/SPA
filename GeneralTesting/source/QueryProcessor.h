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

#ifndef RS_HEAD
#define RS_HEAD
#include "ResultStorage.h"
#endif

#ifndef STD_HEAD
#define STD_HEAD
#include "Common.hpp"
#endif

#include "AbstractWrapper.h"

class QueryProcessor {

private:
	QueryTree queryTree;
	DeclarationTable declarationTable;
    ResultStorage resultStore;
	std::vector<std::string> result;
    void loadDeclaration(std::vector<QueryNode> tree, int* curr);
    int findTypeOf(std::string para, bool* paraIsNum, bool* paraIsEnt,bool* paraIsPlaceholder, int* paraNum);
    int evaluateFollows(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb);
    int evaluateParent(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb);
    int evaluateNext(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb);
    int evaluateModifiesS(bool para1IsNum, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, PKB pkb);
    int evaluateModifiesP(bool para1IsEnt, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB pkb);
    int evaluateUsesS(bool para1IsNum, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, PKB pkb);
    int evaluateUsesP(bool para1IsEnt, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB pkb);
    int evaluateCalls(bool para1IsEnt, bool para1IsPlaceholder, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB pkb);
    int evaluateType(PKB pkb, std::string target);
	int evaluateAssignPattern(std::string pattern, std::string var, int exprRoot, bool varIsEnt, bool varIsPlaceholder, bool hasUnderscore, PKB pkb);
    int evaluateIfWhilePattern(std::string pattern, std::string var, bool varIsEnt, bool varIsPlaceholder, PKB pkb);
    int attrRefChecker(std::string* synonym, std::string* attrName, int* synoType, QueryNode attrRef, PKB pkb);

public:
	QueryProcessor();
	int insertNode(std::string, std::string, int parent);
	void processQuery(PKB pkb);
    void printTree();
	void printResult();
    std::list<std::string> getResult();
};
