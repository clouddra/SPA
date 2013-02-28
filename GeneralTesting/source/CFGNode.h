#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class CFGNode{

private:
	/**
	 * Indicates the earliest that can be reached from the current node
	 */
	/**
	 * Indicates the first statement of the node
	 */
	int start;
	/**
	 * Indicates the last statement of the node
	 */
	int end;


	std::vector<int> next;
	std::vector<int> prev;

	public:
	CFGNode::CFGNode(int startStmt, int endStmt, std::vector<int> prevNodes);
	CFGNode::CFGNode();
	void addStmt(int stmt);
	int getStart();
	int getEnd();
	std::vector<int> getNext();
	std::vector<int> getPrev();
	void addNext(int nextNode);
	void addPrev(int prevNode);
};

/*
GNODE createGNode (TNODE astNode, INDEX nodeType, STMT stmtNum, GNODE prevNode)
Description:
    Creates a CFG node with the given nodeType. eg. “assign? “call?
    stmtNum corresponds to the statement number represented by this node.
    Creates a reference to corresponding astNode.
    Add this GNODE object to prevNode.nextArray[].
    Add prevNode object to the new GNODE.prevArray[].

VOID addNext (GNODE prev, GNODE next)
Description:
    Add next to prev.nextArray[].
   
VOID addPrev (GNODE prev, GNODE next)
Description:
    Add prev to next.prevArray[].

STMT[] getNextStmt (STMT stmtNum)
Description:
    Returns the nextArray[] of the Gnode corresponding to stmtNum.

STMT[] getPrevStmt (STMT stmtNum)
Description:
    Returns the prevArray[] of the Gnode corresponding to stmtNum.

GNODE getGNode (STMT stmtNum)
Description:
    Returns the GNODE object that corresponds to the given stmtNum.

TNODE getTNode (STMT stmtNum)
Description:
    Returns the TNODE object that corresponds to the given stmtNum.


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
	static const enum {programNode, procedureNode, stmtLstNode, assignNode, whileNode, ifNode, callNode, varNode, constNode, plusNode, minusNode, timesNode, divideNode}; 
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

*/