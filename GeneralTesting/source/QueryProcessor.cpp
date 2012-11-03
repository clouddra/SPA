#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

std::vector<std::string> intVecToStringVec(std::vector<int> input) {
    std::vector<std::string> output;
    for (int i = 0; i < (int)input.size(); i++) {
        output.push_back(std::to_string((long long)input[i]));
    }
    return output;
}

QueryProcessor::QueryProcessor()
{
	queryTree = QueryTree();
	declarationTable = DeclarationTable();
}

int QueryProcessor::insertNode(std::string _name, std::string _value, int _parent)
{
	int newNodeIndex = queryTree.insertNode(_name, _value, _parent);

	return newNodeIndex;
}

// Inserting into declaration table
void QueryProcessor::loadDeclaration(std::vector<QueryNode> tree, int* curr) {
    std::vector<int> rootChildren = tree[0].getChildren();
    QueryNode currNode = tree[rootChildren[*curr]];

    while (currNode.getName().compare("declaration") == 0) {
        int varType;
        if (currNode.getValue().compare("stmt") == 0) {
            varType = DeclarationTable::stmt_;
        }
        else if (currNode.getValue().compare("assign") == 0) {
            varType = DeclarationTable::assign_;
        }
        else if (currNode.getValue().compare("while") == 0) {
            varType = DeclarationTable::while_;
        }
        else if (currNode.getValue().compare("variable") == 0) {
            varType = DeclarationTable::variable_;
        }
        else if (currNode.getValue().compare("constant") == 0) {
            varType = DeclarationTable::constant_;
        }
        else if (currNode.getValue().compare("prog_line") == 0) {
            varType = DeclarationTable::prog_line_;
        }

        std::vector<int> childHolder = currNode.getChildren();
        for (int i = 0; i < (int)childHolder.size(); i++) {
            declarationTable.insertDeclaration(varType, tree[childHolder[i]].getName());
        }
        *curr = *curr+1;
        currNode = tree[rootChildren[*curr]];
    }
}

// Figure out type of a parameter 
// Note: return value of -1 means that parameter is either an entity or a number not an error, rather -2 is the error return 
int QueryProcessor::findTypeOf(std::string para, bool* paraIsNum, bool* paraIsEnt, int* paraNum) {
    *paraIsNum = *paraIsEnt = false;
    int paraType = declarationTable.getType(para);
    if (paraType == -1) {
        if (para.find("\"") != std::string::npos) {
            *paraIsEnt = true;
        }
        else {
            std::istringstream convert(para);
            if (convert >> *paraNum) {
                *paraIsNum = true;
            }
            else {  // Can't figure out type of para ??
                return -2;
            }
        }
    }
    return paraType;
}

// Evaluating follows and follows* query
int QueryProcessor::evaluateFollows(bool T, bool para1IsNum, bool para2IsNum, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    // Inserting valid values based on parameter type (if variable)
    if (!para1IsNum) {
        evaluateType(pkb, para1);
    }
    if (!para2IsNum) {
        evaluateType(pkb, para2);
    }

    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            if (para2IsNum) {
                if (!pkb.isFollow(para1Num, para2Num))  // Follows(num1, num2) is false, whole query is false
                    return -1;
                // Follows(num1, num2) is true, do nothing
            }
            else {
                temp = pkb.getFollowedBy(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para2, toStore);
                if (ret == -1)  // Exit cond
                    return -1;
            }
        }
        else if (para2IsNum) {
            temp = pkb.getFollows(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = vvTable.insert(para1, toStore);
            if (ret == -1)  // Exit cond
                return -1;
        }
        else {
            // Double variable e.g Follows(s1, s2) Imcomplete
        }
    }
    else {
        if (para1IsNum) {
            if (para2IsNum) {
                bool found = false;
                temp = pkb.getFollowsT(para2Num);
                for (int i = 0; i < (int)temp.size(); i++) {
                    if (para1Num == temp[i]) {
                        found = true;
                        break;
                    }
                }
                if (!found)  // Follows*(num1, num2) is false, whole query is false
                    return -1;
                // Follows*(num1, num2) is true, do nothing
            }
            else {
                temp = pkb.getFollowedByT(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para2, toStore);
                if (ret == -1)  // Exit cond
                    return -1;
            }
        }
        else if (para2IsNum) {
            temp = pkb.getFollowsT(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = vvTable.insert(para1, toStore);
            if (ret == -1)  // Exit cond
                return -1;
        }
        else {
            // Double variable e.g Follows(s1, s2) Imcomplete
        }
    }
    return 0;
}

