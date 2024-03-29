#include <iostream>

#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

#ifndef MODIFIES_HEAD
#define MODIFIES_HEAD
#include "ModifiesTable.h"
#endif
PKB::PKB() {
    modifiesTable = ModifiesTable();
	parentTable = ParentTable();
	followsTable = FollowsTable();
    varTable = VarTable();
    usesTable = UsesTable();
    procTable = ProcTable();
    ast = AST();
    stmtNodeTable = StmtNodeTable();
}
void PKB::printModifiesTable()
{
	for(int i=0;i<modifiesTable.getSize();i++)
	{
		std::cout << modifiesTable.extractModifies(i).first<< " "<<modifiesTable.extractModifies(i).second << std::endl;
	}
}

void PKB::printUsesTable()
{
	for(int i=0;i<usesTable.getSize();i++)
	{
		std::cout << usesTable.extractUses(i).first<< " "<<usesTable.extractUses(i).second << std::endl;
	}
}

void PKB::printFollowsTable()
{
	for(int i=0;i<followsTable.getSize();i++)
	{
		std::cout << followsTable.extractFollows(i).first<< " "<<followsTable.extractFollows(i).second << std::endl;
	}
}
int PKB::insertNode(int nodeType, std::string value, bool hasStmtNum, int parent) {
    int indexValue = -1;
    bool newStmtFlag = false;

    switch (nodeType) {
    case Node::assignNode:
    case Node::whileNode:
    case Node::ifNode:
    case Node::callNode:
        newStmtFlag = true;
        break;

    case Node::varNode:
        indexValue = varTable.insertVar(value);
        break;

    case Node::constNode:
        indexValue = atoi(value.c_str());
        break;

    case Node::procedureNode:
        indexValue = procTable.insertProc(value, 0, 0);
        break;
    }

    int stmtNum = -1;
    if (hasStmtNum) {
        if (newStmtFlag)
            stmtNum = stmtNodeTable.getSize();
        else
            stmtNum = stmtNodeTable.getSize() - 1;
    }
    int newNode = ast.insertNode(nodeType, indexValue, stmtNum, parent);
    if (newStmtFlag) {
        stmtNodeTable.insertStmtNode(stmtNum, newNode, nodeType);
    }

    return newNode;
}
ModifiesTable* PKB::getModifiesTable() {
    return &modifiesTable;
}

ParentTable* PKB::getParentTable() {
    return &parentTable;
}

FollowsTable* PKB::getFollowsTable() {
    return &followsTable;
}

UsesTable* PKB::getUsesTable() {
    return &usesTable;
}
AST PKB::getAST()
{
	return ast;
}
StmtNodeTable* PKB::getStmtNodeTable()
{
	return &stmtNodeTable;
}

int PKB::getParent(int stmt) {
    return parentTable.getParent(stmt);    
}

std::vector<int> PKB::getParentT(int stmt) {
    std::vector<int> ans;
    int curr = stmt;
    while (curr != -1) {
        curr = getParent(curr);
        if (curr != -1)
            ans.push_back(curr);
    }
    return ans;
}

std::vector<int> PKB::getChild(int stmt) {
    return parentTable.getChild(stmt);
}

std::vector<int> PKB::getChildT(int stmt) {
    int curr = 0;
    std::vector<int> ans = getChild(stmt);
    
    while (curr < ans.size()) {
        std::vector<int> temp = getChild(ans[curr]);
        for (int i = 0; i < temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

bool PKB::isParent(int parent, int child) {
    return parentTable.isParent(parent, child);
}

std::vector<int> PKB::getFollows(int stmt) {
    return followsTable.getFollows(stmt);
}

std::vector<int> PKB::getFollowsT(int stmt) {
    int curr = 0;
    std::vector<int> ans = getFollows(stmt);
    
    while (curr < ans.size()) {
        std::vector<int> temp = getFollows(ans[curr]);
        for (int i = 0; i < temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

std::vector<int> PKB::getFollowedBy(int stmt) {
    return followsTable.getFollowedBy(stmt);
}

std::vector<int> PKB::getFollowedByT(int stmt) {
    int curr = 0;
    std::vector<int> ans = getFollowedBy(stmt);
    
    while (curr < ans.size()) {
        std::vector<int> temp = getFollowedBy(ans[curr]);
        for (int i = 0; i < temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

bool PKB::isFollow(int first, int second) {
    return followsTable.isFollows(first, second);
}

std::vector<int> PKB::getModifiesVar(std::string var) {
    int varIndex = varTable.getVarIndex(var);
    return modifiesTable.getModifiesVar(varIndex);
}

std::vector<std::string> PKB::getModifiedBy(int stmt) {
    std::vector<std::string> toReturn;
    std::vector<int> answer = modifiesTable.getModifiedBy(stmt);
    for (int i = 0; i < answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

bool PKB::isModifies(int stmt, std::string var) {
    int varIndex = varTable.getVarIndex(var);
    return modifiesTable.isModifies(stmt, varIndex);
}

std::vector<int> PKB::getUsesVar(std::string var) {
    int varIndex = varTable.getVarIndex(var);
    return usesTable.getUsesVar(varIndex);
}

std::vector<std::string> PKB::getUsedBy(int stmt) {
    std::vector<std::string> toReturn;
    std::vector<int> answer = usesTable.getUsedBy(stmt);
    for (int i = 0; i < answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

bool PKB::isUses(int stmt, std::string var) {
    int varIndex = varTable.getVarIndex(var);
    return usesTable.isUses(stmt, varIndex);
}

std::vector<int> PKB::getStmtWithType(int nodeType) {
    return stmtNodeTable.getStmtWithType(nodeType);
}

int PKB::getNumStmts() {
    return stmtNodeTable.getSize() - 1;
}