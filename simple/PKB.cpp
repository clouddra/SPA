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
    if (newStmtFlag)
        stmtNodeTable.insertStmtNode(stmtNum, newNode, nodeType);

    return newNode;
}