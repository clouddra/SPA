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
	containsTable = ContainsTable();
	siblingTable = SiblingTable();
    ast = AST();
    stmtNodeTable = StmtNodeTable();
	cfg = CFG();
	cfgBip = CFGBip();
}

int PKB::insertNode(int nodeType, std::string value, int parent) {
    int indexValue = -1;
    bool newStmtFlag = false;
    bool hasStmtNum = false;
    bool insertNodeTable = false;

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
            insertNodeTable = true;
            break;

        case Node::procedureNode:
            indexValue = procTable.insertProc(value);
            insertNodeTable = true;
            break;

        case Node::constNode:
            indexValue = atoi(value.c_str());
            constantList.insert(indexValue);
            hasStmtNum = true;
            insertNodeTable = true;
            break;

        case Node::divideNode:
        case Node::minusNode:
        case Node::plusNode:
        case Node::timesNode:
            hasStmtNum = true;
            insertNodeTable = true;
            break;

        case Node::stmtLstNode:
            insertNodeTable = true;
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
    if (insertNodeTable) {
        nodeTable.insert(newNode, nodeType);
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

ContainsTable* PKB::getContainsTable() {
	return &containsTable;
}

SiblingTable* PKB::getSiblingTable() {
	return &siblingTable;
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

std::vector<int> PKB::getNodeIndexes(int type) {
    switch (type) {
        case Node::minusNode:
            return nodeTable.getMinus();

        case Node::plusNode:
            return nodeTable.getPlus();

        case Node::timesNode:
            return nodeTable.getTimes();

        case Node::stmtLstNode:
            return nodeTable.getStmtLst();
    }
    return std::vector<int>();
}

std::vector<std::string> PKB::convertStmtLst(std::vector<int> nodeIndexes) {
    std::vector<int> temp;
    std::vector<std::string> toRet;
    std::vector<Node> tree = ast.getTree();

    for (int i = 0; i < (int)nodeIndexes.size(); i++) {
        temp = tree[nodeIndexes[i]].getChildren();
        int firstStmt = tree[temp[0]].getStmtNum();
        toRet.push_back(std::to_string((long long)firstStmt));
    }
    return toRet;
}

std::vector<int> PKB::convertToNodeIndex(std::string input, int type) {
    std::vector<int> nodeInput, temp;
    std::vector<Node> tree = ast.getTree();
    std::istringstream convert(input);
    int inputNum = -1;
    bool isNum = false;
    if (convert >> inputNum)
        isNum = true;

    switch (type) {
        case Node::assignNode:
        case Node::callNode:
        case Node::ifNode:
        case Node::whileNode:
        case Node::stmtSpecialValue:
            if (!isNum)
                return temp;
            nodeInput.push_back(stmtNodeTable.getAST(inputNum));
            break;

        case Node::constNode:
            temp = nodeTable.getConstants();
            if (!isNum)
                return temp;
            for (int i = 0; i < (int)temp.size(); i++) {
                if (tree[temp[i]].getValue() == inputNum)
                    nodeInput.push_back(temp[i]);
            }
            break;

        case Node::minusNode:
        case Node::plusNode:
        case Node::timesNode:
        case Node::stmtLstNode:
            if (!isNum)
                return temp;
            nodeInput.push_back(inputNum);
            break;

        case Node::procedureNode:
            temp = nodeTable.getProcs();
            inputNum = procTable.getProcIndex(input);
            if (inputNum == -1)
                return temp;
            for (int i = 0; i < (int)temp.size(); i++) {
                if (tree[temp[i]].getValue() == inputNum)
                    nodeInput.push_back(temp[i]);
            }
            break;

        case Node::varNode:
            temp = nodeTable.getVariables();
            inputNum = varTable.getVarIndex(input);
            if (inputNum == -1)
                return temp;
            for (int i = 0; i < (int)temp.size(); i++) {
                if (tree[temp[i]].getValue() == inputNum)
                    nodeInput.push_back(temp[i]);
            }
            break;
    }
    return nodeInput;
}

std::vector<std::string> PKB::convertToStorageType(std::vector<int> result, int type) {
    std::vector<Node> tree = ast.getTree();
    std::set<std::string> tempSet;
    std::vector<std::string> toRet;

    switch (type) {
        case Node::assignNode:
        case Node::callNode:
        case Node::ifNode:
        case Node::whileNode:
            for (int i = 0; i < (int)result.size(); i++) {
                if (tree[result[i]].getNodeType() == type) {
                    std::string temp = std::to_string((long long) tree[result[i]].getStmtNum());
                    tempSet.insert(temp);
                }
            }
            break;

        case Node::stmtSpecialValue:
            for (int i = 0; i < (int)result.size(); i++) {
                if (tree[result[i]].getNodeType() == Node::assignNode || tree[result[i]].getNodeType() == Node::whileNode || tree[result[i]].getNodeType() == Node::ifNode || tree[result[i]].getNodeType() == Node::callNode) {
                    std::string temp = std::to_string((long long) tree[result[i]].getStmtNum());
                    tempSet.insert(temp);
                }
            }
            break;

        case Node::constNode:
            for (int i = 0; i < (int)result.size(); i++) {
                if (tree[result[i]].getNodeType() == type) {
                    std::string temp = std::to_string((long long) tree[result[i]].getValue());
                    tempSet.insert(temp);
                }
            }
            break;

        case Node::minusNode:
        case Node::plusNode:
        case Node::timesNode:
        case Node::stmtLstNode:
            for (int i = 0; i < (int)result.size(); i++) {
                if (tree[result[i]].getNodeType() == type) {
                    std::string temp = std::to_string((long long) result[i]);
                    toRet.push_back(temp);
                }
            }
            return toRet;
            break;

        case Node::procedureNode:
            for (int i = 0; i < (int)result.size(); i++) {
                if (tree[result[i]].getNodeType() == type) {
                    int temp = tree[result[i]].getValue();
                    tempSet.insert(procTable.getProcName(temp));
                }
            }
            break;
            
        case Node::varNode:
            for (int i = 0; i < (int)result.size(); i++) {
                if (tree[result[i]].getNodeType() == type) {
                    int temp = tree[result[i]].getValue();
                    tempSet.insert(varTable.getVarName(temp));
                }
            }
            break;
    }
    toRet = std::vector<std::string> (tempSet.begin(), tempSet.end());
    return toRet;
}

std::vector<std::string> PKB::getContainer(std::string input, int inputType, int outputType) {
    std::vector<int> nodeInput, temp, result;

    nodeInput = convertToNodeIndex(input, inputType);
    for (int i = 0; i < (int)nodeInput.size(); i++) {
        temp = containsTable.getContainer(nodeInput[i]);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    std::vector<std::string> toRet = convertToStorageType(result, outputType);
    return toRet;
}

std::vector<std::string> PKB::getContainerT(std::string input, int inputType, int outputType) {
    std::vector<int> nodeInput, temp, result;
    std::unordered_set<int> tempSet;

    nodeInput = convertToNodeIndex(input, inputType);
    std::unordered_set<int> inputSet = std::unordered_set<int> (nodeInput.begin(), nodeInput.end());
    for (int i = 0; i < (int)nodeInput.size(); i++) {
        temp = containsTable.getContainer(nodeInput[i]);
        for (int j = 0; j < (int)temp.size(); j++) {
            tempSet.insert(temp[j]);
            if (inputSet.insert(temp[j]).second) {
                nodeInput.push_back(temp[j]);
            }
        }
    }
    result = std::vector<int> (tempSet.begin(), tempSet.end());
    std::vector<std::string> toRet = convertToStorageType(result, outputType);
    return toRet;
}

std::vector<std::string> PKB::getContainedIn(std::string input, int inputType, int outputType) {
    std::vector<int> nodeInput, temp, result;

    nodeInput = convertToNodeIndex(input, inputType);
    for (int i = 0; i < (int)nodeInput.size(); i++) {
        temp = containsTable.getContainedIn(nodeInput[i]);
        result.insert(result.end(), temp.begin(), temp.end());
    }
    std::vector<std::string> toRet = convertToStorageType(result, outputType);
    return toRet;
}

std::vector<std::string> PKB::getContainedInT(std::string input, int inputType, int outputType) {
    std::vector<int> nodeInput, temp, result;
    std::unordered_set<int> tempSet;

    nodeInput = convertToNodeIndex(input, inputType);
    std::unordered_set<int> inputSet = std::unordered_set<int> (nodeInput.begin(), nodeInput.end());
    for (int i = 0; i < (int)nodeInput.size(); i++) {
        temp = containsTable.getContainedIn(nodeInput[i]);
        for (int j = 0; j < (int)temp.size(); j++) {
            tempSet.insert(temp[j]);
            if (inputSet.insert(temp[j]).second) {
                nodeInput.push_back(temp[j]);
            }
        }
    }
    result = std::vector<int> (tempSet.begin(), tempSet.end());
    std::vector<std::string> toRet = convertToStorageType(result, outputType);
    return toRet;
}

bool PKB::isContains(std::string input1, int type1, std::string input2, int type2) {
    std::vector<int> nodeInput1, nodeInput2;
    nodeInput1 = convertToNodeIndex(input1, type1);
    nodeInput2 = convertToNodeIndex(input2, type2);

    for (int i = 0; i < (int)nodeInput1.size(); i++) {
        for (int j = 0; j < (int)nodeInput2.size(); j++) {
            if (containsTable.isContains(nodeInput1[i], nodeInput2[j])) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::string> PKB::getSibling(std::string input, int inputType, int outputType) {
    std::vector<int> nodeInput, temp;
    std::unordered_set<int> result;

    nodeInput = convertToNodeIndex(input, inputType);
    for (int i = 0; i < (int)nodeInput.size(); i++) {
        temp = siblingTable.getSiblings(nodeInput[i]);
        result.insert(temp.begin(), temp.end());
    }
    temp = std::vector<int>(result.begin(), result.end());
    std::vector<std::string> toRet = convertToStorageType(temp, outputType);
    return toRet;
}

bool PKB::isSibling(std::string input1, int type1, std::string input2, int type2) {
    std::vector<int> nodeInput1, nodeInput2;
    nodeInput1 = convertToNodeIndex(input1, type1);
    nodeInput2 = convertToNodeIndex(input2, type2);

    for (int i = 0; i < (int)nodeInput1.size(); i++) {
        for (int j = 0; j < (int)nodeInput2.size(); j++) {
            if (siblingTable.isSibling(nodeInput1[i], nodeInput2[j])) {
                return true;
            }
        }
    }
    return false;
}

std::vector<int> PKB::getParent(int stmt) {
	return parentTable.getParent(stmt);
}

std::vector<int> PKB::getParent() {
    return parentTable.getParent(); 
}

std::vector<int> PKB::getParentT(int stmt) {
	int curr = 0;
    std::vector<int> ans = getParent(stmt);
	while (curr < (int)ans.size()) {
        std::vector<int> temp = getParent(ans[curr]);
        for (int i = 0; i < (int)temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
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

std::vector<std::string> PKB::getCalls(std::string proc) {
    std::vector<std::string> ret;
    int p = procTable.getProcIndex(proc);
    if (p == -1)
        return ret;

    std::vector<int> temp = callsTable.getCalls(p);
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

std::vector<std::string> PKB::getCallsT(std::string proc) {
    std::vector<std::string> ans;
    int p = procTable.getProcIndex(proc);
    if (p == -1)
        return ans;

    int curr = 0;
    ans = getCalls(proc);  
    while (curr < (int)ans.size()) {
        std::vector<std::string> temp = getCalls(ans[curr]);
        for (int i = 0; i < (int)temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

std::vector<std::string> PKB::getCalledBy(std::string proc) {
    std::vector<std::string> ret;
    int p = procTable.getProcIndex(proc);
    if (p == -1)
        return ret;

    std::vector<int> temp = callsTable.getCalledBy(p);
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

std::vector<std::string> PKB::getCalledByT(std::string proc) {
    std::vector<std::string> ans;
    int p = procTable.getProcIndex(proc);
    if (p == -1)
        return ans;

    int curr = 0;
    ans = getCalledBy(proc); 
    while (curr < (int)ans.size()) {
        std::vector<std::string> temp = getCalledBy(ans[curr]);
        for (int i = 0; i < (int)temp.size(); i++) {
            ans.push_back(temp[i]);
        }
        curr++;
    }
    return ans;
}

bool PKB::isCalls(std::string proc1, std::string proc2) {
    int p1 = procTable.getProcIndex(proc1);
    int p2 = procTable.getProcIndex(proc2);
    if (p1 == -1 || p2 == -1)
        return false;
    return callsTable.isCalls(p1, p2);
}

std::vector<int> PKB::getModifiesVar(std::string var) {
    int varIndex = varTable.getVarIndex(var);
    if (varIndex == -1)
        return std::vector<int> ();
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
    if (varIndex == -1)
        return false;
    return modifiesTable.isModifies(stmt, varIndex);
}

std::vector<std::string> PKB::getModifiesVarP(std::string var) {
    std::vector<std::string> ret;
    int varIndex = varTable.getVarIndex(var);
    if (varIndex == -1)
        return ret;

    std::vector<int> temp = modifiesTable.getModifiesVarProc(varIndex);
    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcName(temp[i]));
    }
    return ret;
}

std::vector<std::string> PKB::getModifiesVarP() {
    std::vector<std::string> ret;
    std::vector<int> temp = modifiesTable.getModifiesVarProc();
    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcName(temp[i]));
    }
    return ret;
}

std::vector<std::string> PKB::getModifiedByP(std::string proc) {
    std::vector<std::string> toReturn;
    int procIndex = procTable.getProcIndex(proc);
    if (procIndex == -1)
        return toReturn;
    std::vector<int> answer = modifiesTable.getModifiedByProc(procIndex);
    for (int i = 0; i < (int)answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

std::vector<std::string> PKB::getModifiedByP() {
    std::vector<std::string> toReturn;
    std::vector<int> answer = modifiesTable.getModifiedByProc();
    for (int i = 0; i < (int)answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

bool PKB::isModifiesP(std::string proc, std::string var) {
    int varIndex = varTable.getVarIndex(var);
    if (varIndex == -1)
        return false;
    int procIndex = procTable.getProcIndex(proc);
    if (procIndex == -1)
        return false;
    return modifiesTable.isModifiesProc(procIndex, varIndex);
}

std::vector<std::string> PKB::getUsesVarP(std::string var) {
    std::vector<std::string> ret;
    int varIndex = varTable.getVarIndex(var);
    if (varIndex == -1)
        return ret;
    std::vector<int> temp = usesTable.getUsesVarProc(varIndex);
    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcName(temp[i]));
    }
    return ret;
}

std::vector<std::string> PKB::getUsesVarP() {
    std::vector<std::string> ret;
    std::vector<int> temp = usesTable.getUsesVarProc();
    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcName(temp[i]));
    }
    return ret;
}

std::vector<std::string> PKB::getUsedByP(std::string proc) {
    std::vector<std::string> toReturn;
    int procIndex = procTable.getProcIndex(proc);
    if (procIndex == -1)
        return toReturn;
    std::vector<int> answer = usesTable.getUsedByProc(procIndex);
    for (int i = 0; i < (int)answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

std::vector<std::string> PKB::getUsedByP() {
    std::vector<std::string> toReturn;
    std::vector<int> answer = usesTable.getUsedByProc();
    for (int i = 0; i < (int)answer.size(); i++) {
        toReturn.push_back(varTable.getVarName(answer[i]));
    }
    return toReturn;
}

bool PKB::isUsesP(std::string proc, std::string var) {
    int varIndex = varTable.getVarIndex(var);
    if (varIndex == -1)
        return false;
    int procIndex = procTable.getProcIndex(proc);
    if (procIndex == -1)
        return false;
    return usesTable.isUsesProc(procIndex, varIndex);
}

std::vector<int> PKB::getUsesVar(std::string var) {
    int varIndex = varTable.getVarIndex(var);
    if (varIndex == -1)
        return std::vector<int> ();
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
    if (varIndex == -1)
        return false;
    return usesTable.isUses(stmt, varIndex);
}

std::vector<int> PKB::getNext(int stmt) {
    return cfg.getNext(stmt, stmtNodeTable.getCFG(stmt));
}

std::vector<int> PKB::getNext() {
    std::vector<int> ret;
    for (int i = 0; i < (int)procTable.getSize(); i++) {
        int first = procTable.getProcFirstln(i);
        int last = procTable.getProcLastln(i);

        for (int j = first+1; j <= last; j++) {
            ret.push_back(j);
        }
    }
    return ret;
}

std::vector<int> PKB::getNextT(int stmt) {
    return cfg.getNextT(stmt, stmtNodeTable.getCFG(stmt));
}

std::vector<int> PKB::getPrev(int stmt) {
    return cfg.getPrev(stmt, stmtNodeTable.getCFG(stmt));
}

std::vector<int> PKB::getPrev() {
    std::vector<int> ret;
    for (int i = 0; i < (int)procTable.getSize(); i++) {
        int first = procTable.getProcFirstln(i);
        int last = procTable.getProcLastln(i);

        for (int j = first; j < last; j++) {
            ret.push_back(j);
        }
    }
    return ret;
}

std::vector<int> PKB::getPrevT(int stmt) {
    return cfg.getPrevT(stmt, stmtNodeTable.getCFG(stmt));
}

bool PKB::isNext(int stmt1, int stmt2) {
    return cfg.isNext(stmt1, stmtNodeTable.getCFG(stmt1), stmt2);
}

std::vector<int> PKB::getNextBip(int stmt) {
    return cfg.getNextBip(stmt, stmtNodeTable.getCFG(stmt));
}

std::vector<int> PKB::getNextBip() {
    std::vector<int> ret;
    for (int i = 0; i < (int)procTable.getSize(); i++) {
        int first = procTable.getProcFirstln(i);
        int last = procTable.getProcLastln(i);

        for (int j = first+1; j <= last; j++) {
            ret.push_back(j);
        }
    }
    std::vector<int> temp = callsTable.getCalledBy();
    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcFirstln(temp[i]));
    }
    return ret;
}

std::vector<int> PKB::getNextBipT(int stmt) {
    return cfg.getNextBipT(stmt, stmtNodeTable.getCFG(stmt));
}

std::vector<int> PKB::getPrevBip(int stmt) {
    return cfg.getPrevBip(stmt, stmtNodeTable.getCFG(stmt));
}

std::vector<int> PKB::getPrevBip() {
    std::vector<int> ret;
    for (int i = 0; i < (int)procTable.getSize(); i++) {
        int first = procTable.getProcFirstln(i);
        int last = procTable.getProcLastln(i);

        for (int j = first; j < last; j++) {
            ret.push_back(j);
        }
    }
    std::vector<int> temp = callsTable.getCalls();
    for (int i = 0; i < (int)temp.size(); i++) {
        ret.push_back(procTable.getProcLastln(temp[i]));
    }
    return ret;
}

std::vector<int> PKB::getPrevBipT(int stmt) {
    return cfg.getPrevBipT(stmt, stmtNodeTable.getCFG(stmt));
}

bool PKB::isNextBip(int stmt1, int stmt2) {
    return cfg.isNextBip(stmt1, stmtNodeTable.getCFG(stmt1), stmt2);
}

std::vector<int> PKB::getStmtWithType(int nodeType) {
    return stmtNodeTable.getStmtWithType(nodeType);
}

int PKB::getNumStmts() {
    return stmtNodeTable.getSize() - 1;
}

std::vector<std::string> PKB::getAllProc() {
    return procTable.getAllProc();
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
			Node whileAstNode = ast.getNode(stmtNodeTable.getAST(whileStmt[i]));
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
			Node ifAstNode = ast.getNode(stmtNodeTable.getAST(ifStmt[i]));
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
			int assignNodeIndex = stmtNodeTable.getAST(assignStmt[i]);
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

void PKB::startBuildCfg() {

	int start=0;
	int end=0;
	
	std::vector<std::pair<int,int>> callList;

	for (int i=0; i<(int)ast.getTree().size(); i++)
	{
		Node astNode = ast.getNode(i);
		// Each procedure builds a CFG
		if (astNode.getNodeType() == Node::procedureNode)
		{
			// Insert an empty CFG Node and let buildCFG add in the first statement
			// update proc table to include info about cfg
			procTable.setCFGStart(astNode.getValue(), start);	
			// add dummy at end of proc
			end = cfg.insertCFGNode(-1, -1, buildCfg(astNode.getChildren()[0], -1, callList));							
			procTable.setCFGEnd(astNode.getValue(), end);
			
			// start of next procedure
			start = end+1; 
		}

	}
	
	for (std::vector<std::pair<int,int>>::iterator it = callList.begin() ; it != callList.end(); ++it){

		int bipStart = procTable.getCFGStart(it->second);
		int bipEnd = procTable.getCFGEnd(it->second);
		std::vector<int> afterCall = cfg.getCFG()[it->first].getNext();

		cfg.addBipNext(it->first, bipStart);
		cfg.addBipPrev(bipStart, it->first);

		// set return to all nodes after call
		for (std::vector<int>::iterator j = afterCall.begin() ; j != afterCall.end(); ++j) {
			cfg.addBipPrev(*j, bipEnd);
			cfg.addBipNext(bipEnd, *j);
		}
	}

}

// Takes in the ASTnode of stmtList and the CFGNode of the first child of stmtList
// Returns the ending CFGNodes
std::vector<int> PKB::buildCfg(int stmtListAst, int startCFG, std::vector<std::pair<int,int>>& callList) {
	std::vector<int> toReturn;

	// Ensure the astNode passed in is of stmtLst type
	Node stmtListNode = ast.getNode(stmtListAst);
	if (stmtListNode.getNodeType() != Node::stmtLstNode)
		std::cout << "I am in PKB::buildCFG" << std::endl;
	
	std::vector<int> children = stmtListNode.getChildren();	
	int startStmt = ast.getNode(children[0]).getStmtNum();
	int endStmt = startStmt;
//	int nextCfg = -1;
	int currAst = -1;

	// more than 1 node
	std::vector<int> prevCFG; 
	if (startCFG!=-1)
		prevCFG.push_back(startCFG);

	for (int i=0; i<(int)children.size(); i++)
	{
		currAst = children[i];

		// Add the stmtNum into the empty CFG Node created earlier by someone else


		//cfg.addStmt(currCfg, stmtNum);

		// If the current child has a right sibling (Not the last child)
		if (i != children.size()-1)
		{
			if (ast.getNode(currAst).getNodeType() == Node::assignNode)
			{
				// shift the end. start don't change since its 1 whole block
				endStmt = ast.getNode(currAst).getStmtNum();
			}
			else if (ast.getNode(currAst).getNodeType() == Node::whileNode)
			{
				// insert node sequence before while if they are not control nodes. Else there is no sequence to insert
				if (i>0 && ast.getNode(children[i-1]).getNodeType() == Node::assignNode) {
					int currNode = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
					addCFGtoStmtNodeTable(currNode, startStmt, endStmt);
					prevCFG.clear();
					prevCFG.push_back(currNode);
				}
				// while is always single node
				startStmt = ast.getNode(currAst).getStmtNum();
				endStmt = ast.getNode(currAst).getStmtNum();
				int whileCFG = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
				addCFGtoStmtNodeTable(whileCFG, startStmt, endStmt);

				int whileStmtLst = ast.getNode(currAst).getChildren()[1];
				std::vector<int> endWhile = buildCfg(whileStmtLst, whileCFG, callList);
				
				for (int j=0; j<(int)endWhile.size(); j++)
				{
					cfg.addNext(endWhile[j], whileCFG);
					cfg.addPrev(whileCFG, endWhile[j]);
				}
				// Create and link CFG node for the right sibling

				// start, end = statement following while, while node becomes prev cfg
				// following statement is the start of next node
				startStmt = ast.getNode(children[i+1]).getStmtNum();
				endStmt = ast.getNode(children[i+1]).getStmtNum();
				prevCFG.clear();
				prevCFG.push_back(whileCFG);
			}

			else if (ast.getNode(currAst).getNodeType() == Node::callNode)
			{
				// insert node sequence before while if they are not control nodes. Else there is no sequence to insert
				if (i>0 && ast.getNode(children[i-1]).getNodeType() == Node::assignNode) {
					int currNode = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
					addCFGtoStmtNodeTable(currNode, startStmt, endStmt);
					prevCFG.clear();
					prevCFG.push_back(currNode);
				}

				startStmt = ast.getNode(currAst).getStmtNum();
				endStmt = ast.getNode(currAst).getStmtNum();
				int callCFG = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
				addCFGtoStmtNodeTable(callCFG, startStmt, endStmt);
				
				// record all call statments
				callList.push_back(std::make_pair<int,int>(callCFG, ast.getNode(currAst).getValue()));

				// dummy node for call return
				//callCFG = cfgBip.insertCFGNode(startStmt, endStmt, callCFG);
				
				//bip.push_back(std::pair<int,int>(startStmt, ast.getNode(currAst).getValue()));
				startStmt = ast.getNode(children[i+1]).getStmtNum();
				endStmt = ast.getNode(children[i+1]).getStmtNum();
				prevCFG.clear();
				prevCFG.push_back(callCFG);

			}

			else if (ast.getNode(currAst).getNodeType() == Node::ifNode)
			{

				// insert node sequence before if, save the node index as forkCFG
				endStmt = ast.getNode(currAst).getStmtNum();
				int forkCFG = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
				addCFGtoStmtNodeTable(forkCFG, startStmt, endStmt);

				int thenStmtLst = ast.getNode(currAst).getChildren()[1];
				std::vector<int> endThen = buildCfg(thenStmtLst, forkCFG, callList);

				int elseStmtLst = ast.getNode(currAst).getChildren()[2];
				std::vector<int> endElse = buildCfg(elseStmtLst, forkCFG, callList);

				prevCFG.clear();
				for (int j=0; j<(int)endThen.size(); j++)
				{
					prevCFG.push_back(endThen[j]);
				}
				for (int j=0; j<(int)endElse.size(); j++)
				{
					prevCFG.push_back(endElse[j]);
				}

				// following statement is the start of next node
				startStmt = endStmt = ast.getNode(children[i+1]).getStmtNum();

			}
			else
				std::cout << "I am in PKB::buildCFG (1)" << std::endl;
		}
		else // Last child of stmtList
		{
			if (ast.getNode(currAst).getNodeType() == Node::assignNode)
			{
				endStmt = ast.getNode(currAst).getStmtNum();
				int endCFG = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
				addCFGtoStmtNodeTable(endCFG, startStmt, endStmt);
				toReturn.push_back(endCFG);
			}
			else if (ast.getNode(currAst).getNodeType() == Node::whileNode)
			{

				if (i>0 && ast.getNode(children[i-1]).getNodeType() == Node::assignNode) {
					int currNode = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
					addCFGtoStmtNodeTable(currNode, startStmt, endStmt);
					prevCFG.clear();
					prevCFG.push_back(currNode);

				}
				// insert node sequence before while
				startStmt = ast.getNode(currAst).getStmtNum();
				endStmt = ast.getNode(currAst).getStmtNum();
				int whileCFG = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
				addCFGtoStmtNodeTable(whileCFG, startStmt, endStmt);

				int whileStmtLst = ast.getNode(currAst).getChildren()[1];
				std::vector<int> endWhile = buildCfg(whileStmtLst, whileCFG, callList);
				
				for (int j=0; j<(int)endWhile.size(); j++)
				{
					cfg.addNext(endWhile[j], whileCFG);
					cfg.addPrev(whileCFG, endWhile[j]);
				}

				// start, end = statement following while, while node becomes prev cfg
				// following statement is the start of next node
				prevCFG.clear();
				prevCFG.push_back(whileCFG);
				toReturn.push_back(whileCFG);

			}

			else if (ast.getNode(currAst).getNodeType() == Node::callNode)
			{
				// insert node sequence before while if they are not control nodes. Else there is no sequence to insert
				if (i>0 && ast.getNode(children[i-1]).getNodeType() == Node::assignNode) {
					int currNode = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
					addCFGtoStmtNodeTable(currNode, startStmt, endStmt);
					prevCFG.clear();
					prevCFG.push_back(currNode);
				}

				startStmt = ast.getNode(currAst).getStmtNum();
				endStmt = ast.getNode(currAst).getStmtNum();
				int callCFG = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
				addCFGtoStmtNodeTable(callCFG, startStmt, endStmt);
				callList.push_back(std::make_pair<int,int>(callCFG, ast.getNode(currAst).getValue()));

				// dummy node for call return
				//callCFG = cfgBip.insertCFGNode(startStmt, endStmt, callCFG);
				//bip.push_back(std::pair<int,int>(startStmt, ast.getNode(currAst).getValue()));


				prevCFG.clear();
				toReturn.push_back(callCFG);

			}


			else if (ast.getNode(currAst).getNodeType() == Node::ifNode)
			{

				endStmt = ast.getNode(currAst).getStmtNum();
				int forkCFG = cfg.insertCFGNode(startStmt, endStmt, prevCFG);
				addCFGtoStmtNodeTable(forkCFG, startStmt, endStmt);

				int thenStmtLst = ast.getNode(currAst).getChildren()[1];
				std::vector<int> endThen = buildCfg(thenStmtLst, forkCFG, callList);

				int elseStmtLst = ast.getNode(currAst).getChildren()[2];
				std::vector<int> endElse = buildCfg(elseStmtLst, forkCFG, callList);

				for (int j=0; j<(int)endThen.size(); j++)
					toReturn.push_back(endThen[j]);
				for (int j=0; j<(int)endElse.size(); j++)
					toReturn.push_back(endElse[j]);
			}
			else
				std::cout << "I am in PKB::buildCFG (2)" << std::endl;
		}
	}

	return toReturn;
}

std::vector<int> PKB::getAffectsStart(int start)
{
	std::vector<int> toReturn;

	// Check if start is an assignment statement
	if (stmtNodeTable.getType(start) != Node::assignNode)
		return toReturn;

	// Keep track of visited stmts
	int numOfStmts = stmtNodeTable.getSize();
	std::vector<int> visited(numOfStmts, -1); // "-1" means unvisited
	std::queue<int> stmtQueue; // for Breadth First Search

	int var = modifiesTable.getModifiedBy(start)[0]; // An assignment statement will only modify 1 variable

	int cfgIndex = stmtNodeTable.getCFG(start);
	std::vector<int> nextStmt = cfg.getNext(start, cfgIndex);

	// Do not mark start as visited, in case "Affects(12, 12)"
	
	// Add next statement(s) into stmtQueue
	for (int i=0; i<(int)nextStmt.size(); i++)
		stmtQueue.push(nextStmt[i]);

	while (stmtQueue.size() > 0)
	{
		int currStmt = stmtQueue.front(); // Access first element
		stmtQueue.pop(); // Delete first element

		if (visited[currStmt] == -1) // currStmt has not been visited
		{
			int nodeType = stmtNodeTable.getType(currStmt);
			if (nodeType == Node::assignNode)
			{
				// currStmt uses "var"
				if (usesTable.isUses(currStmt, var))
					toReturn.push_back(currStmt);

				// currStmt did not modify "var"
				if (!modifiesTable.isModifies(currStmt, var))
				{
					// Add the next statements to stmtQueue
					cfgIndex = stmtNodeTable.getCFG(currStmt);
					std::vector<int> temp = cfg.getNext(currStmt, cfgIndex);
					for (int i=0; i<(int)temp.size(); i++)
						stmtQueue.push(temp[i]);
				}
			}
			else if (nodeType == Node::callNode)
			{
				// procedure call did not modify "var"
				if (!modifiesTable.isModifies(currStmt, var))
				{
					// Add the next statements to stmtQueue
					cfgIndex = stmtNodeTable.getCFG(currStmt);
					std::vector<int> temp = cfg.getNext(currStmt, cfgIndex);
					for (int i=0; i<(int)temp.size(); i++)
						stmtQueue.push(temp[i]);
				}
			}
			else // currStmt is either "while" or "if"
			{
				// Add the next statements to stmtQueue
				cfgIndex = stmtNodeTable.getCFG(currStmt);
				std::vector<int> temp1 = cfg.getNext(currStmt, cfgIndex);
				for (int i=0; i<(int)temp1.size(); i++)
					stmtQueue.push(temp1[i]);
			}

			visited[currStmt] = 1; // Mark currStmt as visited
		}
	}

	return toReturn;
}

// Simple-caching for Affects
std::vector<int> PKB::getAffectsStartAPI(int start)
{
	if (affectsMapStart.count(start) > 0)
		return affectsMapStart[start];
	else
	{
		std::vector<int> ans = getAffectsStart(start);
		affectsMapStart[start] = ans;
		return ans;
	}
}

std::vector<int> PKB::getAffectsEndAPI(int end)
{
	if (affectsMapEnd.count(end) > 0)
		return affectsMapEnd[end];
	else
	{
		std::vector<int> ans = getAffectsEnd(end);
		affectsMapEnd[end] = ans;
		return ans;
	}
}


std::vector<int> PKB::getAffectsEnd(int end)
{
	std::vector<int> toReturn;

	// Check if end is an assignment statement
	if (stmtNodeTable.getType(end) != Node::assignNode)
		return toReturn;

	// Keep track of visited stmts
	int numOfStmts = stmtNodeTable.getSize();
	std::vector<int> visited(numOfStmts, -1); // "-1" means unvisited

	// Multiple variables may be used in the "end" statement
	std::vector<int> varVec = usesTable.getUsedBy(end);
	std::unordered_set<int> varSet(varVec.begin(), varVec.end());

	// Do not mark end as visited, in case "Affects(12, 12)"

	int cfgIndex = stmtNodeTable.getCFG(end);
	std::vector<int> prevStmt = cfg.getPrev(end, cfgIndex);
	for (int i=0; i<(int)prevStmt.size(); i++)
	{
		std::vector<int> temp = depthUp(prevStmt[i], varSet, visited);
		toReturn.insert(toReturn.end(), temp.begin(), temp.end());
	}

	return toReturn;
}


// Getting reverse Affects need depth first search due to changing varSet
std::vector<int> PKB::depthUp(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited)
{
	std::vector<int> toReturn;

	// currStmt has been visited before or all the variables used by "end statement" had been found
	if (visited[currStmt] == 1 || varSet.empty())
		return toReturn;

	visited[currStmt] = 1; // Mark currStmt as visited

	int nodeType = stmtNodeTable.getType(currStmt);
	if (nodeType == Node::assignNode)
	{			
		int var = modifiesTable.getModifiedBy(currStmt)[0]; // An assignment statement will only modify 1 variable
		
		// currStmt modifies a variable in varSet
		if (varSet.count(var) > 0)
		{
			toReturn.push_back(currStmt);
			varSet.erase(var); // This modified variable will be taken out of varSet
		}
	}
	else if (nodeType == Node::callNode)
	{
		// Check if the procedure call modifies any of the variables in varSet
		std::vector<int> modifiedVars = modifiesTable.getModifiedBy(currStmt);
		for (int i=0; i<(int)modifiedVars.size(); i++)
		{
			if (varSet.count(modifiedVars[i]) > 0)
				varSet.erase(modifiedVars[i]); // This modified variable will be taken out of varSet
		}
	}

	// Depth first search the previous statements
	int cfgIndex = stmtNodeTable.getCFG(currStmt);
	std::vector<int> prevStmt = cfg.getPrev(currStmt, cfgIndex);
	for (int i=0; i<(int)prevStmt.size(); i++)
	{
		std::vector<int> temp = depthUp(prevStmt[i], varSet, visited);
		toReturn.insert(toReturn.end(), temp.begin(), temp.end());
	}

	return toReturn;
}

std::vector<int> PKB::getAffectsTStart(int start)
{
	std::vector<int> toReturn;

	// Check if start is an assignment statement
	if (stmtNodeTable.getType(start) != Node::assignNode)
		return toReturn;

	// Keep track of visited stmts
	int numOfStmts = stmtNodeTable.getSize();
	std::vector<int> visited(numOfStmts, -1); // "-1" means unvisited

	int var = modifiesTable.getModifiedBy(start)[0]; // An assignment statement will only modify 1 variable
	std::unordered_set<int> varSet;
	varSet.insert(var);
	std::unordered_set<int> ignoreSet;

	// Do not mark start as visited, in case "Affects(12, 12)"

	int cfgIndex = stmtNodeTable.getCFG(start);
	std::vector<int> nextStmt = cfg.getNext(start, cfgIndex);
	for (int i=0; i<(int)nextStmt.size(); i++)
	{
		std::vector<int> temp = depthDownT(nextStmt[i], varSet, visited, ignoreSet);
		toReturn.insert(toReturn.end(), temp.begin(), temp.end());
	}

	// Remove duplicates before return answer
	std::unordered_set<int> uniqueReturn(toReturn.begin(), toReturn.end());
	std::vector<int> finalReturn(uniqueReturn.begin(), uniqueReturn.end());

	return finalReturn;
}

std::vector<int> PKB::depthDownT(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::unordered_set<int> ignoreSet)
{
	std::vector<int> toReturn;

	// currStmt has been visited before or all the variables used by "end statement" had been found
	if (visited[currStmt] == 1 || varSet.empty())
		return toReturn;

	visited[currStmt] = 1; // Mark currStmt as visited

	int nodeType = stmtNodeTable.getType(currStmt);
	if (nodeType == Node::assignNode && ignoreSet.count(currStmt) == 0)
	{			
		int var = modifiesTable.getModifiedBy(currStmt)[0]; // An assignment statement will only modify 1 variable
		std::vector<int> varsUsedByCurrStmt = usesTable.getUsedBy(currStmt);
		bool canErase = true;

		for (int i=0; i<(int)varsUsedByCurrStmt.size(); i++)
		{
			// currStmt uses a var in varSet
			if (varSet.count(varsUsedByCurrStmt[i]) > 0)
			{
				toReturn.push_back(currStmt);
				varSet.insert(var);
				int numOfStmts = stmtNodeTable.getSize();
				std::vector<int> temp(numOfStmts, -1);
				visited = temp; // Reset all stmts to unvisited
				ignoreSet.insert(currStmt); // Do not process this stmt next time
				canErase = false; // We cannot remove the modified var in varSet
				break;
			}
		}

		// currStmt modifies a var in varSet and this var was not used
		if (canErase && varSet.count(var) > 0)
			varSet.erase(var); // This modified variable will be taken out of varSet
	}
	else if (nodeType == Node::callNode)
	{
		// Check if the procedure call modifies any of the variables in varSet
		std::vector<int> modifiedVars = modifiesTable.getModifiedBy(currStmt);
		for (int i=0; i<(int)modifiedVars.size(); i++)
		{
			if (varSet.count(modifiedVars[i]) > 0)
				varSet.erase(modifiedVars[i]); // This modified variable will be taken out of varSet
		}
	}

	// Depth first search the next statements
	int cfgIndex = stmtNodeTable.getCFG(currStmt);
	std::vector<int> nextStmt = cfg.getNext(currStmt, cfgIndex);
	for (int i=0; i<(int)nextStmt.size(); i++)
	{
		std::vector<int> temp = depthDownT(nextStmt[i], varSet, visited, ignoreSet);
		toReturn.insert(toReturn.end(), temp.begin(), temp.end());
	}

	return toReturn;
}

std::vector<int> PKB::getAffectsTEnd(int end)
{
	std::vector<int> toReturn;

	// Check if end is an assignment statement
	if (stmtNodeTable.getType(end) != Node::assignNode)
		return toReturn;

	// Keep track of visited stmts
	int numOfStmts = stmtNodeTable.getSize();
	std::vector<int> visited(numOfStmts, -1); // "-1" means unvisited

	// Multiple variables may be used in the "end" statement
	std::vector<int> varVec = usesTable.getUsedBy(end);
	std::unordered_set<int> varSet(varVec.begin(), varVec.end());
	std::unordered_set<int> ignoreSet;

	// Do not mark end as visited, in case "Affects(12, 12)"

	int cfgIndex = stmtNodeTable.getCFG(end);
	std::vector<int> prevStmt = cfg.getPrev(end, cfgIndex);
	for (int i=0; i<(int)prevStmt.size(); i++)
	{
		std::vector<int> temp = depthUpT(prevStmt[i], varSet, visited, ignoreSet);
		toReturn.insert(toReturn.end(), temp.begin(), temp.end());
	}

	// Remove duplicates before return answer
	std::unordered_set<int> uniqueReturn(toReturn.begin(), toReturn.end());
	std::vector<int> finalReturn(uniqueReturn.begin(), uniqueReturn.end());

	return finalReturn;
}

std::vector<int> PKB::depthUpT(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::unordered_set<int> ignoreSet)
{
	std::vector<int> toReturn;

	// currStmt has been visited before or all the variables used by "end statement" had been found
	if (visited[currStmt] == 1 || varSet.empty())
		return toReturn;

	visited[currStmt] = 1; // Mark currStmt as visited

	int nodeType = stmtNodeTable.getType(currStmt);
	if (nodeType == Node::assignNode && ignoreSet.count(currStmt) == 0)
	{			
		int var = modifiesTable.getModifiedBy(currStmt)[0]; // An assignment statement will only modify 1 variable
		
		// currStmt modifies a variable in varSet
		if (varSet.count(var) > 0)
		{
			toReturn.push_back(currStmt);
			varSet.erase(var); // This modified variable will be taken out of varSet

			// Add in the vars used by currStmt to varSet
			std::vector<int> varsUsedByCurrStmt = usesTable.getUsedBy(currStmt);
			for (int i=0; i<(int)varsUsedByCurrStmt.size(); i++)
				varSet.insert(varsUsedByCurrStmt[i]);
			
			int numOfStmts = stmtNodeTable.getSize();
			std::vector<int> temp(numOfStmts, -1);
			visited = temp; // Reset all stmts to unvisited
			ignoreSet.insert(currStmt); // Do not process this stmt next time
		}
	}
	else if (nodeType == Node::callNode)
	{
		// Check if the procedure call modifies any of the variables in varSet
		std::vector<int> modifiedVars = modifiesTable.getModifiedBy(currStmt);
		for (int i=0; i<(int)modifiedVars.size(); i++)
		{
			if (varSet.count(modifiedVars[i]) > 0)
				varSet.erase(modifiedVars[i]); // This modified variable will be taken out of varSet
		}
	}

	// Depth first search the previous statements
	int cfgIndex = stmtNodeTable.getCFG(currStmt);
	std::vector<int> prevStmt = cfg.getPrev(currStmt, cfgIndex);
	for (int i=0; i<(int)prevStmt.size(); i++)
	{
		std::vector<int> temp = depthUpT(prevStmt[i], varSet, visited, ignoreSet);
		toReturn.insert(toReturn.end(), temp.begin(), temp.end());
	}

	return toReturn;
}

// Simple-caching for Affects*
std::vector<int> PKB::getAffectsTStartAPI(int start)
{
	if (affectsTMapStart.count(start) > 0)
		return affectsTMapStart[start];
	else
	{
		std::vector<int> ans = getAffectsTStart(start);
		affectsTMapStart[start] = ans;
		return ans;
	}
}

std::vector<int> PKB::getAffectsTEndAPI(int end)
{
	if (affectsTMapEnd.count(end) > 0)
		return affectsTMapEnd[end];
	else
	{
		std::vector<int> ans = getAffectsTEnd(end);
		affectsTMapEnd[end] = ans;
		return ans;
	}
}

std::vector<int> PKB::getAffectsBipStart(int start)
{
	std::vector<int> toReturn;

	// Check if start is an assignment statement
	if (stmtNodeTable.getType(start) != Node::assignNode)
		return toReturn;

	// Keep track of visited stmts
	int numOfStmts = stmtNodeTable.getSize();
	std::vector<int> visited(numOfStmts, -1); // "-1" means unvisited

	int var = modifiesTable.getModifiedBy(start)[0]; // An assignment statement will only modify 1 variable

	// Do not mark start as visited, in case "AffectsBip(12, 12)"

	int cfgIndex = stmtNodeTable.getCFG(start);
	// Use getNextBip, in case currStmt is last stmt
	std::vector<int> nextStmtBip = cfg.getNextBip(start, cfgIndex);
	std::vector<int> branchIn; // Currently in main branch, so branchIn is empty
	for (int i=0; i<(int)nextStmtBip.size(); i++)
	{
		std::vector<int> temp = depthDownBip(nextStmtBip[i], var, visited, branchIn);
		toReturn.insert(toReturn.end(), temp.begin(), temp.end());
	}
	
	// Remove duplicates before return answer
	std::unordered_set<int> uniqueReturn(toReturn.begin(), toReturn.end());
	std::vector<int> finalReturn(uniqueReturn.begin(), uniqueReturn.end());

	return finalReturn;

}


std::vector<int> PKB::depthDownBip(int currStmt, int var, std::vector<int> visited, std::vector<int> branchIn)
{
	std::vector<int> toReturn;

	// currStmt has been visited before
	if (visited[currStmt] == 1)
		return toReturn;

	visited[currStmt] = 1; // Mark currStmt as visited

	int nodeType = stmtNodeTable.getType(currStmt);
	if (nodeType == Node::assignNode)
	{
		std::vector<int> varsUsedByCurrStmt = usesTable.getUsedBy(currStmt);

		for (int i=0; i<(int)varsUsedByCurrStmt.size(); i++)
		{
			// currStmt uses the targeted variable
			if (varsUsedByCurrStmt[i] == var)
			{
				toReturn.push_back(currStmt);
				break;
			}
		}

		int currVar = modifiesTable.getModifiedBy(currStmt)[0]; // An assignment statement will only modify 1 variable
		// currStmt does not modify the targeted variable
		if (currVar != var)
		{
			// Depth first search the next statements
			int cfgIndex = stmtNodeTable.getCFG(currStmt);
			std::vector<int> nextStmt = cfg.getNext(currStmt, cfgIndex);

			// If currStmt is last stmt of proc and proc is main branch
			if (branchIn.empty() && nextStmt.empty())
			{
				std::vector<int> nextStmtBip = cfg.getNextBip(currStmt, cfgIndex);
				for (int i=0; i<(int)nextStmtBip.size(); i++)
				{
					std::vector<int> temp = depthDownBip(nextStmtBip[i], var, visited, branchIn);
					toReturn.insert(toReturn.end(), temp.begin(), temp.end());
				}
			}
			else if (nextStmt.empty()) // at last stmt and in sub branch, so should return to prev branch
			{
				// Reset curr proc stmts to unvisited
				int currProcInd = procTable.getProcOfStmt(currStmt);
				int firstStmt = procTable.getProcFirstln(currProcInd);
				int lastStmt = procTable.getProcLastln(currProcInd);
				for (int k = firstStmt; k <= lastStmt; k++)
					visited[k] = -1;

				int nextBipStmt = branchIn.back();
				branchIn.pop_back();
				std::vector<int> temp = depthDownBip(nextBipStmt, var, visited, branchIn);
				toReturn.insert(toReturn.end(), temp.begin(), temp.end());
			}
			else // currStmt is not the last stmt
			{
				for (int i=0; i<(int)nextStmt.size(); i++)
				{
					std::vector<int> temp = depthDownBip(nextStmt[i], var, visited, branchIn);
					toReturn.insert(toReturn.end(), temp.begin(), temp.end());
				}
			}
		}
	}
	else if (nodeType == Node::callNode)
	{
		int cfgIndex = stmtNodeTable.getCFG(currStmt);
		std::vector<int> nextStmt = cfg.getNext(currStmt, cfgIndex); // We are making use of the assumption getNext(callstmt) only has 0 or 1 stmt

		if (!nextStmt.empty()) // call stmt is not the last stmt of current procedure
		{
			// Jump to the first stmt of called procedure, add the nextStmt to branch back in later
			branchIn.push_back(nextStmt[0]);
			std::vector<int> nextStmtBip = cfg.getNextBip(currStmt, cfgIndex);
			for (int i=0; i<(int)nextStmtBip.size(); i++)
			{
				std::vector<int> temp = depthDownBip(nextStmtBip[i], var, visited, branchIn);
				toReturn.insert(toReturn.end(), temp.begin(), temp.end());
			}
		}
		else // call stmt is the last stmt of current procedure
		{
			if (branchIn.empty()) // in main branch
			{
				int currProcInd = procTable.getProcOfStmt(currStmt);
				int dummyCfgNodeInd = procTable.getCFGEnd(currProcInd);
				std::vector<int> nextBipDummy = cfg.getNextBip(-1, dummyCfgNodeInd);

				if (nextBipDummy.empty()) // no where else to branch after we return
				{
					std::vector<int> nextStmtBip = cfg.getNextBip(currStmt, cfgIndex);
					for (int i=0; i<(int)nextStmtBip.size(); i++)
					{
						std::vector<int> temp = depthDownBip(nextStmtBip[i], var, visited, branchIn);
						toReturn.insert(toReturn.end(), temp.begin(), temp.end());
					}
				}
				// Add the many possible nextStmt to branch back in later
				for (int j=0; j<(int)nextBipDummy.size(); j++)
				{
					branchIn.push_back(nextBipDummy[j]);
					// Jump to the first stmt of called procedure
					std::vector<int> nextStmtBip = cfg.getNextBip(currStmt, cfgIndex);
					for (int i=0; i<(int)nextStmtBip.size(); i++)
					{
						std::vector<int> temp = depthDownBip(nextStmtBip[i], var, visited, branchIn);
						toReturn.insert(toReturn.end(), temp.begin(), temp.end());
					}
					branchIn.pop_back();
				}
			}
			else // in subBranch, we need to return to prev branch after we branch out
			{
				// Reset curr proc stmts to unvisited
				int currProcInd = procTable.getProcOfStmt(currStmt);
				int firstStmt = procTable.getProcFirstln(currProcInd);
				int lastStmt = procTable.getProcLastln(currProcInd);
				for (int k = firstStmt; k <= lastStmt; k++)
					visited[k] = -1;

				// No need to handle branchIn, coz we cannot pop it yet
				std::vector<int> nextStmtBip = cfg.getNextBip(currStmt, cfgIndex);
				for (int i=0; i<(int)nextStmtBip.size(); i++)
				{
					std::vector<int> temp = depthDownBip(nextStmtBip[i], var, visited, branchIn);
					toReturn.insert(toReturn.end(), temp.begin(), temp.end());
				}
			}
		}
	}
	else if (nodeType == Node::whileNode)
	{
		int cfgIndex = stmtNodeTable.getCFG(currStmt);
		std::vector<int> nextStmt = cfg.getNext(currStmt, cfgIndex);
		if ((int)nextStmt.size() == 1) // while stmt is actually the last stmt, hence only 1 nextStmt
		{
			if (visited[nextStmt[0]] == -1) // stmt in while loop has not been visited yet
			{
				visited[currStmt] = -1; // set while node to unvisited so we can visited it again and do branch out
				std::vector<int> temp = depthDownBip(nextStmt[0], var, visited, branchIn);
				toReturn.insert(toReturn.end(), temp.begin(), temp.end());
			}
			else // time to branch out
			{
				if (branchIn.empty()) // in main branch
				{
					std::vector<int> nextStmtBip = cfg.getNextBip(currStmt, cfgIndex);
					for (int i=0; i<(int)nextStmtBip.size(); i++)
					{
						// branchIn remains empty, as the next procedure becomes the main branch
						std::vector<int> temp = depthDownBip(nextStmtBip[i], var, visited, branchIn);
						toReturn.insert(toReturn.end(), temp.begin(), temp.end());
					}
				}
				else // in sub branch, so we return to previous branch
				{
					// Reset curr proc stmts to unvisited
					int currProcInd = procTable.getProcOfStmt(currStmt);
					int firstStmt = procTable.getProcFirstln(currProcInd);
					int lastStmt = procTable.getProcLastln(currProcInd);
					for (int k = firstStmt; k <= lastStmt; k++)
						visited[k] = -1;

					int nextBipStmt = branchIn.back();
					branchIn.pop_back();
					std::vector<int> temp = depthDownBip(nextBipStmt, var, visited, branchIn);
					toReturn.insert(toReturn.end(), temp.begin(), temp.end());
				}
			}
		}
		else // while stmt is not the last stmt, normal getNext
		{
			for (int i=0; i<(int)nextStmt.size(); i++)
			{
				std::vector<int> temp = depthDownBip(nextStmt[i], var, visited, branchIn);
				toReturn.insert(toReturn.end(), temp.begin(), temp.end());
			}
		}
	}
	else // nodeType is "if", hence cannot be last stmt
	{
		// Depth first search the next statements, use normal getNext
		int cfgIndex = stmtNodeTable.getCFG(currStmt);
		std::vector<int> nextStmt = cfg.getNext(currStmt, cfgIndex);
		for (int i=0; i<(int)nextStmt.size(); i++)
		{
			std::vector<int> temp = depthDownBip(nextStmt[i], var, visited, branchIn);
			toReturn.insert(toReturn.end(), temp.begin(), temp.end());
		}
	}

	return toReturn;
}


std::vector<int> PKB::getAffectsBipEnd(int end)
{
	std::vector<int> toReturn;

	// Check if end is an assignment statement
	if (stmtNodeTable.getType(end) != Node::assignNode)
		return toReturn;

	// Keep track of visited stmts
	int numOfStmts = stmtNodeTable.getSize();
	std::vector<int> visited(numOfStmts, -1); // "-1" means unvisited

	// Multiple variables may be used in the "end" statement
	std::vector<int> varVec = usesTable.getUsedBy(end);
	std::unordered_set<int> varSet(varVec.begin(), varVec.end());

	// Do not mark end as visited, in case "AffectsBip(12, 12)"

	int cfgIndex = stmtNodeTable.getCFG(end);
	std::vector<int> prevStmt = cfg.getPrev(end, cfgIndex);
	std::unordered_set<int> prevSet(prevStmt.begin(), prevStmt.end());

	std::unordered_map<int, int> procToStmtMap;
	for (int i=0; i<(int)prevStmt.size(); i++)
	{
		// There could be multiple prevStmt that are call stmt, we need to map them for branchIn
		if (stmtNodeTable.getType(prevStmt[i]) == Node::callNode)
		{
			int callNodeAstInd = stmtNodeTable.getAST(prevStmt[i]);
			int procInd = ast.getNode(callNodeAstInd).getValue();
			procToStmtMap[procInd] = prevStmt[i];
		}
	}

	std::vector<int> prevStmtBip = cfg.getPrevBip(end, cfgIndex);
	std::vector<int> branchIn; // Currently in main branch, so branchIn is empty
	for (int i=0; i<(int)prevStmtBip.size(); i++)
	{
		// if the prevBip stmt is not a getPrev stmt, means we are branching to another proc
		if (prevSet.count(prevStmtBip[i]) == 0)
		{
			int prevBipProcInd = procTable.getProcOfStmt(prevStmtBip[i]);
			// if prevBipStmt was in a procedure that prevStmt called, we going in sub branch
			if (procToStmtMap.count(prevBipProcInd) > 0)
			{
				branchIn.push_back(procToStmtMap[prevBipProcInd]);
				std::vector<int> temp = depthUpBip(prevStmtBip[i], varSet, visited, branchIn);
				toReturn.insert(toReturn.end(), temp.begin(), temp.end());
				branchIn.pop_back();
			}
			else // we are currently at first stmt, we going in main branch
			{
				std::vector<int> temp = depthUpBip(prevStmtBip[i], varSet, visited, branchIn);
				toReturn.insert(toReturn.end(), temp.begin(), temp.end());
			}
		}
		else // in the same procedure
		{
			std::vector<int> temp = depthUpBip(prevStmtBip[i], varSet, visited, branchIn);
			toReturn.insert(toReturn.end(), temp.begin(), temp.end());
		}
	}

	// Remove duplicates before return answer
	std::unordered_set<int> uniqueReturn(toReturn.begin(), toReturn.end());
	std::vector<int> finalReturn(uniqueReturn.begin(), uniqueReturn.end());

	return finalReturn;

}


std::vector<int> PKB::depthUpBip(int currStmt, std::unordered_set<int> varSet, std::vector<int> visited, std::vector<int> branchIn)
{
	std::vector<int> toReturn;

	// currStmt has been visited before or all the variables used by "end statement" had been found
	if (visited[currStmt] == 1 || varSet.empty())
		return toReturn;

	visited[currStmt] = 1; // Mark currStmt as visited

	int nodeType = stmtNodeTable.getType(currStmt);
	if (nodeType == Node::assignNode)
	{			
		int var = modifiesTable.getModifiedBy(currStmt)[0]; // An assignment statement will only modify 1 variable
		
		// currStmt modifies a variable in varSet
		if (varSet.count(var) > 0)
		{
			toReturn.push_back(currStmt);
			varSet.erase(var); // This modified variable will be taken out of varSet
		}
	}

	int cfgIndex = stmtNodeTable.getCFG(currStmt);
	std::vector<int> prevStmt = cfg.getPrev(currStmt, cfgIndex);
	std::unordered_set<int> prevSet(prevStmt.begin(), prevStmt.end());

	std::unordered_map<int, int> procToStmtMap;
	for (int i=0; i<(int)prevStmt.size(); i++)
	{
		// There could be multiple prevStmt that are call stmt, we need to map them for branchIn
		if (stmtNodeTable.getType(prevStmt[i]) == Node::callNode)
		{
			int callNodeAstInd = stmtNodeTable.getAST(prevStmt[i]);
			int procInd = ast.getNode(callNodeAstInd).getValue();
			procToStmtMap[procInd] = prevStmt[i];
		}
	}

	std::vector<int> prevStmtBip = cfg.getPrevBip(currStmt, cfgIndex);
	for (int i=0; i<(int)prevStmtBip.size(); i++)
	{
		// if the prevBip stmt is not a getPrev stmt, means we are branching to another proc
		if (prevSet.count(prevStmtBip[i]) == 0)
		{
			int prevBipProcInd = procTable.getProcOfStmt(prevStmtBip[i]);
			// if prevBipStmt was in a procedure that prevStmt called, we going in sub branch
			if (procToStmtMap.count(prevBipProcInd) > 0)
			{
				branchIn.push_back(procToStmtMap[prevBipProcInd]);
				std::vector<int> temp = depthUpBip(prevStmtBip[i], varSet, visited, branchIn);
				toReturn.insert(toReturn.end(), temp.begin(), temp.end());
				branchIn.pop_back();
			}
			else // we are currently at first stmt
			{
				if (branchIn.empty()) // we are main branch
				{
					std::vector<int> temp = depthUpBip(prevStmtBip[i], varSet, visited, branchIn);
					toReturn.insert(toReturn.end(), temp.begin(), temp.end());
				}
				else // we are sub branch, branching back now
				{
					// Reset curr proc stmts to unvisited
					int currProcInd = procTable.getProcOfStmt(currStmt);
					int firstStmt = procTable.getProcFirstln(currProcInd);
					int lastStmt = procTable.getProcLastln(currProcInd);
					for (int k = firstStmt; k <= lastStmt; k++)
						visited[k] = -1;

					int branchToStmt = branchIn.back();
					branchIn.pop_back();
					std::vector<int> temp = depthUpBip(branchToStmt, varSet, visited, branchIn);
					toReturn.insert(toReturn.end(), temp.begin(), temp.end());
				}
			}
		}
		else // in the same procedure
		{
			std::vector<int> temp = depthUpBip(prevStmtBip[i], varSet, visited, branchIn);
			toReturn.insert(toReturn.end(), temp.begin(), temp.end());
		}
	}

	return toReturn;
}



void PKB::addCFGtoStmtNodeTable(int cfgNode, int startStmt, int endStmt){
	for (int i=startStmt; i<=endStmt; i++)
		stmtNodeTable.setCFG(i, cfgNode);
}
