#include <vector>
#include <iostream>
#ifndef NODE_HEAD
#define NODE_HEAD
#include "Node.h"

#endif
#ifndef MODIFIES_HEAD
#define MODIFIES_HEAD
#include "ModifiesTable.h"
#endif

#ifndef PARENT_HEAD
#define PARENT_HEAD
#include "ParentTable.h"
#endif

#ifndef FOLLOWS_HEAD
#define FOLLOWS_HEAD
#include "FollowsTable.h"
#endif

#ifndef USESTABLE_HEAD
#define USESTABLE_HEAD
#include "UsesTable.h"
#endif

#ifndef CALLS_HEAD
#define CALLS_HEAD
#include "CallsTable.h"
#endif

#ifndef STMTNODE_HEAD
#define STMTNODE_HEAD
#include "StmtNodeTable.h"
#endif

#ifndef AST_HEAD
#define AST_HEAD
#include "AST.h"
#endif

#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

class DesignExtractor {

private:
	AST _ast;
	PKB* _pkb;
	ModifiesTable* _mt;
	FollowsTable* _ft;
	UsesTable* _ut;
	CallsTable* _ct;
	ParentTable* _pt;
	StmtNodeTable* _stmtt;
	void checkChildrenUses(int nodeIndex, std::vector<int> parents);
	//void checkChildrenCalls(int caller, int nodeIndex);
	void checkParentIfProc(int nodeIndex, int callNodeIndex);
	void DesignExtractor::insertFollows(int stmt1, int stmt2);
	void DesignExtractor::insertParent(int stmt1, int stmt2);
	void DesignExtractor::insertUses(int stmt1, int stmt2);
	void DesignExtractor::insertModifies(int stmt1, int stmt2);
	void DesignExtractor::insertCalls(int stmt1, int stmt2);

public:
    DesignExtractor(PKB* pkb);
	DesignExtractor() ;
	void populateTables();
};