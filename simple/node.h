#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class Node {

private:
	int nodeType; // enum
	int value;    // index of some table or null depending on nodeType
    int stmtNum;
	// Links are index of AST
	int parent;
	int leftSibling;
	int rightSibling;
	std::vector<int> children;

public:
	static const enum {programNode, procedureNode, stmtLstNode, assignNode, whileNode, varNode, plusNode, constNode}; // not sure how to deal with constants
	Node(int type, int v, int s);
	int getNodeType();
	int getValue();
	int getParent();
	int getLeft();
	int getRight();
    int getStmtNum();
	std::vector<int> getChildren();
	void setParent(int p);
	void setLeft(int l);
	void setRight(int r);
	void addChild(int c);
    bool equals(Node other);
};