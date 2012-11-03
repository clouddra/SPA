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

// Walk our Query Tree and store results in result vector
void QueryProcessor::processQuery(PKB pkb)
{
    std::vector<QueryNode> tree = queryTree.getTree();
    std::string target = tree[0].getValue(); 
    std::vector<int> rootChildren = tree[0].getChildren();   
    int curr = 0;
    QueryNode currNode = tree[rootChildren[curr]];

    // Inserting into declaration table
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
        curr++;
        currNode = tree[rootChildren[curr]];
    }

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
            // Parameter 1
            int para1Type = declarationTable.getType(para1);
            if (para1Type == -1) {
                if (para1.find("\"") != std::string::npos) {
                    para1IsEnt = true;
                    para1 = para1.substr(1, para1.size()-2);
                }
                else {
                    std::istringstream convert(para1);
                    if (convert >> para1Num) {
                        para1IsNum = true;
                    }
                    else {  // Can't figure out type of para 1 ??
                        return;
                    }
                }
            }
            // Parameter 2
            int para2Type = declarationTable.getType(para2);
            if (para2Type == -1) {
                if (para1.find("\"") != std::string::npos) {
                    para2IsEnt = true;
                    para2 = para2.substr(1, para2.size()-2);
                }
                else {
                    std::istringstream convert(para2);
                    if (convert >> para2Num) {
                        para2IsNum = true;
                    }
                    else {  // Can't figure out type of para 2 ??
                        return;
                    }
                }
            }

            std::vector<int> temp;
            std::vector<std::string> toStore;
            // follows query, assuming no entities
            if (relation.getName().compare("follows") == 0) {
                if (para1IsNum) {
                    if (para2IsNum) {
                        if (!pkb.isFollow(para1Num, para2Num))  // Follows(num1, num2) is false, whole query is false
                            return;
                        // Follows(num1, num2) is true, do nothing
                    }
                    else {
                        temp = pkb.getFollowedBy(para1Num);
                        toStore = intVecToStringVec(temp);
                        int ret = vvTable.insert(para2, toStore);
                        if (ret == -1)  // Exit cond
                            return;
                    }
                }
                else if (para2IsNum) {
                    temp = pkb.getFollows(para2Num);
                    toStore = intVecToStringVec(temp);
                    int ret = vvTable.insert(para1, toStore);
                    if (ret == -1)  // Exit cond
                        return;
                }
                else {
                    // Double variable e.g Follows(s1, s2)
                }
            }
        }
    } 

    // Evaluating select (Currently only doing stmt, assign, while)
    // This is the last step (ie we set the result here) so no other evaluation should take place after this
    int targetType = declarationTable.getType(target);
    std::vector<int> validStmtNum;
    std::vector<std::string> toStore;
    switch (targetType) {
        case -1:    // Target is not declared ?? (Could be BOOLEAN)
        {
            if (target.compare("BOOLEAN") == 0)
                result.push_back("TRUE");
            return;
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
                return;
            result = vvTable.getValues(target);
            break;
        }

        case DeclarationTable::assign_:
        {
            validStmtNum = pkb.getStmtWithType(Node::assignNode);
            toStore = intVecToStringVec(validStmtNum);
            int ret = vvTable.insert(target, toStore);
            if (ret == -1)  // Exit cond
                return;
            result = vvTable.getValues(target);
            break;
        }

        case DeclarationTable::while_:
        {
            validStmtNum = pkb.getStmtWithType(Node::whileNode);
            toStore = intVecToStringVec(validStmtNum);
            int ret = vvTable.insert(target, toStore);
            if (ret == -1)  // Exit cond
                return;
            result = vvTable.getValues(target);
            break;
        }
    }
}

void QueryProcessor::printTree() 
{
    std::vector<QueryNode> temp = queryTree.getTree();
	for (int i=0; i<(int)temp.size(); i++)
		std::cout << "Name: " << temp[i].getName() << ", Value: " << temp[i].getValue() << std::endl;
}

void QueryProcessor::printResult()
{
	for (int i=0; i<(int)result.size(); i++) {
		std::cout << result[i] << " ";
        if ((i % 10) == 9)
            std::cout << std::endl;
    }
}