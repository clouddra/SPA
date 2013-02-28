#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

PKB::PKB() {
    modifiesTable = ModifiesTable();
    varTable = VarTable();
	procTable = ProcTable();
    usesTable = UsesTable();
	parentTable = ParentTable();
	callsTable = CallsTable();
    ast = AST();
    stmtNodeTable = StmtNodeTable();
}

int PKB::insertNode(int nodeType, std::string value, int parent) {
    int indexValue = -1;
    bool newStmtFlag = false;
    bool hasStmtNum = false;

    switch (nodeType) {
    case Node::assignNode:
    case Node::whileNode:
    case Node::ifNode:
        newStmtFlag = true;
        hasStmtNum = true;
        break;

    case Node::callNode:
        newStmtFlag = true;
        hasStmtNum = true;
        indexValue = procTable.insertProc(value);
        break;

    case Node::varNode:
        indexValue = varTable.insertVar(value);
        hasStmtNum = true;
        break;

    case Node::constNode:
        indexValue = atoi(value.c_str());
        constantList.insert(indexValue);
        hasStmtNum = true;
        break;

    case Node::procedureNode:
        indexValue = procTable.insertProc(value);
        break;

    case Node::divideNode:
    case Node::minusNode:
    case Node::plusNode:
    case Node::timesNode:
        hasStmtNum = true;
    }

    int stmtNum = -1;
    if (hasStmtNum) {
        if (newStmtFlag)
            stmtNum = stmtNodeTable.getSize();
        else
            stmtNum = stmtNodeTable.getSize() - 1;
    }
    int newNode = ast.insertNode(nodeType, indexValue, stmtNum, parent);
    if (newStmtFlag) {
        stmtNodeTable.insertStmtNode(stmtNum, newNode, nodeType);
    }

    return newNode;
}

ModifiesTable* PKB::getModifiesTable() {
    return &modifiesTable;
}

ParentTable* PKB::getParentTable() {
    return &parentTable;
}

FollowsTable* PKB::getFollowsTable() {
    return &followsTable;
}

UsesTable* PKB::getUsesTable() {
    return &usesTable;
}

CallsTable* PKB::getCallsTable() {
	return &callsTable;
}

AST PKB::getAST() {
	return ast;
}

StmtNodeTable* PKB::getStmtNodeTable() {
	return &stmtNodeTable;
}

std::vector<std::string> PKB::getVarTable()  {
    return varTable.getTable();
}

ProcTable* PKB::getProcTable() {
	return &procTable;
}

void PKB::postParseCleanup() {
    // Set the first line and last line of each procedure
    std::vector<Node> tree = ast.getTree();
    std::vector<int> procedures = tree[0].getChildren();
    
    for (int i = 0; i < (int)procedures.size(); i++) {
        Node currProc = tree[procedures[i]];
        Node temp = tree[currProc.getChildren()[0]];
        temp = tree[temp.getChildren()[0]];
        int firstStmt = temp.getStmtNum();
        int lastStmt;
        if ((i+1) < (int)procedures.size()) {
            Node temp2 = tree[procedures[i+1]];
            temp2 = tree[temp2.getChildren()[0]];
            temp2 = tree[temp2.getChildren()[0]];
            lastStmt = temp2.getStmtNum() - 1;
        }
        else {
            lastStmt = getNumStmts();
        }
        procTable.updateProc(currProc.getValue(), firstStmt, lastStmt);
    }
}

std::vector<int> PKB::getParent(int stmt) {
    std::vector<int> ans;
    int temp = parentTable.getParent(stmt);
    if (temp == -1) 
        return ans;
    ans.push_back(temp); 
    return ans;
}

std::vector<int> PKB::getParent() {
    return parentTable.getParent(); 
}

std::vector<int> PKB::getParentT(int stmt) {
    std::vector<int> ans;
    int curr = stmt;
    while (curr != -1) {
        curr = parentTable.getParent(curr);
        if (curr != -1)
            ans.push_back(curr);
    }
    return ans;
}

std::vector<int> PKB::getChild(int stmt) {
    return parentTable.getChild(stmt);
}

std::vector<int> PKB::getChild() {
    return parentTable.getChild();
}

