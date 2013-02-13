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

#ifndef VARTABLE_HEAD
#define VARTABLE_HEAD
#include "VarTable.h"
#endif

#ifndef USESTABLE_HEAD
#define USESTABLE_HEAD
#include "UsesTable.h"
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

#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

#ifndef CALLS_HEAD
#define CALLS_HEAD
#include "CallsTable.h"
#endif

class PKB {

private:
    AST ast;
    ModifiesTable modifiesTable;
    ParentTable parentTable;
    FollowsTable followsTable;
    VarTable varTable;
    UsesTable usesTable;
    ProcTable procTable;
    StmtNodeTable stmtNodeTable;
    std::set<int> constantList;

public:
    PKB();
    int insertNode(int nodeType, std::string value, int parent);
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
    std::vector<int> getModifiesVar(std::string var);
    std::vector<int> getModifiesVar();
	std::vector<std::string> getModifiedBy(int stmt);
    std::vector<std::string> getModifiedBy();
    bool isModifies(int stmt, std::string var);
    std::vector<int> getUsesVar(std::string var);
    std::vector<int> getUsesVar();
	std::vector<std::string> getUsedBy(int stmt);
    std::vector<std::string> getUsedBy();
    bool isUses(int stmt, std::string var);
    std::vector<int> getStmtWithType(int nodeType);
    std::vector<std::string> getVarTable();
    int getNumStmts();
    void postParseCleanup();
	std::vector<int> matchPattern(int nodeType, std::string varName, std::string pattern);
    std::set<int> getConstants();

	void printModifiesTable();
	AST PKB::getAST();
	ModifiesTable* getModifiesTable();
	ParentTable* getParentTable();
	FollowsTable* getFollowsTable();
	UsesTable* getUsesTable();
	StmtNodeTable* getStmtNodeTable();
};