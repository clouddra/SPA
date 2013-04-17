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

#ifndef QUEUE_HEAD
#define QUEUE_HEAD
#include <queue>
#endif

class PKB {

private:
    AST ast;
	CFG cfg;
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
	std::unordered_map<int, std::vector<int>> nextTMap;
	std::unordered_map<int, std::vector<int>> prevTMap;
	std::unordered_map<int, std::vector<int>> affectsMapStart;
	std::unordered_map<int, std::vector<int>> affectsMapEnd;
	std::unordered_map<int, std::vector<int>> affectsTMapStart;
	std::unordered_map<int, std::vector<int>> affectsTMapEnd;
	std::unordered_map<int, std::vector<int>> affectsBipMapStart;
	std::unordered_map<int, std::vector<int>> affectsBipMapEnd;
	std::unordered_map<int, std::vector<int>> affectsBipTMapStart;
	std::unordered_map<int, std::vector<int>> affectsBipTMapEnd;
	void addCFGtoStmtNodeTable(int cfgNode, int startStmt, int endStmt);
    std::vector<int> convertToNodeIndex(std::string input, int type);
    std::vector<std::string> convertToStorageType(std::vector<int> result, int type);
	Node qNodeToNode(QueryNode qNode);
	std::vector<int> getNextT(int stmt);
    std::vector<int> getPrevT(int stmt);
	std::vector<int> buildCfg(int stmtListAst, int cfgIndex, std::vector<std::pair<int,int>>& callList);
	std::vector<int> getAffectsStart(int start); 
	std::vector<int> getAffectsEnd(int end);
	std::vector<int> depthUp(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited);
	std::vector<int> getAffectsTStart(int start);
	std::vector<int> getAffectsTEnd(int end);
	std::vector<int> depthUpT(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::unordered_set<int> ignoreSet);
	std::vector<int> depthDownT(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::unordered_set<int> ignoreSet);
	std::vector<int> depthDownBip(int currStmt, int var, std::vector<int> visited, std::vector<int> branchIn);
	std::vector<int> depthUpBip(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::vector<int> branchIn);
	std::vector<int> depthDownBipT(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::vector<int> branchIn, std::unordered_set<int> ignoreSet);
	std::vector<int> depthUpBipT(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::vector<int> branchIn, std::unordered_set<int> ignoreSet);
	std::vector<int> getAffectsBipStart(int start);
	std::vector<int> getAffectsBipEnd(int end);	
	std::vector<int> getAffectsBipTStart(int start);
	std::vector<int> getAffectsBipTEnd(int end);

public:
    PKB();
    int insertNode(int nodeType, std::string value, int parent);

	/**
	 * Return all containers of given input in the wanted outputType
	 */
    std::vector<std::string> getContainer(std::string input, int inputType, int outputType);

	/**
	 * Return all containers* of given input in the wanted outputType (includes indirect containers)
	 */
    std::vector<std::string> getContainerT(std::string input, int inputType, int outputType);

	/**
	 * Return all items contained in given input in the wanted outputType
	 */
	std::vector<std::string> getContainedIn(std::string input, int inputType, int outputType);

	/**
	 * Return all items contained* in given input in the wanted outputType (includes indirect containment)
	 */
    std::vector<std::string> getContainedInT(std::string input, int inputType, int outputType);

	/**
	 * Returns TRUE if input1 is the container of input2.
     * Otherwise, return FALSE.
	 */
    bool isContains(std::string input1, int type1, std::string input2, int type2);

	/**
	 * Returns all siblings of given input
	 */
    std::vector<std::string> getSibling(std::string input, int inputType, int outputType);

	/**
	 * Returns TRUE if input1 is a sibling of input2.
     * Otherwise, return FALSE.
	 */
    bool isSibling(std::string input1, int type1, std::string input2, int type2);

	/**
	 * Returns the parent stmtNum of given stmt
	 */
    std::vector<int> getParent(int stmt);

