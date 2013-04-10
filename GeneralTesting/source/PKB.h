#ifndef PARENT_HEAD
#define PARENT_HEAD
#include "ParentTable.h"
#endif

#ifndef FOLLOWS_HEAD
#define FOLLOWS_HEAD
#include "FollowsTable.h"
#endif

#ifndef MODIFIES_HEAD
#define MODIFIES_HEAD
#include "ModifiesTable.h"
#endif

#ifndef USESTABLE_HEAD
#define USESTABLE_HEAD
#include "UsesTable.h"
#endif

#ifndef CONTAINS_HEAD
#define CONTAINS_HEAD
#include "ContainsTable.h"
#endif

#ifndef SIBLING_HEAD
#define SIBLING_HEAD
#include "SiblingTable.h"
#endif

#ifndef VARTABLE_HEAD
#define VARTABLE_HEAD
#include "VarTable.h"
#endif

#ifndef PROCTABLE_HEAD
#define PROCTABLE_HEAD
#include "ProcTable.h"
#endif

#ifndef AST_HEAD
#define AST_HEAD
#include "AST.h"
#endif

#ifndef STMTNODE_HEAD
#define STMTNODE_HEAD
#include "StmtNodeTable.h"
#endif

#ifndef NODETABLE_HEAD
#define NODETABLE_HEAD
#include "NodeTable.h"
#endif

#ifndef STD_HEAD
#define STD_HEAD
#include "Common.hpp"
#endif

#ifndef CALLS_HEAD
#define CALLS_HEAD
#include "CallsTable.h"
#endif

#ifndef QUERYNODE_HEAD
#define QUERYNODE_HEAD
#include "QueryNode.h"
#endif

#ifndef CFG_HEAD
#define CFG_HEAD
#include "CFG.h"
#endif

#ifndef CFG_BIP
#define CFG_BIP
#include "CFGBip.h"
#endif

#ifndef QUEUE_HEAD
#define QUEUE_HEAD
#include <queue>
#endif