// Evaluating based on variable type (target should be a declared variable in the declaration table)
int QueryProcessor::evaluateType(PKB pkb, std::string target) {
    int targetType = declarationTable.getType(target);
    std::vector<int> validStmtNum;
    std::vector<std::string> toStore;

    switch (targetType) {
        case -1:    // Target is not declared
        {
            return -1;
        }

        case DeclarationTable::stmt_:  
        {
            int temp = pkb.getNumStmts();
            for (int i = 1; i <= temp; i++) {
                validStmtNum.push_back(i);
            }
            toStore = intVecToStringVec(validStmtNum);
            int ret = vvTable.insert(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::assign_:
        {
            validStmtNum = pkb.getStmtWithType(Node::assignNode);
            toStore = intVecToStringVec(validStmtNum);
            int ret = vvTable.insert(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::while_:
        {
            validStmtNum = pkb.getStmtWithType(Node::whileNode);
            toStore = intVecToStringVec(validStmtNum);
            int ret = vvTable.insert(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }
    }
    return 0;
}

// Walk our Query Tree and store results in result vector
void QueryProcessor::processQuery(PKB pkb)
{
    std::vector<QueryNode> tree = queryTree.getTree();
    std::string target = tree[0].getValue(); 
    std::vector<int> rootChildren = tree[0].getChildren();   
    int curr = 0;
    QueryNode currNode = tree[rootChildren[curr]];
    result.push_back("FALSE");

    // Inserting into declaration table
    loadDeclaration(tree, &curr);

    // Evaluating clauses (Incomplete)
    for (int i = curr; i < (int)rootChildren.size(); i++) {
        currNode = tree[rootChildren[i]];
        if (currNode.getName().compare("such that") == 0) {
            QueryNode relation = tree[currNode.getChildren()[0]];
            int paraNode1 = relation.getChildren()[0];
            std::string para1 = tree[paraNode1].getName();
            int paraNode2 = relation.getChildren()[1];
            std::string para2 = tree[paraNode2].getName();

            // Figure out type of parameter 1 and 2
            bool para1IsNum = false, para2IsNum = false;
            bool para1IsEnt = false, para2IsEnt = false;
            int para1Num = -1, para2Num = -1;
            int para1Type = findTypeOf(para1, &para1IsNum, &para1IsEnt, &para1Num);
            int para2Type = findTypeOf(para2, &para2IsNum, &para2IsEnt, &para2Num);
            if (para1Type == -2 || para2Type == -2) {   // Cannot figure out parameter type
                return;
            }

            // follows query, assuming no entities
            if (relation.getName().compare("follows") == 0) {
                int ret = evaluateFollows(false, para1IsNum, para2IsNum, para1, para2, para1Num, para2Num, pkb); 
                if (ret == -1)
                    return;
            }
            // follows* query, assuming no entities
            if (relation.getName().compare("followst") == 0) {
                int ret = evaluateFollows(true, para1IsNum, para2IsNum, para1, para2, para1Num, para2Num, pkb); 
                if (ret == -1)
                    return;
            }
        }
    } 

    // Evaluating select (Currently only doing stmt, assign, while)
    bool isBool = false;
    if (target.compare("BOOLEAN") == 0) 
        isBool = true;
    int ret = evaluateType(pkb, target);
    if (ret == -1)
        return;

    // This is the final step, inserting into result, please do not do any evaluation after this
    // Only insert into result here (besides the FALSE at the start) as results will be cleared here
    result.clear(); 
    if (isBool) 
        result.push_back("TRUE");
    else
        result = vvTable.getValues(target);
}

void QueryProcessor::printTree() 
{
    std::vector<QueryNode> temp = queryTree.getTree();
	for (int i=0; i<(int)temp.size(); i++)
		std::cout << "Name: " << temp[i].getName() << ", Value: " << temp[i].getValue() << std::endl;
}

// Note: Most of the time result should be sorted if its stmtNum, but no guarantee
void QueryProcessor::printResult()
{
	for (int i=0; i<(int)result.size(); i++) {
		std::cout << result[i] << " ";
        if ((i % 10) == 9)
            std::cout << std::endl;
    }
}