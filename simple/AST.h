#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#ifndef NODE_HEAD
#define NODE_HEAD
#include "Node.h"
#endif

class AST {

private:
    std::vector<Node> tree;

public:
    AST();
    int insertNode(int nodeType, int value, int stmtNum, int parent);
    std::vector<Node> getTree();
};