class PKB {

private:
    AST ast;
	CFG cfg;
	CFGBip cfgBip;
    ModifiesTable modifiesTable;
    ParentTable parentTable;
    FollowsTable followsTable;
	CallsTable callsTable;
	ContainsTable containsTable;
	SiblingTable siblingTable;
    VarTable varTable;
	ProcTable procTable;
    UsesTable usesTable;
    StmtNodeTable stmtNodeTable;
    NodeTable nodeTable;
    std::set<int> constantList;
	std::unordered_map<int, std::vector<int>> affectsMapStart;
	std::unordered_map<int, std::vector<int>> affectsMapEnd;
	std::unordered_map<int, std::vector<int>> affectsTMapStart;
	std::unordered_map<int, std::vector<int>> affectsTMapEnd;
	void addCFGtoStmtNodeTable(int cfgNode, int startStmt, int endStmt);
	void addCFGBiptoStmtNodeTable(int cfgBipNode, int startStmt, int endStmt);
    std::vector<int> convertToNodeIndex(std::string input, int type);
    std::vector<std::string> convertToStorageType(std::vector<int> result, int type);

public:
    PKB();
    int insertNode(int nodeType, std::string value, int parent);
    std::vector<std::string> getContainer(std::string input, int inputType, int outputType);
    std::vector<std::string> getContainerT(std::string input, int inputType, int outputType);
	std::vector<std::string> getContainedIn(std::string input, int inputType, int outputType);
    std::vector<std::string> getContainedInT(std::string input, int inputType, int outputType);
    bool isContains(std::string input1, int type1, std::string input2, int type2);
    std::vector<int> getParent(int stmt);
    std::vector<int> getParent();
    std::vector<int> getParentT(int stmt);
	std::vector<int> getChild(int stmt);
    std::vector<int> getChild();
    std::vector<int> getChildT(int stmt);
    bool isParent(int parent, int child);
    std::vector<int> getFollows(int stmt);
    std::vector<int> getFollows();
    std::vector<int> getFollowsT(int stmt);
	std::vector<int> getFollowedBy(int stmt);
    std::vector<int> getFollowedBy();
    std::vector<int> getFollowedByT(int stmt);
    bool isFollow(int first, int second);
    std::vector<std::string> getCalls(std::string proc);
    std::vector<std::string> getCalls();
    std::vector<std::string> getCallsT(std::string proc);
	std::vector<std::string> getCalledBy(std::string proc);
    std::vector<std::string> getCalledBy();
    std::vector<std::string> getCalledByT(std::string proc);
    bool isCalls(std::string proc1, std::string proc2);
    std::vector<int> getModifiesVar(std::string var);
    std::vector<int> getModifiesVar();
	std::vector<std::string> getModifiedBy(int stmt);
    std::vector<std::string> getModifiedBy();
    bool isModifies(int stmt, std::string var);
    std::vector<std::string> getModifiesVarP(std::string var);
    std::vector<std::string> getModifiesVarP();
	std::vector<std::string> getModifiedByP(std::string proc);
    std::vector<std::string> getModifiedByP();
    bool isModifiesP(std::string proc, std::string var);
    std::vector<int> getUsesVar(std::string var);
    std::vector<int> getUsesVar();
	std::vector<std::string> getUsedBy(int stmt);
    std::vector<std::string> getUsedBy();
    bool isUses(int stmt, std::string var);
    std::vector<std::string> getUsesVarP(std::string var);
    std::vector<std::string> getUsesVarP();
	std::vector<std::string> getUsedByP(std::string proc);
    std::vector<std::string> getUsedByP();
    bool isUsesP(std::string proc, std::string var);
    std::vector<int> getNext(int stmt);
    std::vector<int> getNext();
	std::vector<int> getNextT(int stmt);
	std::vector<int> getPrev(int stmt);
    std::vector<int> getPrev();
    std::vector<int> getPrevT(int stmt);
    bool isNext(int stmt1, int stmt2);
    std::vector<int> getNextBip(int stmt);
    std::vector<int> getNextBip();
	std::vector<int> getNextBipT(int stmt);
	std::vector<int> getPrevBip(int stmt);
    std::vector<int> getPrevBip();
    std::vector<int> getPrevBipT(int stmt);
    bool isNextBip(int stmt1, int stmt2);
    std::vector<int> getStmtWithType(int nodeType);
    std::vector<std::string> getVarTable();
    int getNumStmts();
    std::vector<std::string> getAllProc();
    void postParseCleanup();
    std::vector<int> getNodeIndexes(int type);
    std::vector<std::string> convertStmtLst(std::vector<int> nodeIndexes);
	std::vector<int> matchIfWhilePattern(int nodeType, std::string varName);
	std::vector<int> matchAssignPattern(std::string varName, std::vector<QueryNode> queryTree, int patternRoot, bool hasUnderscore);
	Node qNodeToNode(QueryNode qNode);
	bool treeCompare(int astNodeIndex, int qNodeIndex, std::vector<QueryNode> queryTree);
	bool subtreeCompare(int astNodeIndex, int qNodeIndex, std::vector<QueryNode> queryTree);
    std::set<int> getConstants();
	void startBuildCfg();
	void startBuildCfgBip();
	std::vector<int> buildCfg(int stmtListAst, int cfgIndex, std::vector<std::pair<int,int>>& callList);
	std::vector<int> getAffectsStart(int start); 
	std::vector<int> getAffectsEnd(int end);
	std::vector<int> depthUp(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited);
	std::vector<int> getAffectsTStart(int start);
	std::vector<int> getAffectsTEnd(int end);
	std::vector<int> depthUpT(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::unordered_set<int> ignoreSet);
	std::vector<int> depthDownT(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::unordered_set<int> ignoreSet);
	std::vector<int> getAffectsStartAPI(int start);
	std::vector<int> getAffectsEndAPI(int end);
	std::vector<int> getAffectsTStartAPI(int start);
	std::vector<int> getAffectsTEndAPI(int end);

	void printModifiesTable();
	AST PKB::getAST();
	ModifiesTable* getModifiesTable();
	ParentTable* getParentTable();
	FollowsTable* getFollowsTable();
	UsesTable* getUsesTable();
	CallsTable* getCallsTable();
	StmtNodeTable* getStmtNodeTable();
	ProcTable* getProcTable();
	ContainsTable* getContainsTable();
	SiblingTable* getSiblingTable();		
};