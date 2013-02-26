#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#include "CFGNode.h"

class CFG{

private:
	/**
	 * Indicates the earliest that can be reached from the current node
	 */
	std::vector<CFGNode> cfg;

	public:

	std::vector<int> getNext(int stmt1, int nodeIndex);
	std::vector<int> getPrev(int stmt2, int nodeIndex);
	CFGNode getCFGNode(int nodeIndex);
	std::vector<CFGNode> getCFG();
	int insertCFGNode(int start, int end, std::vector<int> prev);
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