	/**
	 * Returns all stmtNums which are parents
	 */
    std::vector<int> getParent();

	/**
	 * Returns the parent* stmtNum of given stmt (include ancestors)
	 */
    std::vector<int> getParentT(int stmt);

	/**
	 * Returns the children stmtNums of given stmt
	 */
	std::vector<int> getChild(int stmt);

	/**
	 * Returns all stmtNums which are children
	 */
    std::vector<int> getChild();

	/**
	 * Returns the child* stmtNum of given stmt (include descendants)
	 */
    std::vector<int> getChildT(int stmt);

	/**
	 * Returns TRUE if "parent" is direct parent of "child"
	 * Otherwise, returns FALSE
	 */
    bool isParent(int parent, int child);

	/**
	 * Returns the stmtNums directly after the given stmt (at same nesting level)
	 */
    std::vector<int> getFollows(int stmt);

	/**
	 * Returns all stmtNums that appear directly after some stmt
	 */
    std::vector<int> getFollows();

	/**
	 * Returns the stmtNums after the given stmt (include indirect stmts, at same nesting level)
	 */
    std::vector<int> getFollowsT(int stmt);

	/**
	 * Returns the stmtNums directly before the given stmt (at same nesting level)
	 */
	std::vector<int> getFollowedBy(int stmt);

	/**
	 * Returns all stmtNums that have some stmt directly after it
	 */
    std::vector<int> getFollowedBy();

	/**
	 * Returns the stmtNums before the given stmt (include indirect stmts, at same nesting level)
	 */
    std::vector<int> getFollowedByT(int stmt);

	/**
	 * Returns TRUE if "first" is directly before "second" (at the same nesting level)
	 * Otherwise, returns FALSE
	 */
    bool isFollow(int first, int second);

	/**
	 * Returns all the procedures that "proc" calls
	 */
    std::vector<std::string> getCalls(std::string proc);

	/**
	 * Returns all the procedures that were called (appeared in call stmts)
	 */
    std::vector<std::string> getCalls();

	/**
	 * Returns all the procedures that "proc" calls* (include indirect calls)
	 */
    std::vector<std::string> getCallsT(std::string proc);

	/**
	 * Returns all the procedures that called "proc"
	 */
	std::vector<std::string> getCalledBy(std::string proc);

	/**
	 * Returns all the procedures that called some other procedure
	 */
    std::vector<std::string> getCalledBy();

	/**
	 * Returns all the procedures that called "proc" (include indirect calls)
	 */
    std::vector<std::string> getCalledByT(std::string proc);

	/**
	 * Returns TRUE if "proc1" directly calls "proc2"
	 * Otherwise, returns FALSE
	 */
    bool isCalls(std::string proc1, std::string proc2);

	/**
	 * Returns all statements that modifies variable var.
	 */
    std::vector<int> getModifiesVar(std::string var);

	/**
	 * Returns all statements that modifies any variable.
	 */
    std::vector<int> getModifiesVar();

	/**
	 * Returns all variables that are modified by statement stmt.
	 */
	std::vector<std::string> getModifiedBy(int stmt);

	/**
	 * Returns all variables that are modified by any statement.
	 */
    std::vector<std::string> getModifiedBy();

	/**
	 * Returns TRUE if stmt modifies var.
	 * Otherwise, return FALSE.
	 */
    bool isModifies(int stmt, std::string var);

	/**
	 * Return all procedures that modify variable var.
	 */
    std::vector<std::string> getModifiesVarP(std::string var);

	/**
	 * Returns all procedures that modify any variables.
	 */
    std::vector<std::string> getModifiesVarP();

	/**
	 * Returns all variables modified by procedure proc
	 */
	std::vector<std::string> getModifiedByP(std::string proc);

	/**
	 * Returns all variables that is modified by any procedure.
	 */
    std::vector<std::string> getModifiedByP();

