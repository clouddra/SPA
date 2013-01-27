#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

PKB::PKB() {
    modifiesTable = ModifiesTable();
    varTable = VarTable();
    usesTable = UsesTable();
    procTable = ProcTable();
	parentTable = ParentTable();
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
    case Node::callNode:
        newStmtFlag = true;
        hasStmtNum = true;
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
        indexValue = procTable.insertProc(value, stmtNodeTable.getSize());   // This line assumes there are no empty procedure
        if (indexValue != 0)
            procTable.setProcLastln(indexValue-1, stmtNodeTable.getSize()-1);
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
AST PKB::getAST()
{
	return ast;
}
StmtNodeTable* PKB::getStmtNodeTable()
{
	return &stmtNodeTable;
}
std::vector<std::string> PKB::getVarTable() {
    return varTable.getTable();
}

void PKB::postParseCleanup() {
    procTable.setProcLastln(procTable.getSize()-1, stmtNodeTable.getSize()-1);  //Set the lastLine of the last procedure
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

std::vector<int> PKB::matchPattern(int nodeType, std::string varName, std::string expr) {
	std::vector<int> toReturn;

	if (nodeType == Node::assignNode)
	{
		std::vector<int> assignStmt = getStmtWithType(Node::assignNode); // Get all assignment statements
		if (expr == "_")
		{
			for (int i=0; i<(int)assignStmt.size(); i++)
			{
				// Check if the assignStmt contains "varName = ..."
				if (isModifies(assignStmt[i], varName))
					toReturn.push_back(assignStmt[i]);
			}
		}
		else // This means expr = _"var"_
		{
			std::string var = expr.substr(2, expr.length() - 4); // extract the var by removing _" and "_
			for (int i=0; i<(int)assignStmt.size(); i++)
			{
				// Check if the assignStmt contains "varName = ...var..."
				if (isModifies(assignStmt[i], varName) && isUses(assignStmt[i], var))
					toReturn.push_back(assignStmt[i]);
			}
		}
	}
	else if (nodeType == Node::whileNode)
	{
		// Evaluate while pattern
	}
	else if (nodeType == Node::ifNode)
	{
		// Evaluate if pattern
	}
	else
	{
		 // nodeType is invalid, should never happen!
		std::cout << "I am in PKB::matchPattern" << std::endl;
	}
	return toReturn;
}

std::set<int> PKB::getConstants() {
    return constantList;
}