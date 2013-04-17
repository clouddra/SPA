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
    
    /**
	 * Insert a node into the tree with given name, value and parent
	 */
	int insertNode(std::string, std::string, int);

    /**
	 * Getter function for tree
	 */
	std::vector<QueryNode> getTree();

    /**
	 * Reorganize the tree to such that such that notation (remove all ands)
	 */
    void reorgTree();
};