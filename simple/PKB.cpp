#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

PKB::PKB() {
    modifiesTable = ModifiesTable();
    varTable = VarTable();
    usesTable = UsesTable();
    procTable = ProcTable();
    ast = AST();
    stmtNodeTable = StmtNodeTable();
}

int PKB::insertNode(int nodeType, std::string value, int stmtNum, int parent) {
    int indexValue = -1;
    bool newStmtFlag = false;

    switch (nodeType) {
    // Please add if and call when implemented
    case Node::assignNode:
    case Node::whileNode:
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

    int newNode = ast.insertNode(nodeType, indexValue, stmtNum, parent);
    if (newStmtFlag) {
        stmtNodeTable.insertStmtNode(stmtNum, newNode, nodeType);
    }

    return newNode;
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