std::vector<int> PKB::getChildT(int stmt) {
    int curr = 0;
    std::vector<int> ans = getChild(stmt);
    
    while (curr < (int)ans.size()) {
        std::vector<int> temp = getChild(ans[curr]);
        for (int i = 0; i < (int)temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

bool PKB::isParent(int parent, int child) {
    return parentTable.isParent(parent, child);
}

std::vector<int> PKB::getFollows(int stmt) {
    return followsTable.getFollows(stmt);
}

std::vector<int> PKB::getFollows() {
    return followsTable.getFollows();
}

std::vector<int> PKB::getFollowsT(int stmt) {
    int curr = 0;
    std::vector<int> ans = getFollows(stmt);
    
    while (curr < (int)ans.size()) {
        std::vector<int> temp = getFollows(ans[curr]);
        for (int i = 0; i < (int)temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

std::vector<int> PKB::getFollowedBy(int stmt) {
    return followsTable.getFollowedBy(stmt);
}

std::vector<int> PKB::getFollowedBy() {
    return followsTable.getFollowedBy();
}

std::vector<int> PKB::getFollowedByT(int stmt) {
    int curr = 0;
    std::vector<int> ans = getFollowedBy(stmt);
    
    while (curr < (int)ans.size()) {
        std::vector<int> temp = getFollowedBy(ans[curr]);
        for (int i = 0; i < (int)temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

bool PKB::isFollow(int first, int second) {
    return followsTable.isFollows(first, second);
}

std::vector<std::string> PKB::getCalls(int stmt) {
    std::vector<int> temp = callsTable.getCalls(stmt);
    std::vector<std::string> ret;

    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcName(temp[i]));
    }
    return ret;
}

std::vector<std::string> PKB::getCalls() {
    std::vector<int> temp = callsTable.getCalls();
    std::vector<std::string> ret;

    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcName(temp[i]));
    }
    return ret;
}

std::vector<std::string> PKB::getCallsT(int stmt) {
    int curr = 0;
    std::vector<std::string> ans = getCalls(stmt);
    std::vector<int> intTemp = callsTable.getCalls(stmt);
    
    while (curr < (int)ans.size()) {
        std::vector<std::string> temp = getCalls(intTemp[curr]);
        for (int i = 0; i < (int)temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

std::vector<std::string> PKB::getCalledBy(int stmt) {
    std::vector<int> temp = callsTable.getCalledBy(stmt);
    std::vector<std::string> ret;

    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcName(temp[i]));
    }
    return ret;
}

std::vector<std::string> PKB::getCalledBy() {
    std::vector<int> temp = callsTable.getCalledBy();
    std::vector<std::string> ret;

    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcName(temp[i]));
    }
    return ret;
}

std::vector<std::string> PKB::getCalledByT(int stmt) {
    int curr = 0;
    std::vector<std::string> ans = getCalledBy(stmt);
    std::vector<int> intTemp = callsTable.getCalledBy(stmt);
    
    while (curr < (int)ans.size()) {
        std::vector<std::string> temp = getCalledBy(intTemp[curr]);
        for (int i = 0; i < (int)temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

bool PKB::isCalls(int first, int second) {
    return callsTable.isCalls(first, second);
}

std::vector<int> PKB::getModifiesVar(std::string var) {
    int varIndex = varTable.getVarIndex(var);
    return modifiesTable.getModifiesVar(varIndex);
}

std::vector<int> PKB::getModifiesVar() {
    return modifiesTable.getModifiesVar();
}

std::vector<std::string> PKB::getModifiedBy(int stmt) {
    std::vector<std::string> toReturn;
    std::vector<int> answer = modifiesTable.getModifiedBy(stmt);
    for (int i = 0; i < (int)answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

std::vector<std::string> PKB::getModifiedBy() {
    std::vector<std::string> toReturn;
    std::vector<int> answer = modifiesTable.getModifiedBy();
    for (int i = 0; i < (int)answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

bool PKB::isModifies(int stmt, std::string var) {
    int varIndex = varTable.getVarIndex(var);
    return modifiesTable.isModifies(stmt, varIndex);
}

std::vector<int> PKB::getUsesVar(std::string var) {
    int varIndex = varTable.getVarIndex(var);
    return usesTable.getUsesVar(varIndex);
}

std::vector<int> PKB::getUsesVar() {
    return usesTable.getUsesVar();
}

std::vector<std::string> PKB::getUsedBy(int stmt) {
    std::vector<std::string> toReturn;
    std::vector<int> answer = usesTable.getUsedBy(stmt);
    for (int i = 0; i < (int)answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

std::vector<std::string> PKB::getUsedBy() {
    std::vector<std::string> toReturn;
    std::vector<int> answer = usesTable.getUsedBy();
    for (int i = 0; i < (int)answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

bool PKB::isUses(int stmt, std::string var) {
    int varIndex = varTable.getVarIndex(var);
    return usesTable.isUses(stmt, varIndex);
}

std::vector<int> PKB::getStmtWithType(int nodeType) {
    return stmtNodeTable.getStmtWithType(nodeType);
}

int PKB::getNumStmts() {
    return stmtNodeTable.getSize() - 1;
}

// The below function is used for pattern while, if and assign _
// varName here refers to the variable name in simple source
std::vector<int> PKB::matchIfWhilePattern(int nodeType, std::string varName) {
	std::vector<int> toReturn;

	if (nodeType == Node::whileNode)
	{
		std::vector<int> whileStmt = getStmtWithType(Node::whileNode);
		// Loop through all while loops, get only those with "while varName"
		for (int i=0; i<(int)whileStmt.size(); i++)
		{
			Node whileAstNode = ast.getNode(stmtNodeTable.getNode(whileStmt[i]));
			std::vector<int> children = whileAstNode.getChildren();
			Node varAstNode = ast.getNode(children[0]); // First AST child of "while" should be variable
			if (varAstNode.getValue() == varTable.getVarIndex(varName))
				toReturn.push_back(whileStmt[i]);
		}
	}
	else if (nodeType == Node::ifNode)
	{
		std::vector<int> ifStmt = getStmtWithType(Node::ifNode);
		// Loop through all if statements, get only those with "if varName"
		for (int i=0; i<(int)ifStmt.size(); i++)
		{
			Node ifAstNode = ast.getNode(stmtNodeTable.getNode(ifStmt[i]));
			std::vector<int> children = ifAstNode.getChildren();
			Node varAstNode = ast.getNode(children[0]);
			if (varAstNode.getValue() == varTable.getVarIndex(varName))
				toReturn.push_back(ifStmt[i]);
		}
	}
	else
	{
		 // nodeType is invalid, should never happen!
		std::cout << "I am in PKB::matchPattern" << std::endl;
	}
	return toReturn;
}

// The below function is used to handle all assign pattern
// varName here refers to the variable name in simple source
// patternRoot should indicate the root of sub-expression, which is "+" for "x+y"
std::vector<int> PKB::matchAssignPattern(std::string varName, std::vector<QueryNode> queryTree, int patternRoot, bool hasUnderscore)
{
	std::vector<int> toReturn;

	std::vector<int> assignStmt = getStmtWithType(Node::assignNode);

	// Loop through all assignment statements
	for (int i=0; i<(int)assignStmt.size(); i++)
	{
		// Check if the assignStmt contains "varName = ..."
		if (isModifies(assignStmt[i], varName))
		{
			int assignNodeIndex = stmtNodeTable.getNode(assignStmt[i]);
			// Right child of assignNode should be the expression root (in AST)
			int exprNodeIndex = ast.getNode(assignNodeIndex).getChildren()[1];
			
			if (hasUnderscore)
			{
                if (patternRoot == -1) // Means _
                    toReturn.push_back(assignStmt[i]);
				else if (subtreeCompare(exprNodeIndex, patternRoot, queryTree)) // Means something like _"x+y"_
					toReturn.push_back(assignStmt[i]);
			}
			else // Means "x+y"
			{
				if (treeCompare(exprNodeIndex, patternRoot, queryTree))
					toReturn.push_back(assignStmt[i]);
			}
		}
	}

	return toReturn;
}

// Does inorder traversal to compare query expression tree against AST expression tree
// Requires exact match. query of "x+y" for expression "a+x+y" will be false
bool PKB::treeCompare(int astNodeIndex, int qNodeIndex, std::vector<QueryNode> queryTree)
{
	bool left = true, right = true;
	Node qNode = qNodeToNode(queryTree[qNodeIndex]); // qNode is now of Node type
	Node aNode = ast.getNode(astNodeIndex); // aNode is now of Node type
	if (aNode.equals(qNode))
	{
		std::vector<int> qChild = queryTree[qNodeIndex].getChildren();
		std::vector<int> aChild = aNode.getChildren();
		// An expression either has no children, or 2 children
		if (qChild.size() > 0 && aChild.size() > 0)
		{
			// Check left child
			left = treeCompare(aChild[0], qChild[0], queryTree);
			// Check right child
			right = treeCompare(aChild[1], qChild[1], queryTree);
			return (left && right);
		}
		else if (qChild.size() == 0 && aChild.size() == 0)
		{
			return true;
		}
		else // Either qNode or aNode has children but the other doesn't
			return false;
	}
	else // aNode and qNode are different
		return false;
}

// To handle pattern assign with _"x+y"_
bool PKB::subtreeCompare(int astNodeIndex, int qNodeIndex, std::vector<QueryNode> queryTree)
{
	bool curr = false, left = false, right = false;
	// Compare for subtree at current AST node
	curr = treeCompare(astNodeIndex, qNodeIndex, queryTree);

	Node aNode = ast.getNode(astNodeIndex); // aNode is now of Node type
	std::vector<int> aChild = aNode.getChildren();
	// An expression either has no children, or 2 children
	if (aChild.size() > 0)
	{
		left = subtreeCompare(aChild[0], qNodeIndex, queryTree);
		right = subtreeCompare(aChild[1], qNodeIndex, queryTree);
	}
	return (curr || left || right);
}


// This is used for pattern assign, to make qNode to Node for var, plus, minus, times, divide
// However it does not incorporate the children of qNode
Node PKB::qNodeToNode(QueryNode qNode)
{
	int nodeType;
	int value = -1; // only varNode will have index value
	if (qNode.getName() == "+")
		nodeType = Node::plusNode;
	else if (qNode.getName() == "-")
		nodeType = Node::minusNode;
	else if (qNode.getName() == "*")
		nodeType = Node::timesNode;
	else if (qNode.getName() == "/")
		nodeType = Node::divideNode;
    else if (qNode.getValue().compare("variable") == 0)
	{
		nodeType = Node::varNode;
		value = varTable.getVarIndex(qNode.getName());
	}
    else
    {
        nodeType = Node::constNode;
        value = atoi(qNode.getName().c_str());
    }
	return Node(nodeType, value, -1);
}

std::set<int> PKB::getConstants() {
    return constantList;
}