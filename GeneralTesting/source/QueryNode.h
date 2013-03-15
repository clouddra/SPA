#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#ifndef NODE_HEAD
#define NODE_HEAD
#include "Node.h"
#endif

class QueryNode {

private:
	std::string name;
	std::string value;
	std::vector<int> children; // Indexes of children
	int parent; // Parent's index
//	Node pattern_root;
//	QueryNode parent;

public:
	QueryNode(std::string, std::string);
	std::string getName();
	std::string getValue();
	std::vector<int> getChildren();
	void addChild(int);
    void keep1Child();
	void setParent(int);
};
