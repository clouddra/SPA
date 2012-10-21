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
}

int PKB::insertNode(int nodeType, std::string value, int stmtNum, int parent) {
    int indexValue;

    switch (nodeType) {
    
    case Node::varNode:
        indexValue = varTable.insertVar(value);
        break;

    case Node::constNode:
        indexValue = atoi(value.c_str());
        break;

    case Node::procedureNode:
        indexValue = procTable.insertProc(value, 0, 0);
        break;

    default:
        indexValue = -1;
    }

    int newNode = ast.insertNode(nodeType, indexValue, stmtNum, parent);
    return newNode;
}