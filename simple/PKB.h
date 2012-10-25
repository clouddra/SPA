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

class PKB {

private:
    AST ast;
    ModifiesTable modifiesTable;
    VarTable varTable;
    UsesTable usesTable;
    ProcTable procTable;
    StmtNodeTable stmtNodeTable;

public:
    PKB();
    int insertNode(int nodeType, std::string value, int stmtNum, int parent);
};