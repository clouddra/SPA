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

#ifndef QUERYGRAPH_HEAD
#define QUERYGRAPH_HEAD
#include "QueryGraph.h"
#endif

#ifdef ENABLE_THREADING
#include "Threading.h"
#endif

#include "AbstractWrapper.h"

class QueryProcessor {

private:
	QueryTree queryTree;
	DeclarationTable declarationTable;
    ResultStorage resultStore;
	std::vector<std::string> result;
    std::vector<QueryGraph> optiGraphs;

    void loadDeclaration(std::vector<QueryNode> tree, int* curr);
    int findTypeOf(std::string para, bool* paraIsNum, bool* paraIsEnt,bool* paraIsPlaceholder, int* paraNum);
    int evaluateContains(bool T, bool para1IsNum, bool para2IsNum, std::string para1, std::string para2, int para1Num, int para2Num, int para1Type, int para2Type, PKB& pkb);
    int evaluateSibling(bool para1IsNum, bool para2IsNum, std::string para1, std::string para2, int para1Num, int para2Num, int para1Type, int para2Type, PKB& pkb);
    int evaluateFollows(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB& pkb);
    int evaluateParent(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB& pkb);
    int evaluateNext(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB& pkb);
    int evaluateNextBip(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB& pkb);
    int evaluateAffects(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB& pkb);
    int evaluateAffectsBip(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB& pkb);
    int evaluateModifiesS(bool para1IsNum, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, PKB& pkb);
    int evaluateModifiesP(bool para1IsEnt, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB& pkb);
    int evaluateUsesS(bool para1IsNum, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, PKB& pkb);
    int evaluateUsesP(bool para1IsEnt, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB& pkb);
    int evaluateCalls(bool T, bool para1IsEnt, bool para1IsPlaceholder, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB& pkb);
    int evaluateType(PKB& pkb, std::string target);
	int evaluateAssignPattern(std::string pattern, std::string var, int exprRoot, bool varIsEnt, bool varIsPlaceholder, bool hasUnderscore, PKB& pkb);
    int evaluateIfWhilePattern(std::string pattern, std::string var, bool varIsEnt, bool varIsPlaceholder, PKB& pkb);
    int evaluateIfPatternStmtLst(std::string pattern, std::string var, bool varIsEnt, bool varIsPlaceholder, std::string stmtLst1, bool SL1IsPlaceholder, std::string stmtLst2, bool SL2IsPlaceholder, PKB& pkb);
    int evaluateWhilePatternStmtLst(std::string pattern, std::string var, bool varIsEnt, bool varIsPlaceholder, std::string stmtLst, PKB& pkb);
    int attrRefChecker(std::string* synonym, std::string* attrName, int* synoType, QueryNode attrRef, PKB& pkb);
    std::vector<std::vector<int>> optimizeQuery(std::vector<QueryNode> tree, int curr, PKB& pkb);

public:
	QueryProcessor();

    /**
	 * Insert a node into the queryTree, used by PQLParser to build the queryTree
     * Returns 0 if successful and -1 if not
	 */
	int insertNode(std::string, std::string, int parent);

    /**
	 * Function called by SPAController to start query processing (after queryTree is built)
	 */
	void processQuery(PKB& pkb);

    /**
	 * Print out the queryTree
	 */
    void printTree();

    /**
	 * Print out the result vector
	 */
	void printResult();

    /**
	 * Getter function for the result vector
	 */
    std::list<std::string> getResult();
};