	/**
	 * Returns TRUE if proc modifies var.
	 * Otherwise, return FALSE.
	*/
    bool isModifiesP(std::string proc, std::string var);

	/**
	 * Returns all statements that uses variable var.
	 */
    std::vector<int> getUsesVar(std::string var);

	/**
	 * Returns all statements that uses any variables.
	 */
    std::vector<int> getUsesVar();

	/**
	 * Returns all variables that is used by statement stmt.
	 */
	std::vector<std::string> getUsedBy(int stmt);

	/**
	 * Returns all variables that is used by any statements.
	 */
    std::vector<std::string> getUsedBy();

	/**
	 * Returns TRUE if stmt uses var.
	 * Otherwise, return FALSE.
	 */
    bool isUses(int stmt, std::string var);

	/**
	 * Return all procedures that use variable var.
	 */
    std::vector<std::string> getUsesVarP(std::string var);

	/**
	 * Returns all procedures that uses any variables.
	 */
    std::vector<std::string> getUsesVarP();

	/**
	 * Returns all variables used by procedure proc
	 */
	std::vector<std::string> getUsedByP(std::string proc);

	/**
	 * Returns all variables that is used by any procedures.
	 */
    std::vector<std::string> getUsedByP();

	/**
	 * Returns TRUE if proc uses var.
	 * Otherwise, return FALSE.
	 */
    bool isUsesP(std::string proc, std::string var);

	/**
	 * Returns all statements directly after given stmt in the CFG
	 */
    std::vector<int> getNext(int stmt);

	/**
	 * Returns all statements that are not the first stmt of a procedure
	 */
    std::vector<int> getNext();

	/**
	 * Returns all statements directly/indirectly after given stmt in the CFG
	 */
	std::vector<int> getNextTAPI(int stmt);

	/**
	 * Returns all statements directly before given stmt in the CFG
	 */
	std::vector<int> getPrev(int stmt);

	/**
	 * Returns all statements that are not the last stmt of a procedure (in CFG terms)
	 */
    std::vector<int> getPrev();

	/**
	 * Returns all statements directly/indirectly before given stmt in the CFG
	 */
	std::vector<int> getPrevTAPI(int stmt);

	/**
	 * Returns TRUE if stmt1 is followed by stmt2 in CFG
	 * Otherwise, return FALSE.
	 */
    bool isNext(int stmt1, int stmt2);

	/**
	 * Returns all statements directly after given stmt in the CFGBip (will branch out if curr stmt = call)
	 * If at last stmt of proc, will branch to after all call stmts that called the proc
	 */
    std::vector<int> getNextBip(int stmt);

	/**
	 * Returns all statements directly after some stmt in CFGBip (will branch out if curr stmt = call)
	 */
    std::vector<int> getNextBip();

	/**
	 * Returns all statements after given stmt in the CFGBip (include indirect statements)
	 */
	std::vector<int> getNextBipT(int stmt);

	/**
	 * Returns all statements directly before given stmt in the CFGBip (will branch out if prev stmt = call)
	 * If at first stmt of proc, will branch to all call stmts that called the proc
	 */
	std::vector<int> getPrevBip(int stmt);

	/**
	 * Returns all statements directly before some stmt in CFGBip (will branch out if prev stmt = call)
	 */
    std::vector<int> getPrevBip();

	/**
	 * Returns all statements before given stmt in the CFGBip (include indirect statements)
	 */
    std::vector<int> getPrevBipT(int stmt);

	/**
	 * Returns TRUE if stmt1 is followed by stmt2 in CFGBip
	 * Otherwise, return FALSE.
	 */
    bool isNextBip(int stmt1, int stmt2);

	/**
	 * Returns all statements corresponding to the given nodeType
	 */
    std::vector<int> getStmtWithType(int nodeType);

	/**
	 * Returns the varTable
	 */
    std::vector<std::string> getVarTable();

