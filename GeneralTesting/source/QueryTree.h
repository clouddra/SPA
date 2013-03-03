#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#ifndef QUERYNODE_HEAD
#define QUERYNODE_HEAD
#include "QueryNode.h"
#endif

class QueryTree {

private:
	std::vector<QueryNode> tree;

public:
	QueryTree();
	int insertNode(std::string, std::string, int);
	std::vector<QueryNode> getTree();
    void reorgTree();
};