	/**
	 * Returns the total number of statements
	 */
    int getNumStmts();

	/**
	 * Returns the names of all procedures
	 */
    std::vector<std::string> getAllProc();


    void postParseCleanup();
    std::vector<int> getNodeIndexes(int type);
    std::vector<std::string> convertStmtLst(std::vector<int> nodeIndexes);
    std::vector<std::vector<std::string>> addStmtLst(std::vector<int> stmts, bool addStmtLst1, bool addStmtLst2);

	/**
	 * Evaluates Patten "if" or Pattern "while"
	 */
	std::vector<int> matchIfWhilePattern(int nodeType, std::string varName);

	/**
	 * Evaluates Patten "assign"
	 */
	std::vector<int> matchAssignPattern(std::string varName, std::vector<QueryNode> queryTree, int patternRoot, bool hasUnderscore);
	
	/**
	 * Determines if AST subtree is an exact match, example Pattern a(x, "x+y")
	 */
	bool treeCompare(int astNodeIndex, int qNodeIndex, std::vector<QueryNode> queryTree);

	/**
	 * Determines if AST subtree is partially contained, example Pattern a(x, _"x+y"_)
	 */
	bool subtreeCompare(int astNodeIndex, int qNodeIndex, std::vector<QueryNode> queryTree);

	/**
	 * Returns all constants' values
	 */
    std::set<int> getConstants();

	/**
	 * Builds the CFG
	 */
	void startBuildCfg();

	/**
	 * Builds the CFGBip
	 */
	void startBuildCfgBip();

	/**
	 * Returns a1 for Affects(start, a1), where start is the given statement
	 */
	std::vector<int> getAffectsStartAPI(int start);

	/**
	 * Returns a1 for Affects(a1, end), where end is the given statement
	 */
	std::vector<int> getAffectsEndAPI(int end);

	/**
	 * Returns a1 for Affects*(start, a1), where start is the given statement
	 */
	std::vector<int> getAffectsTStartAPI(int start);

	/**
	 * Returns a1 for Affects*(a1, end), where end is the given statement
	 */
	std::vector<int> getAffectsTEndAPI(int end);

	/**
	 * Returns a1 for AffectsBip(start, a1), where start is the given statement
	 */
	std::vector<int> getAffectsBipStartAPI(int start);

	/**
	 * Returns a1 for AffectsBip(a1, end), where end is the given statement
	 */
	std::vector<int> getAffectsBipEndAPI(int end);	

	/**
	 * Returns a1 for AffectsBip*(start, a1), where start is the given statement
	 */
	std::vector<int> getAffectsBipTStartAPI(int start);

	/**
	 * Returns a1 for AffectsBip*(a1, end), where end is the given statement
	 */
	std::vector<int> getAffectsBipTEndAPI(int end);

	/**
	 * Prints the modifiesTable
	 */
	void printModifiesTable();

	/**
	 * Returns the ast
	 */
	AST PKB::getAST();

	/**
	 * Returns a reference to modifiesTable
	 */
	ModifiesTable* getModifiesTable();

	/**
	 * Returns a reference to parentTable
	 */
	ParentTable* getParentTable();

	/**
	 * Returns a reference to followsTable
	 */
	FollowsTable* getFollowsTable();

	/**
	 * Returns a reference to usesTable
	 */
	UsesTable* getUsesTable();

	/**
	 * Returns a reference to callsTable
	 */
	CallsTable* getCallsTable();

	/**
	 * Returns a reference to stmtNodeTable
	 */
	StmtNodeTable* getStmtNodeTable();

	/**
	 * Returns a reference to procTable
	 */
	ProcTable* getProcTable();

	/**
	 * Returns a reference to containsTable
	 */
	ContainsTable* getContainsTable();

	/**
	 * Returns a reference to siblingTable
	 */
	SiblingTable* getSiblingTable();		
};