#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

#include <set>

// Notation guide: Variables = Declared in pql query, Entities = Variables in simple code (VarTable in PKB)

std::vector<std::string> intVecToStringVec(std::vector<int> input) {
    std::vector<std::string> output;
    for (int i = 0; i < (int)input.size(); i++) {
        output.push_back(std::to_string((long long)input[i]));
    }
    return output;
}

std::vector<int> stringVecToIntVec(std::vector<std::string> input) {
    std::vector<int> output;
    bool error = false;
    for (int i = 0; i < (int)input.size(); i++) {
        std::istringstream convert(input[i]);
        int num = -1;
        if (!(convert >> num)) {
            error = true;
            break;
        }
        output.push_back(num);
    }
    if (error) {
        // std::cout << "String conversion error: cannot convert to int";
        return std::vector<int>();
    }
    return output;
}

QueryProcessor::QueryProcessor()
{
	queryTree = QueryTree();
	declarationTable = DeclarationTable();
    optiGraphs.push_back(QueryGraph());
    optiGraphs.push_back(QueryGraph());
    optiGraphs.push_back(QueryGraph());
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
    bool exit = false;

    while (currNode.getName().compare("declaration") == 0 && !exit) {
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
        else if (currNode.getValue().compare("if") == 0) {
            varType = DeclarationTable::if_;
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
        else if (currNode.getValue().compare("procedure") == 0) {
            varType = DeclarationTable::procedure_;
        }
        else if (currNode.getValue().compare("call") == 0) {
            varType = DeclarationTable::call_;
        }
        else if (currNode.getValue().compare("plus") == 0) {
            varType = DeclarationTable::plus_;
        }
        else if (currNode.getValue().compare("minus") == 0) {
            varType = DeclarationTable::minus_;
        }
        else if (currNode.getValue().compare("times") == 0) {
            varType = DeclarationTable::times_;
        }
        else if (currNode.getValue().compare("stmtLst") == 0) {
            varType = DeclarationTable::stmtLst_;
        }

        std::vector<int> childHolder = currNode.getChildren();
        for (int i = 0; i < (int)childHolder.size(); i++) {
            declarationTable.insertDeclaration(varType, tree[childHolder[i]].getName());
        }
        *curr = *curr+1;
        if (*curr < (int)rootChildren.size())
            currNode = tree[rootChildren[*curr]];
        else
            exit = true;
    }
}

// Figure out type of a parameter 
// Note: return value of -1 means that parameter is either an entity or a number not an error, rather -2 is the error return 
int QueryProcessor::findTypeOf(std::string para, bool* paraIsNum, bool* paraIsEnt, bool* paraIsPlaceholder, int* paraNum) {
    *paraIsPlaceholder = *paraIsNum = *paraIsEnt = false;
    int paraType = declarationTable.getType(para);
    if (paraType == -1) {
        if (para.compare("_") == 0) {
            *paraIsPlaceholder = true;
        }
        else if (para.find("\"") != std::string::npos) {
            *paraIsEnt = true;
        }
        else {
            std::istringstream convert(para);
            if (convert >> *paraNum) {
                *paraIsNum = true;
            }
            else {  // Can't figure out type of para ?? Should never reach here
                // std::cout << "Cannot figure out type of " << para << std::endl;
                return -2;
            }
        }
    }
    return paraType;
}

// Evaluate sibling query
int QueryProcessor::evaluateSibling(bool para1IsNum, bool para2IsNum, std::string para1, std::string para2, int para1Num, int para2Num, int para1Type, int para2Type, PKB pkb) {
    std::vector<std::string> toStore;
    int ret;

    switch (para1Type) {
        case::DeclarationTable::assign_:
            para1Type = Node::assignNode;
            break;

        case::DeclarationTable::call_:
            para1Type = Node::callNode;
            break;

        case::DeclarationTable::constant_:
            para1Type = Node::constNode;
            break;

        case::DeclarationTable::if_:
            para1Type = Node::ifNode;
            break;

        case::DeclarationTable::minus_:
            para1Type = Node::minusNode;
            break;

        case::DeclarationTable::plus_:
            para1Type = Node::plusNode;
            break;

        case::DeclarationTable::procedure_:
            para1Type = Node::procedureNode;
            break;

        case::DeclarationTable::prog_line_:
            para1Type = Node::stmtSpecialValue;
            break;

        case::DeclarationTable::stmtLst_:
            para1Type = Node::stmtLstNode;
            break;

        case::DeclarationTable::stmt_:
            para1Type = Node::stmtSpecialValue;
            break;

        case::DeclarationTable::times_:
            para1Type = Node::timesNode;
            break;

        case::DeclarationTable::variable_:
            para1Type = Node::varNode;
            break;

        case::DeclarationTable::while_:
            para1Type = Node::whileNode;
            break;

        case -1:
            para1Type = Node::stmtSpecialValue;
            break;
    }

    switch (para2Type) {
        case::DeclarationTable::assign_:
            para2Type = Node::assignNode;
            break;

        case::DeclarationTable::call_:
            para2Type = Node::callNode;
            break;

        case::DeclarationTable::constant_:
            para2Type = Node::constNode;
            break;

        case::DeclarationTable::if_:
            para2Type = Node::ifNode;
            break;

        case::DeclarationTable::minus_:
            para2Type = Node::minusNode;
            break;

        case::DeclarationTable::plus_:
            para2Type = Node::plusNode;
            break;

        case::DeclarationTable::procedure_:
            para2Type = Node::procedureNode;
            break;

        case::DeclarationTable::prog_line_:
            para2Type = Node::stmtSpecialValue;
            break;

        case::DeclarationTable::stmtLst_:
            para2Type = Node::stmtLstNode;
            break;

        case::DeclarationTable::stmt_:
            para2Type = Node::stmtSpecialValue;
            break;

        case::DeclarationTable::times_:
            para2Type = Node::timesNode;
            break;

        case::DeclarationTable::variable_:
            para2Type = Node::varNode;
            break;

        case::DeclarationTable::while_:
            para2Type = Node::whileNode;
            break;
        
        case -1:
            para2Type = Node::stmtSpecialValue;
            break;
    }

    if (para1IsNum) {
        // Sibling (5, 7)
        if (para2IsNum) {
            if (!pkb.isSibling(para1, para1Type, para2, para2Type))
                return -1;
        }
        // Sibling (5, a)
        else {
            toStore = pkb.getSibling(para1, para1Type, para2Type);
            ret = resultStore.insertResult(para2, toStore);
            if (ret == -1)
                return -1;
        }
    }
    // Sibling(w, 5)
    else if (para2IsNum) {
        toStore = pkb.getSibling(para2, para2Type, para1Type);
        ret = resultStore.insertResult(para1, toStore);
        if (ret == -1)
            return -1;
    }
    // Double variable
    else {
        // Sibling(v, v)
        if (para1.compare(para2) == 0) {
            std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
            for (int i = 0; i < (int)para1ValString.size(); i++) {
                std::vector<std::string> holder = pkb.getSibling(para1ValString[i], para1Type, para1Type);
                bool found = false;
                for (int j = 0; j < (int)holder.size(); j++) {
                    if (holder[j].compare(para1ValString[i]) == 0) {
                        found = true;
                        break;
                    }
                }
                if (found)
                    toStore.push_back(para1ValString[i]);
            }
            ret = resultStore.insertResult(para1, toStore);
            if (ret == -1)
                return -1;
        }
        // Sibling(w, s)
        else {
            std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
            std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
            bool isPara1;
            if (para1ValString.size() < para2ValString.size()) {
                isPara1 = true;
			}
            else {
                isPara1 = false;
			}

            std::vector<std::vector<std::string>> toStoreTuple;
            if (isPara1) {
                for (int i = 0; i < (int)para1ValString.size(); i++) {
                    toStore = pkb.getSibling(para1ValString[i], para1Type, para2Type);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(para1ValString[i]);
                        holder.push_back(toStore[j]);
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            else {
                for (int i = 0; i < (int)para2ValString.size(); i++) {
                    toStore = pkb.getSibling(para2ValString[i], para2Type, para1Type);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(toStore[j]);
                        holder.push_back(para2ValString[i]);
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            ret = resultStore.insertResult(para1, para2, toStoreTuple);
            if (ret == -1)
                return -1;
        }
    }
    return 0;
}

// Evaluating contains and contains* query 
int QueryProcessor::evaluateContains(bool T, bool para1IsNum, bool para2IsNum, std::string para1, std::string para2, int para1Num, int para2Num, int para1Type, int para2Type, PKB pkb) {
    std::vector<std::string> toStore;
    int ret;

    switch (para1Type) {
        case::DeclarationTable::assign_:
            para1Type = Node::assignNode;
            break;

        case::DeclarationTable::call_:
            para1Type = Node::callNode;
            break;

        case::DeclarationTable::constant_:
            para1Type = Node::constNode;
            break;

        case::DeclarationTable::if_:
            para1Type = Node::ifNode;
            break;

        case::DeclarationTable::minus_:
            para1Type = Node::minusNode;
            break;

        case::DeclarationTable::plus_:
            para1Type = Node::plusNode;
            break;

        case::DeclarationTable::procedure_:
            para1Type = Node::procedureNode;
            break;

        case::DeclarationTable::prog_line_:
            para1Type = Node::stmtSpecialValue;
            break;

        case::DeclarationTable::stmtLst_:
            para1Type = Node::stmtLstNode;
            break;

        case::DeclarationTable::stmt_:
            para1Type = Node::stmtSpecialValue;
            break;

        case::DeclarationTable::times_:
            para1Type = Node::timesNode;
            break;

        case::DeclarationTable::variable_:
            para1Type = Node::varNode;
            break;

        case::DeclarationTable::while_:
            para1Type = Node::whileNode;
            break;

        case -1:
            para1Type = Node::stmtSpecialValue;
            break;
    }

    switch (para2Type) {
        case::DeclarationTable::assign_:
            para2Type = Node::assignNode;
            break;

        case::DeclarationTable::call_:
            para2Type = Node::callNode;
            break;

        case::DeclarationTable::constant_:
            para2Type = Node::constNode;
            break;

        case::DeclarationTable::if_:
            para2Type = Node::ifNode;
            break;

        case::DeclarationTable::minus_:
            para2Type = Node::minusNode;
            break;

        case::DeclarationTable::plus_:
            para2Type = Node::plusNode;
            break;

        case::DeclarationTable::procedure_:
            para2Type = Node::procedureNode;
            break;

        case::DeclarationTable::prog_line_:
            para2Type = Node::stmtSpecialValue;
            break;

        case::DeclarationTable::stmtLst_:
            para2Type = Node::stmtLstNode;
            break;

        case::DeclarationTable::stmt_:
            para2Type = Node::stmtSpecialValue;
            break;

        case::DeclarationTable::times_:
            para2Type = Node::timesNode;
            break;

        case::DeclarationTable::variable_:
            para2Type = Node::varNode;
            break;

        case::DeclarationTable::while_:
            para2Type = Node::whileNode;
            break;
        
        case -1:
            para2Type = Node::stmtSpecialValue;
            break;
    }

    if (!T) {
        if (para1IsNum) {
            // Contains (5, 7)
            if (para2IsNum) {
                if (!pkb.isContains(para1, para1Type, para2, para2Type))
                    return -1;
            }
            // Contains (5, a)
            else {
                toStore = pkb.getContainedIn(para1, para1Type, para2Type);
                ret = resultStore.insertResult(para2, toStore);
                if (ret == -1)
                    return -1;
            }
        }
        // Contains(w, 5)
        else if (para2IsNum) {
            toStore = pkb.getContainer(para2, para2Type, para1Type);
            ret = resultStore.insertResult(para1, toStore);
            if (ret == -1)
                return -1;
        }
        // Double variable
        else {
            // Contains(w, w)
            if (para1.compare(para2) == 0) {
                return -1;
            }
            // Contains(w, s)
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
				}
                else {
                    isPara1 = false;
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValString.size(); i++) {
                        toStore = pkb.getContainedIn(para1ValString[i], para1Type, para2Type);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValString.size(); i++) {
                        toStore = pkb.getContainer(para2ValString[i], para2Type, para1Type);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]);
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    else {
        if (para1IsNum) {
            // Contains*(5, 7)
            if (para2IsNum) {
                std::vector<std::string> holder = pkb.getContainedInT(para1, para1Type, para2Type);
                bool found = false;
                for (int i = 0; i < (int)holder.size(); i++) {
                    if (holder[i].compare(para2) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return -1;
            }
            // Contains*(5, a)
            else {
                toStore = pkb.getContainedInT(para1, para1Type, para2Type);
                ret = resultStore.insertResult(para2, toStore);
                if (ret == -1)
                    return -1;
            }
        }
        // Contains*(w, 5)
        else if (para2IsNum) {
            toStore = pkb.getContainerT(para2, para2Type, para1Type);
            ret = resultStore.insertResult(para1, toStore);
            if (ret == -1)
                return -1;
        }
        // Double variable
        else {
            // Contains*(w, w)
            if (para1.compare(para2) == 0) {
                return -1;
            }
            // Contains*(w, s)
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
				}
                else {
                    isPara1 = false;
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValString.size(); i++) {
                        toStore = pkb.getContainedInT(para1ValString[i], para1Type, para2Type);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValString.size(); i++) {
                        toStore = pkb.getContainerT(para2ValString[i], para2Type, para1Type);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]);
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    return 0;
}

// Evaluating follows and follows* query
int QueryProcessor::evaluateFollows(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    if (para1IsPlaceholder && para2IsPlaceholder) {
        if (pkb.getFollowsTable() -> getSize() > 0)
            return 0;
        else
            return -1;
    }
    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            if (para2IsNum) {
                if (!pkb.isFollow(para1Num, para2Num)) {  // Follows(num1, num2) is false, whole query is false
                    // std::cout << "Follows(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Follows(num1, num2) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                temp = pkb.getFollowedBy(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            else {
                temp = pkb.getFollowedBy(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            if (para1IsPlaceholder) {
                temp = pkb.getFollows(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            temp = pkb.getFollows(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            temp = pkb.getFollowedBy();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            temp = pkb.getFollows();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Follows(s1, s2)
            if (para1.compare(para2) == 0) {
                // Follows (s1, s1)           
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    if (pkb.isFollow(para1Val[i], para1Val[i])) {
                        temp.push_back(i);
                    }
                }
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {
                        temp = pkb.getFollowedBy(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {
                        temp = pkb.getFollows(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]);
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
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
                if (!found) {  // Follows*(num1, num2) is false, whole query is false
                    // std::cout << "Follows*(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Follows*(num1, num2) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                temp = pkb.getFollowedBy(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            else {
                temp = pkb.getFollowedByT(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            if (para2IsPlaceholder) {
                temp = pkb.getFollows(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            temp = pkb.getFollowsT(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            temp = pkb.getFollowedBy();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            temp = pkb.getFollows();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Follows*(s1, s2)
            if (para1.compare(para2) == 0) {
                // Follows* (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    bool found = false;
                    std::vector<int> temp2 = pkb.getFollowsT(para1Val[i]);
                    for (int j = 0; j < (int)temp2.size(); j++) {
                        if (para1Val[i] == temp2[j]) {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        temp.push_back(para1Val[i]);
                }
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {
                        temp = pkb.getFollowedByT(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {
                        temp = pkb.getFollowsT(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]);
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    return 0;
}

// Evaluating parent and parent* query
int QueryProcessor::evaluateParent(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    if (para1IsPlaceholder && para2IsPlaceholder) {
        if (pkb.getParentTable() -> getSize() > 0)
            return 0;
        else
            return -1;
    }
    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            if (para2IsNum) {
                if (!pkb.isParent(para1Num, para2Num)) {  // Parent(num1, num2) is false, whole query is false
                    // std::cout << "Parent(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Parent(num1, num2) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                temp = pkb.getChild(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            else {
                temp = pkb.getChild(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            if (para1IsPlaceholder) {
                temp = pkb.getParent(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            temp = pkb.getParent(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            temp = pkb.getChild();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            temp = pkb.getParent();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Parent(s1, s2)
            if (para1.compare(para2) == 0) {
                // Parent (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    if (pkb.isParent(para1Val[i], para1Val[i])) {
                        temp.push_back(i);
                    }
                }
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {
                        temp = pkb.getChild(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {
                        temp = pkb.getParent(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]); 
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    else {
        if (para1IsNum) {
            if (para2IsNum) {
                bool found = false;
                temp = pkb.getParentT(para2Num);
                for (int i = 0; i < (int)temp.size(); i++) {
                    if (para1Num == temp[i]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {  // Parent*(num1, num2) is false, whole query is false
                    // std::cout << "Parent*(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Parent*(num1, num2) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                temp = pkb.getChild(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            else {
                temp = pkb.getChildT(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            if (para1IsPlaceholder) {
                temp = pkb.getParent(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            temp = pkb.getParentT(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            temp = pkb.getChild();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            temp = pkb.getParent();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Parent*(s1, s2)
            if (para1.compare(para2) == 0) {
                // Parent* (s1, s1)
                std::vector<int> para2Val = stringVecToIntVec(resultStore.getValuesFor(para2));
                if (para2Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para2Val.size(); i++) {
                    bool found = false;
                    std::vector<int> temp2 = pkb.getParentT(para2Val[i]);
                    for (int j = 0; j < (int)temp2.size(); j++) {
                        if (para2Val[i] == temp2[j]) {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        temp.push_back(para2Val[i]);
                }
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {
                        temp = pkb.getChildT(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {
                        temp = pkb.getParentT(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]); 
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    return 0;
}

int QueryProcessor::evaluateNext(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    if (para1IsPlaceholder && para2IsPlaceholder) {
        if (pkb.getNumStmts() > 1)
            return 0;
        else
            return -1;
    }
    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            if (para2IsNum) {
                if (!pkb.isNext(para1Num, para2Num)) {  // Next(num1, num2) is false, whole query is false
                    // std::cout << "Next(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Next(num1, num2) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                temp = pkb.getNext(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            else {
                temp = pkb.getNext(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            if (para1IsPlaceholder) {
                temp = pkb.getPrev(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            temp = pkb.getPrev(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            temp = pkb.getNext();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            temp = pkb.getPrev();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Next(s1, s2)
            if (para1.compare(para2) == 0) {
                // Next (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    if (pkb.isNext(para1Val[i], para1Val[i])) {
                        temp.push_back(i);
                    }
                }
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {
                        temp = pkb.getNext(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {
                        temp = pkb.getPrev(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]); 
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    else {
        if (para1IsNum) {
            if (para2IsNum) {
                bool found = false;
                temp = pkb.getPrevT(para2Num);
                for (int i = 0; i < (int)temp.size(); i++) {
                    if (para1Num == temp[i]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {  // Next*(num1, num2) is false, whole query is false
                    // std::cout << "Next*(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Next*(num1, num2) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                temp = pkb.getNext(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            else {
                temp = pkb.getNextT(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            if (para1IsPlaceholder) {
                temp = pkb.getPrev(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            temp = pkb.getPrevT(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            temp = pkb.getNext();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            temp = pkb.getPrev();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Next*(s1, s2)
            if (para1.compare(para2) == 0) {
                // Next* (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    bool found = false;
                    std::vector<int> temp2 = pkb.getNextT(para1Val[i]);
                    for (int j = 0; j < (int)temp2.size(); j++) {
                        if (para1Val[i] == temp2[j]) {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        temp.push_back(para1Val[i]);
                }
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {
                        temp = pkb.getNextT(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {
                        temp = pkb.getPrevT(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]);
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    return 0;
}

int QueryProcessor::evaluateNextBip(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    if (para1IsPlaceholder && para2IsPlaceholder) {
        if (pkb.getNumStmts() > 1)
            return 0;
        else
            return -1;
    }
    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            if (para2IsNum) {
                if (!pkb.isNextBip(para1Num, para2Num)) {  // Next(num1, num2) is false, whole query is false
                    // std::cout << "Next(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Next(num1, num2) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                temp = pkb.getNextBip(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            else {
                temp = pkb.getNextBip(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            if (para1IsPlaceholder) {
                temp = pkb.getPrevBip(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            temp = pkb.getPrevBip(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            temp = pkb.getNextBip();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            temp = pkb.getPrevBip();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g NextBip(s1, s2)
            if (para1.compare(para2) == 0) {
                // NextBip (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    if (pkb.isNextBip(para1Val[i], para1Val[i])) {
                        temp.push_back(i);
                    }
                }
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {
                        temp = pkb.getNextBip(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {
                        temp = pkb.getPrevBip(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]); 
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    else {
        if (para1IsNum) {
            if (para2IsNum) {
                bool found = false;
                temp = pkb.getPrevBipT(para2Num);
                for (int i = 0; i < (int)temp.size(); i++) {
                    if (para1Num == temp[i]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {  // Next*(num1, num2) is false, whole query is false
                    // std::cout << "Next*(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Next*(num1, num2) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                temp = pkb.getNextBip(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            else {
                temp = pkb.getNextBipT(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            if (para1IsPlaceholder) {
                temp = pkb.getPrevBip(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            temp = pkb.getPrevBipT(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            temp = pkb.getNextBip();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            temp = pkb.getPrevBip();
            toStore = intVecToStringVec(temp);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g NextBip*(s1, s2)
            if (para1.compare(para2) == 0) {
                // NextBip* (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    bool found = false;
                    std::vector<int> temp2 = pkb.getNextBipT(para1Val[i]);
                    for (int j = 0; j < (int)temp2.size(); j++) {
                        if (para1Val[i] == temp2[j]) {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        temp.push_back(para1Val[i]);
                }
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {
                        temp = pkb.getNextBipT(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {
                        temp = pkb.getPrevBipT(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]);
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    return 0;
}

int QueryProcessor::evaluateAffects(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    if (para1IsPlaceholder && para2IsPlaceholder) {
        return 0;
    }
    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            // Affects(1, 10)
            if (para2IsNum) {
                temp = pkb.getAffectsStartAPI(para1Num);
                for (int i = 0; i < (int)temp.size(); i++) {
                    if (temp[i] == para2Num)
                        return 0;
                }
                return -1;
            }
            // Affects(1, _)
            else if (para2IsPlaceholder) {
                temp = pkb.getAffectsStartAPI(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            // Affects(1, a)
            else {
                temp = pkb.getAffectsStartAPI(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            // Affects(_, 10)
            if (para1IsPlaceholder) {
                temp = pkb.getAffectsEndAPI(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            // Affects(a, 10)
            else {
                temp = pkb.getAffectsEndAPI(para2Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        // Affects(_, a)
        else if (para1IsPlaceholder) {
            std::vector<int> temp2 = stringVecToIntVec(resultStore.getValuesFor(para2));
            for (int i = temp2.size()-1; i > -1; i--) {
                temp = pkb.getAffectsEndAPI(temp2[i]);
                if (temp.size() == 0)
                    temp2.erase(temp2.begin()+i);
            }
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        // Affects(a, _)
        else if (para2IsPlaceholder) {
            std::vector<int> temp2 = stringVecToIntVec(resultStore.getValuesFor(para1));
            for (int i = temp2.size()-1; i > -1; i--) {
                temp = pkb.getAffectsStartAPI(temp2[i]);
                if (temp.size() == 0)
                    temp2.erase(temp2.begin()+i);
            }
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Affects(a1, a2)
            if (para1.compare(para2) == 0) {
                // Affects (a1, a1)
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
				
				#ifndef ENABLE_THREADING
                for (int i = 0; i < (int)para1Val.size(); i++) {

                    /*
                    // Autotester timeout
                    if (i % 5 == 4) {
                        if (AbstractWrapper::GlobalStop) {
                            return -1;
                        } 
                    } */

                    std::vector<int> temp2;
                    temp2 = pkb.getAffectsStartAPI(para1Val[i]);
                    for (int j = 0; j < (int)temp2.size(); j++) {
                        if (temp2[j] == para1Val[i]) {
                            temp.push_back(para1Val[i]);
                            break;
                        }
                    }
                }
				#else
				Threading threading;
				if (!threading.processAffectsSameVarDriver(temp, para1Val, pkb)) return -1;
				#endif

                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            // Affects(a1, a2)
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;

				#ifndef ENABLE_THREADING
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {

                        /*
                        // Autotester timeout
                        if (i % 5 == 4) {
                            if (AbstractWrapper::GlobalStop) {
                                return -1;
                            } 
                        } */

                        temp = pkb.getAffectsStartAPI(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {

                        /*
                        // Autotester timeout
                        if (i % 5 == 4) {
                            if (AbstractWrapper::GlobalStop) {
                                return -1;
                            } 
                        } */

                        temp = pkb.getAffectsEndAPI(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]); 
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
				#else
				Threading threading;
				if(!threading.processAffectsDiffVarDriver(toStoreTuple, para1ValString, para1ValInt, para2ValString, para2ValInt, isPara1, pkb)) return -1;
				#endif

                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    else {
        if (para1IsNum) {
            // Affects*(1, 10)
            if (para2IsNum) {
                temp = pkb.getAffectsTStartAPI(para1Num);
                for (int i = 0; i < (int)temp.size(); i++) {
                    if (temp[i] == para2Num)
                        return 0;
                }
                return -1;
            }
            // Affects*(1, _)
            else if (para2IsPlaceholder) {
                temp = pkb.getAffectsTStartAPI(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            // Affects*(1, a)
            else {
                temp = pkb.getAffectsTStartAPI(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            // Affects*(_, 10)
            if (para1IsPlaceholder) {
                temp = pkb.getAffectsTEndAPI(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            // Affects*(a, 10)
            else {
                temp = pkb.getAffectsTEndAPI(para2Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        // Affects*(_, a)
        else if (para1IsPlaceholder) {
            std::vector<int> temp2 = stringVecToIntVec(resultStore.getValuesFor(para2));
            for (int i = temp2.size()-1; i > -1; i--) {
                temp = pkb.getAffectsTEndAPI(temp2[i]);
                if (temp.size() == 0)
                    temp2.erase(temp2.begin()+i);
            }
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        // Affects*(a, _)
        else if (para2IsPlaceholder) {
            std::vector<int> temp2 = stringVecToIntVec(resultStore.getValuesFor(para1));
            for (int i = temp2.size()-1; i > -1; i--) {
                temp = pkb.getAffectsTStartAPI(temp2[i]);
                if (temp.size() == 0)
                    temp2.erase(temp2.begin()+i);
            }
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Affects*(a1, a2)
            if (para1.compare(para2) == 0) {
                // Affects* (a1, a1)
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
				#ifndef ENABLE_THREADING
                for (int i = 0; i < (int)para1Val.size(); i++) {

                    /*
                    // Autotester timeout
                    if (i % 5 == 4) {
                        if (AbstractWrapper::GlobalStop) {
                            return -1;
                        } 
                    } */

                    std::vector<int> temp2;
                    temp2 = pkb.getAffectsTStartAPI(para1Val[i]);
                    for (int j = 0; j < (int)temp2.size(); j++) {
                        if (temp2[j] == para1Val[i]) {
                            temp.push_back(para1Val[i]);
                            break;
                        }
                    }
                }
				#else
				Threading threading;
				if(!threading.processAffectsTSameVarDriver(temp, para1Val, pkb)) return -1;
				#endif
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            // Affects*(a1, a2)
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;

				#ifndef ENABLE_THREADING
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {

                        /*
                        // Autotester timeout
                        if (i % 5 == 4) {
                            if (AbstractWrapper::GlobalStop) {
                                return -1;
                            } 
                        } */

                        temp = pkb.getAffectsTStartAPI(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {

                        /*
                        // Autotester timeout
                        if (i % 5 == 4) {
                            if (AbstractWrapper::GlobalStop) {
                                return -1;
                            } 
                        } */

                        temp = pkb.getAffectsTEndAPI(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]); 
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
				#else
				Threading threading;
				if(!threading.processAffectsTDiffVarDriver(toStoreTuple, para1ValString, para1ValInt, para2ValString, para2ValInt, isPara1, pkb)) return -1;
				#endif

                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    return 0;
}

int QueryProcessor::evaluateAffectsBip(bool T, bool para1IsNum, bool para1IsPlaceholder, bool para2IsNum, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    if (para1IsPlaceholder && para2IsPlaceholder) {
        return 0;
    }
    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            // AffectsBip(1, 10)
            if (para2IsNum) {
                temp = pkb.getAffectsBipStart(para1Num);
                for (int i = 0; i < (int)temp.size(); i++) {
                    if (temp[i] == para2Num)
                        return 0;
                }
                return -1;
            }
            // AffectsBip(1, _)
            else if (para2IsPlaceholder) {
                temp = pkb.getAffectsBipStart(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            // AffectsBip(1, a)
            else {
                temp = pkb.getAffectsBipStart(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            // AffectsBip(_, 10)
            if (para1IsPlaceholder) {
                temp = pkb.getAffectsBipEnd(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            // AffectsBip(a, 10)
            else {
                temp = pkb.getAffectsBipEnd(para2Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        // AffectsBip(_, a)
        else if (para1IsPlaceholder) {
            std::vector<int> temp2 = stringVecToIntVec(resultStore.getValuesFor(para2));
            for (int i = temp2.size()-1; i > -1; i--) {
                temp = pkb.getAffectsBipEnd(temp2[i]);
                if (temp.size() == 0)
                    temp2.erase(temp2.begin()+i);
            }
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        // AffectsBip(a, _)
        else if (para2IsPlaceholder) {
            std::vector<int> temp2 = stringVecToIntVec(resultStore.getValuesFor(para1));
            for (int i = temp2.size()-1; i > -1; i--) {
                temp = pkb.getAffectsBipStart(temp2[i]);
                if (temp.size() == 0)
                    temp2.erase(temp2.begin()+i);
            }
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        // Double variable
        else {
            // AffectsBip (a1, a1)
            if (para1.compare(para2) == 0) {
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
				
				#ifndef ENABLE_THREADING
                for (int i = 0; i < (int)para1Val.size(); i++) {

                    /*
                    // Autotester timeout
                    if (i % 5 == 4) {
                        if (AbstractWrapper::GlobalStop) {
                            return -1;
                        } 
                    } */

                    std::vector<int> temp2;
                    temp2 = pkb.getAffectsBipStart(para1Val[i]);
                    for (int j = 0; j < (int)temp2.size(); j++) {
                        if (temp2[j] == para1Val[i]) {
                            temp.push_back(para1Val[i]);
                            break;
                        }
                    }
                }
                
				#else
				Threading threading;
				if(!threading.processAffectsBipSameVarDriver(temp, para1Val, pkb)) return -1;
				#endif

                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            // AffectsBip(a1, a2)
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;

				#ifndef ENABLE_THREADING
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {

                        /*
                        // Autotester timeout
                        if (i % 5 == 4) {
                            if (AbstractWrapper::GlobalStop) {
                                return -1;
                            } 
                        } */

                        temp = pkb.getAffectsBipStart(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {

                        /*
                        // Autotester timeout
                        if (i % 5 == 4) {
                            if (AbstractWrapper::GlobalStop) {
                                return -1;
                            } 
                        } */

                        temp = pkb.getAffectsBipEnd(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]); 
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                
				#else
				Threading threading;
				if(!threading.processAffectsBipDiffVarDriver(toStoreTuple, para1ValString, para1ValInt, para2ValString, para2ValInt, isPara1, pkb)) return -1;
				#endif

                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    
    else {
        if (para1IsNum) {
            // AffectsBip*(1, 10)
            if (para2IsNum) {
                temp = pkb.getAffectsBipTStart(para1Num);
                for (int i = 0; i < (int)temp.size(); i++) {
                    if (temp[i] == para2Num)
                        return 0;
                }
                return -1;
            }
            // AffectsBip*(1, _)
            else if (para2IsPlaceholder) {
                temp = pkb.getAffectsBipTStart(para1Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            // AffectsBip*(1, a)
            else {
                temp = pkb.getAffectsBipTStart(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            // AffectsBip*(_, 10)
            if (para1IsPlaceholder) {
                temp = pkb.getAffectsBipTEnd(para2Num);
                if (temp.size() > 0)
                    return 0;
                else
                    return -1;
            }
            // AffectsBip*(a, 10)
            else {
                temp = pkb.getAffectsBipTEnd(para2Num);
                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        // AffectsBip*(_, a)
        else if (para1IsPlaceholder) {
            std::vector<int> temp2 = stringVecToIntVec(resultStore.getValuesFor(para2));
            for (int i = temp2.size()-1; i > -1; i--) {
                temp = pkb.getAffectsBipTEnd(temp2[i]);
                if (temp.size() == 0)
                    temp2.erase(temp2.begin()+i);
            }
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        // AffectsBip*(a, _)
        else if (para2IsPlaceholder) {
            std::vector<int> temp2 = stringVecToIntVec(resultStore.getValuesFor(para1));
            for (int i = temp2.size()-1; i > -1; i--) {
                temp = pkb.getAffectsBipTStart(temp2[i]);
                if (temp.size() == 0)
                    temp2.erase(temp2.begin()+i);
            }
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        // Double variable
        else {
            // AffectsBip* (a1, a1)
            if (para1.compare(para2) == 0) {
                std::vector<int> para1Val = stringVecToIntVec(resultStore.getValuesFor(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
			    #ifndef ENABLE_THREADING
                for (int i = 0; i < (int)para1Val.size(); i++) {

                    /*
                    // Autotester timeout
                    if (i % 5 == 4) {
                        if (AbstractWrapper::GlobalStop) {
                            return -1;
                        } 
                    } */

                    std::vector<int> temp2;
                    temp2 = pkb.getAffectsBipTStart(para1Val[i]);
                    for (int j = 0; j < (int)temp2.size(); j++) {
                        if (temp2[j] == para1Val[i]) {
                            temp.push_back(para1Val[i]);
                            break;
                        }
                    }
                }
                
                #else
				Threading threading;
				if (!threading.processAffectsBipTSameVarDriver(temp, para1Val, pkb)) return -1;
				#endif

                toStore = intVecToStringVec(temp);
                int ret = resultStore.insertResult(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            // Affects*(a1, a2)
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<int> para1ValInt;
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<int> para2ValInt;
                bool isPara1;
                if (para1ValString.size() < para2ValString.size()) {
                    isPara1 = true;
					para1ValInt = stringVecToIntVec(para1ValString);
				}
                else {
                    isPara1 = false;
					para2ValInt = stringVecToIntVec(para2ValString);
				}

                std::vector<std::vector<std::string>> toStoreTuple;

				#ifndef ENABLE_THREADING
                if (isPara1) {
                    for (int i = 0; i < (int)para1ValInt.size(); i++) {

                        /*
                        // Autotester timeout
                        if (i % 5 == 4) {
                            if (AbstractWrapper::GlobalStop) {
                                return -1;
                            } 
                        } */

                        temp = pkb.getAffectsBipTStart(para1ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]);
                            holder.push_back(toStore[j]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValInt.size(); i++) {

                        /*
                        // Autotester timeout
                        if (i % 5 == 4) {
                            if (AbstractWrapper::GlobalStop) {
                                return -1;
                            } 
                        } */

                        temp = pkb.getAffectsBipTEnd(para2ValInt[i]);
                        toStore = intVecToStringVec(temp);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]); 
                            holder.push_back(para2ValString[i]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                
				#else
				Threading threading;
				if (!threading.processAffectsBipTDiffVarDriver(toStoreTuple, para1ValString, para1ValInt, para2ValString, para2ValInt, isPara1, pkb)) return -1;
				#endif

                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    } 
    return 0;
}

// Evaluating modifies query, statements
int QueryProcessor::evaluateModifiesS(bool para1IsNum, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    // Inserting valid values based on PKB tables 
    if (para1IsNum) {
        if (para2IsEnt) {
            if (!pkb.isModifies(para1Num, para2)) {  // Modifies(num, ent) is false, whole query is false
                // std::cout << "Modifies(" << para1 << ",\"" << para2 << "\") is false" << std::endl;
                return -1;
            }
            // Modifies(num, ent) is true, do nothing
        }
        else if (para2IsPlaceholder) {
            toStore = pkb.getModifiedBy(para1Num);
            if (toStore.size() > 0)
                return 0;
            else
                return -1;
        }
        else {
            toStore = pkb.getModifiedBy(para1Num);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
    }
    else if (para2IsEnt) {
        temp = pkb.getModifiesVar(para2);
        toStore = intVecToStringVec(temp);
        int ret = resultStore.insertResult(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else if (para2IsPlaceholder) {
        temp = pkb.getModifiesVar();
        toStore = intVecToStringVec(temp);
        int ret = resultStore.insertResult(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else {
        // Double variable e.g Modifies(s1, v)
        if (para1.compare(para2) == 0) {
            // Modifies (s1, s1)
            // std::cout << "Parameters of Modifies(" << para1 << "," << para2 << ") are the wrong type\n";
            return -1;
        }
        else {
            std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
            std::vector<int> para1ValInt;
            std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
            std::vector<std::vector<std::string>> toStoreTuple;
            if (para1ValString.size() == 0 || para2ValString.size() == 0) {
                return -1;
            }
            bool isPara1;
            if (para1ValString.size() < para2ValString.size()) {
                isPara1 = true;
                para1ValInt = stringVecToIntVec(para1ValString);
            }
            else
                isPara1 = false;

            if (isPara1) {
                for (int i = 0; i < (int)para1ValInt.size(); i++) {
                    toStore = pkb.getModifiedBy(para1ValInt[i]);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(para1ValString[i]);
                        holder.push_back(toStore[j]); 
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            else {
                for (int i = 0; i < (int)para2ValString.size(); i++) {
                    temp = pkb.getModifiesVar(para2ValString[i]);
                    toStore = intVecToStringVec(temp);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(toStore[j]);
                        holder.push_back(para2ValString[i]);
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            int ret = resultStore.insertResult(para1, para2, toStoreTuple);
            if (ret == -1)
                return -1;
        }
    }
    return 0;
}

// Evaluating modifies query, procedure
int QueryProcessor::evaluateModifiesP(bool para1IsEnt, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    // Inserting valid values based on PKB tables 
    if (para1IsEnt) {
        if (para2IsEnt) {
            if (!pkb.isModifiesP(para1, para2)) {  // Modifies(ent, ent) is false, whole query is false
                // std::cout << "Modifies(" << para1 << ",\"" << para2 << "\") is false" << std::endl;
                return -1;
            }
            // Modifies(ent, ent) is true, do nothing
        }
        else if (para2IsPlaceholder) {
            toStore = pkb.getModifiedByP(para1);
            if (toStore.size() > 0)
                return 0;
            else
                return -1;
        }
        else {
            toStore = pkb.getModifiedByP(para1);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
    }
    else if (para2IsEnt) {
        toStore = pkb.getModifiesVarP(para2);
        int ret = resultStore.insertResult(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else if (para2IsPlaceholder) {
        toStore = pkb.getModifiesVarP();
        int ret = resultStore.insertResult(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else {
        // Double variable e.g Modifies(p, v)
        if (para1.compare(para2) == 0) {
            // Modifies (p, p)
            // std::cout << "Parameters of Modifies(" << para1 << "," << para2 << ") are the wrong type\n";
            return -1;
        }
        else {
            std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
            std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
            std::vector<std::vector<std::string>> toStoreTuple;
            if (para1ValString.size() == 0 || para2ValString.size() == 0) {
                return -1;
            }
            bool isPara1;
            if (para1ValString.size() < para2ValString.size())
                isPara1 = true;
            else
                isPara1 = false;

            if (isPara1) {
                for (int i = 0; i < (int)para1ValString.size(); i++) {
                    toStore = pkb.getModifiedByP(para1ValString[i]);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(para1ValString[i]);
                        holder.push_back(toStore[j]); 
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            else {
                for (int i = 0; i < (int)para2ValString.size(); i++) {
                    toStore = pkb.getModifiesVarP(para2ValString[i]);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(toStore[j]);
                        holder.push_back(para2ValString[i]); 
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            int ret = resultStore.insertResult(para1, para2, toStoreTuple);
            if (ret == -1)
                return -1;
        }
    }
    return 0;
}

// Evaluating uses query, statements
int QueryProcessor::evaluateUsesS(bool para1IsNum, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, int para1Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    // Inserting valid values based on PKB tables 
    if (para1IsNum) {
        if (para2IsEnt) {
            if (!pkb.isUses(para1Num, para2)) {  // Uses(num, ent) is false, whole query is false
                // std::cout << "Uses(" << para1 << ",\"" << para2 << "\") is false" << std::endl;
                return -1;
            }
            // Uses(num, ent) is true, do nothing
        }
        else if (para2IsPlaceholder) {
            toStore = pkb.getUsedBy(para1Num);
            if (toStore.size() > 0)
                return 0;
            else 
                return -1;
        }
        else {
            toStore = pkb.getUsedBy(para1Num);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
    }
    else if (para2IsEnt) {
        temp = pkb.getUsesVar(para2);
        toStore = intVecToStringVec(temp);
        int ret = resultStore.insertResult(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else if (para2IsPlaceholder) {
        temp = pkb.getUsesVar();
        toStore = intVecToStringVec(temp);
        int ret = resultStore.insertResult(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else {
        // Double variable e.g Uses(s1, v)
        if (para1.compare(para2) == 0) {
            // Uses (s1, s1)
            // std::cout << "Parameters of Uses(" << para1 << "," << para2 << ") are the wrong type\n";
            return -1;
        }
        else {
            std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
            std::vector<int> para1ValInt;
            std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
            std::vector<std::vector<std::string>> toStoreTuple;
            if (para1ValString.size() == 0 || para2ValString.size() == 0) {
                return -1;
            }
            bool isPara1;
            if (para1ValString.size() < para2ValString.size()) {
                isPara1 = true;
                para1ValInt = stringVecToIntVec(para1ValString);
            }
            else
                isPara1 = false;

            if (isPara1) {
                for (int i = 0; i < (int)para1ValInt.size(); i++) {
                    toStore = pkb.getUsedBy(para1ValInt[i]);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(para1ValString[i]); 
                        holder.push_back(toStore[j]);
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            else {
                for (int i = 0; i < (int)para2ValString.size(); i++) {
                    temp = pkb.getUsesVar(para2ValString[i]);
                    toStore = intVecToStringVec(temp);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(toStore[j]);
                        holder.push_back(para2ValString[i]);
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            int ret = resultStore.insertResult(para1, para2, toStoreTuple);
            if (ret == -1)
                return -1;
        }
    }
    return 0;
}

// Evaluating uses query, procedure
int QueryProcessor::evaluateUsesP(bool para1IsEnt, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    // Inserting valid values based on PKB tables 
    if (para1IsEnt) {
        if (para2IsEnt) {
            if (!pkb.isUsesP(para1, para2)) {  // Uses(ent, ent) is false, whole query is false
                // std::cout << "Uses(" << para1 << ",\"" << para2 << "\") is false" << std::endl;
                return -1;
            }
            // Uses(num, ent) is true, do nothing
        }
        else if (para2IsPlaceholder) {
            toStore = pkb.getUsedByP(para1);
            if (toStore.size() > 0)
                return 0;
            else 
                return -1;
        }
        else {
            toStore = pkb.getUsedByP(para1);
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
    }
    else if (para2IsEnt) {
        toStore = pkb.getUsesVarP(para2);
        int ret = resultStore.insertResult(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else if (para2IsPlaceholder) {
        toStore = pkb.getUsesVarP();
        int ret = resultStore.insertResult(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else {
        // Double variable e.g Uses(p, v)
        if (para1.compare(para2) == 0) {
            // Uses (p, p)
            // std::cout << "Parameters of Uses(" << para1 << "," << para2 << ") are the wrong type\n";
            return -1;
        }
        else {
            std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
            std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
            std::vector<std::vector<std::string>> toStoreTuple;
            if (para1ValString.size() == 0 || para2ValString.size() == 0) {
                return -1;
            }
            bool isPara1;
            if (para1ValString.size() < para2ValString.size())
                isPara1 = true;
            else
                isPara1 = false;

            if (isPara1) {
                for (int i = 0; i < (int)para1ValString.size(); i++) {
                    toStore = pkb.getUsedByP(para1ValString[i]);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder;
                        holder.push_back(para1ValString[i]); 
                        holder.push_back(toStore[j]);
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            else {
                for (int i = 0; i < (int)para2ValString.size(); i++) {
                    toStore = pkb.getUsesVarP(para2ValString[i]);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::vector<std::string> holder; 
                        holder.push_back(toStore[j]);
                        holder.push_back(para2ValString[i]);
                        toStoreTuple.push_back(holder);
                    }
                }
            }
            int ret = resultStore.insertResult(para1, para2, toStoreTuple);
            if (ret == -1)
                return -1;
        }
    }
    return 0;
}

// Evaluating calls query
int QueryProcessor::evaluateCalls(bool T, bool para1IsEnt, bool para1IsPlaceholder, bool para2IsEnt, bool para2IsPlaceholder, std::string para1, std::string para2, PKB pkb) {
    std::vector<std::string> toStore;

    if (para1IsPlaceholder && para2IsPlaceholder) {
        if (pkb.getCallsTable()->getSize() > 0)
            return 0;
        else
            return -1;
    }
    if (!T) {
        if (para1IsEnt) {
            if (para2IsEnt) {
                if (!pkb.isCalls(para1, para2)) {  
                    // std::cout << "Calls(" << para1 << ",\"" << para2 << "\") is false" << std::endl;
                    return -1;
                }
                // Calls(ent, ent) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                toStore = pkb.getCalledBy(para1);
                if (toStore.size() > 0)
                    return 0;
                else 
                    return -1;
            }
            else {
                toStore = pkb.getCalledBy(para1);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsEnt) {
            if (para1IsPlaceholder) {
                toStore = pkb.getCalls(para2);
                if (toStore.size() > 0)
                    return 0;
                else 
                    return -1;
            }
            toStore = pkb.getCalls(para2);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            toStore = pkb.getCalledBy();
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            toStore = pkb.getCalls();
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Calls(p1, p2)
            if (para1.compare(para2) == 0) {
                // Calls (p1, p1)
                return -1;
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<std::vector<std::string>> toStoreTuple;
                if (para1ValString.size() == 0 || para2ValString.size() == 0) {
                    return -1;
                }
                bool isPara1;
                if (para1ValString.size() < para2ValString.size())
                    isPara1 = true;
                else
                    isPara1 = false;

                if (isPara1) {
                    for (int i = 0; i < (int)para1ValString.size(); i++) {
                        toStore = pkb.getCalledBy(para1ValString[i]);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]); 
                            holder.push_back(toStore[j]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValString.size(); i++) {
                        toStore = pkb.getCalls(para2ValString[i]);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;              
                            holder.push_back(toStore[j]);
                            holder.push_back(para2ValString[i]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
        }
    }
    else {
        if (para1IsEnt) {
            if (para2IsEnt) {
                bool found = false;
                std::vector<std::string> temp2 = pkb.getCalledByT(para1);
                for (int i = 0; i < (int)temp2.size(); i++) {
                    if (para2.compare(temp2[i]) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) {  
                    // std::cout << "Calls*(" << para1 << ",\"" << para2 << "\") is false" << std::endl;
                    return -1;
                }
                // Calls*(ent, ent) is true, do nothing
            }
            else if (para2IsPlaceholder) {
                toStore = pkb.getCalledBy(para1);
                if (toStore.size() > 0)
                    return 0;
                else 
                    return -1;
            }
            else {
                toStore = pkb.getCalledByT(para1);
                int ret = resultStore.insertResult(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsEnt) {
            if (para1IsPlaceholder) {
                toStore = pkb.getCalls(para2);
                if (toStore.size() > 0)
                    return 0;
                else 
                    return -1;
            }
            toStore = pkb.getCallsT(para2);
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para1IsPlaceholder) {
            toStore = pkb.getCalledBy();
            int ret = resultStore.insertResult(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else if (para2IsPlaceholder) {
            toStore = pkb.getCalls();
            int ret = resultStore.insertResult(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Calls*(p1, p2)
            if (para1.compare(para2) == 0) {
                // Calls* (p1, p1)
                return -1;
            }
            else {
                std::vector<std::string> para1ValString = resultStore.getValuesFor(para1);
                std::vector<std::string> para2ValString = resultStore.getValuesFor(para2);
                std::vector<std::vector<std::string>> toStoreTuple;
                if (para1ValString.size() == 0 || para2ValString.size() == 0) {
                    return -1;
                }
                bool isPara1;
                if (para1ValString.size() < para2ValString.size())
                    isPara1 = true;
                else
                    isPara1 = false;

                if (isPara1) {
                    for (int i = 0; i < (int)para1ValString.size(); i++) {
                        toStore = pkb.getCalledByT(para1ValString[i]);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(para1ValString[i]); 
                            holder.push_back(toStore[j]);
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                else {
                    for (int i = 0; i < (int)para2ValString.size(); i++) {
                        toStore = pkb.getCallsT(para2ValString[i]);
                        for (int j = 0; j < (int)toStore.size(); j++) {
                            std::vector<std::string> holder;
                            holder.push_back(toStore[j]);
                            holder.push_back(para2ValString[i]); 
                            toStoreTuple.push_back(holder);
                        }
                    }
                }
                int ret = resultStore.insertResult(para1, para2, toStoreTuple);
                if (ret == -1)
                    return -1;
            }
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
            // std::cout << target << " is not declared" << std::endl;
            return -1;
        }

        case DeclarationTable::stmt_:
        case DeclarationTable::prog_line_:
        {
            int temp = pkb.getNumStmts();
            for (int i = 1; i <= temp; i++) {
                validStmtNum.push_back(i);
            }
            toStore = intVecToStringVec(validStmtNum);
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::assign_:
        {
            validStmtNum = pkb.getStmtWithType(Node::assignNode);
            toStore = intVecToStringVec(validStmtNum);
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::while_:
        {
            validStmtNum = pkb.getStmtWithType(Node::whileNode);
            toStore = intVecToStringVec(validStmtNum);
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::if_:
        {
            validStmtNum = pkb.getStmtWithType(Node::ifNode);
            toStore = intVecToStringVec(validStmtNum);
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::call_:
        {
            validStmtNum = pkb.getStmtWithType(Node::callNode);
            toStore = intVecToStringVec(validStmtNum);
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::variable_:
        {
            toStore = pkb.getVarTable();
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::constant_:
        {
            std::set<int> temp = pkb.getConstants();
            std::vector<int> temp2 (temp.begin(), temp.end());
            toStore = intVecToStringVec(temp2);
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::procedure_:
        {
            toStore = pkb.getAllProc();
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::minus_:
        {
            toStore = intVecToStringVec(pkb.getNodeIndexes(Node::minusNode));
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::plus_:
        {
            toStore = intVecToStringVec(pkb.getNodeIndexes(Node::plusNode));
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::times_:
        {
            toStore = intVecToStringVec(pkb.getNodeIndexes(Node::timesNode));
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }

        case DeclarationTable::stmtLst_:
        {
            toStore = intVecToStringVec(pkb.getNodeIndexes(Node::stmtLstNode));
            int ret = resultStore.insertResult(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }
    }
    return targetType;
}

/*
By vector index
0: 0 variables + with
1: 1 variable, not affects/next*
2: 2 variable, not affects/next*
3: 1 variable affects/next*
4: 2 variable affects/next*
5: 1 variable affects*
6: 2 variable affects*
7: 3/4 variable pattern
*/
std::vector<std::vector<int>> QueryProcessor::optimizeQuery(std::vector<QueryNode> tree, int curr, PKB pkb) {
    std::vector<int> rootChildren = tree[0].getChildren();
    std::vector<std::vector<int>> queryOrder;
    for (int i = 0; i < 8; i++) {
        std::vector<int> temp;
        queryOrder.push_back(temp);
    }
    
    for (int i = curr; i < (int)rootChildren.size(); i++) {
        QueryNode currNode = tree[rootChildren[i]];

        if (currNode.getName().compare("such that") == 0) {
            QueryNode relCon = tree[currNode.getChildren()[0]];
            std::vector<int> clause = relCon.getChildren();

            QueryNode relation = tree[clause[0]];
            int paraNode1 = relation.getChildren()[0];
            std::string para1 = tree[paraNode1].getName();
            int paraNode2 = relation.getChildren()[1];
            std::string para2 = tree[paraNode2].getName();

            // Figure out type of parameter 1 and 2
            bool para1IsNum = false, para2IsNum = false;
            bool para1IsEnt = false, para2IsEnt = false;
            bool para1IsPlaceholder = false, para2IsPlaceholder = false;
            int dummy;
            int para1Type = findTypeOf(para1, &para1IsNum, &para1IsEnt, &para1IsPlaceholder, &dummy);
            int para2Type = findTypeOf(para2, &para2IsNum, &para2IsEnt, &para2IsPlaceholder, &dummy);
            if (para1Type == -2 || para2Type == -2) {   // Cannot figure out parameter type
                return std::vector<std::vector<int>>();
            }

            int ret;
            if (para1Type != -1) {
                ret = evaluateType(pkb, para1);
                if (ret == -1) 
                    return std::vector<std::vector<int>>();
            }
            if (para2Type != -1) {
                ret = evaluateType(pkb, para2);
                if (ret == -1)
                    return std::vector<std::vector<int>>();
            }

            // 0 variables
            if (para1Type == -1 && para2Type == -1) {
                queryOrder[0].push_back(i);
            }
            // 1 variable
            else if (para1Type == -1 || para2Type == -1) {
                if (relation.getName().compare("affects") == 0 || relation.getName().compare("nextt") == 0) {
                    queryOrder[3].push_back(i);
                }
                else if (relation.getName().compare("affectst") == 0) {
                    queryOrder[5].push_back(i);
                }
                else {
                    queryOrder[1].push_back(i);
                }
            }
            // 2 variables
            else {
                if (relation.getName().compare("affects") == 0 || relation.getName().compare("nextt") == 0) {
                    queryOrder[4].push_back(i);
                    optiGraphs[1].insertQuery(para1, para2);
                }
                else if (relation.getName().compare("affectst") == 0) {
                    queryOrder[6].push_back(i);
                    optiGraphs[2].insertQuery(para1, para2);
                }
                else {
                    queryOrder[2].push_back(i);
                    optiGraphs[0].insertQuery(para1, para2);
                }
            }
        }

        else if (currNode.getName().compare("pattern") == 0) {
            QueryNode patternCond = tree[currNode.getChildren()[0]];
            std::vector<int> clause = patternCond.getChildren();
            
            QueryNode temp = tree[clause[0]];
            std::string para1 = tree[temp.getChildren()[0]].getName();
            std::string para2 = tree[temp.getChildren()[1]].getName();
            std::string para3 = tree[temp.getChildren()[2]].getName();
            std::string para4 = "";
            if ((int)temp.getChildren().size() > 3)
                para4 = tree[temp.getChildren()[3]].getName();

             // Figure out type of parameter 1 and 2
            bool para1IsNum = false, para2IsNum = false;
            bool para1IsEnt = false, para2IsEnt = false;
            bool para1IsPlaceholder = false, para2IsPlaceholder = false;
            int dummy;
            int para1Type = findTypeOf(para1, &para1IsNum, &para1IsEnt, &para1IsPlaceholder, &dummy);
            int para2Type = findTypeOf(para2, &para2IsNum, &para2IsEnt, &para2IsPlaceholder, &dummy);
            if (para1Type == -2 || para2Type == -2) {   // Cannot figure out parameter type
                return std::vector<std::vector<int>>();
            }

            int ret, varCount = 0;
            if (para1Type != -1) {
                varCount += 1;
                ret = evaluateType(pkb, para1);
                if (ret == -1) 
                    return std::vector<std::vector<int>>();
            }
            if (para2Type != -1) {
                varCount += 1;
                ret = evaluateType(pkb, para2);
                if (ret == -1)
                    return std::vector<std::vector<int>>();
            }
            bool SL1 = false, SL2 = false;
            if (para1Type != DeclarationTable::assign_) {
                int type = declarationTable.getType(para3);
                if (type == DeclarationTable::stmtLst_) {
                    varCount += 1;
                    SL1 = true;
                    ret = evaluateType(pkb, para3);
                    if (ret == -1)
                        return std::vector<std::vector<int>>();
                }
                type = declarationTable.getType(para4);
                if (type == DeclarationTable::stmtLst_) {
                    varCount += 1;
                    SL2 = true;
                    ret = evaluateType(pkb, para4);
                    if (ret == -1)
                        return std::vector<std::vector<int>>();
                }
            }

            // 0 variables
            if (varCount == 0) {
                queryOrder[0].push_back(i);
            }
            // 1 variable
            else if (varCount == 1) {
                queryOrder[1].push_back(i);
            }
            // 2 variables
            else if (varCount == 2) {
                queryOrder[2].push_back(i);
                if (!SL1 && !SL2)
                    optiGraphs[0].insertQuery(para1, para2);
                else if (SL1 && SL2)
                    optiGraphs[0].insertQuery(para3, para4);
                else if (SL1) {
                    if (para1Type != -1)
                        optiGraphs[0].insertQuery(para1, para3);
                    else
                        optiGraphs[0].insertQuery(para2, para3);
                }
                else {
                    if (para1Type != -1)
                        optiGraphs[0].insertQuery(para1, para4);
                    else
                        optiGraphs[0].insertQuery(para2, para4);
                }
            }
            else {
                queryOrder[7].push_back(i);
            }
        }

        else if (currNode.getName().compare("with") == 0) {
            queryOrder[0].push_back(i);
        }
    }
    return queryOrder;
}

// Walk our Query Tree and store results in result vector
void QueryProcessor::processQuery(PKB pkb) {
    // Reorganize query tree
    queryTree.reorgTree();

    std::vector<QueryNode> tree = queryTree.getTree();
    std::vector<int> rootChildren = tree[0].getChildren();   
    int curr = 0;

    // Inserting into declaration table
    loadDeclaration(tree, &curr);

    QueryNode currNode = tree[rootChildren[curr]];
    currNode = tree[currNode.getChildren()[0]];
    std::string target;
    std::vector<std::string> tupleTarget;
    bool isBool = false, isTuple = false;
    if (currNode.getName().compare("elem") == 0) {
        currNode = tree[currNode.getChildren()[0]];
        currNode = tree[currNode.getChildren()[0]];
        target = currNode.getName(); 
        if (target.compare("BOOLEAN") == 0) {
            isBool = true;
            result.push_back("false");
        }
        else if (target.compare("attrRef") == 0) {
            if (currNode.getChildren().size() == 2) {
                std::string holder;
                int temp;
                int ret = attrRefChecker(&target, &holder, &temp, currNode, pkb);
                if (ret == -1)
                    return;
            }
        }
        else {
            int ret = evaluateType(pkb, target);
            if (ret == -1)
                return;
        }
    }
    else if (currNode.getName().compare("tuple") == 0) {
        isTuple = true;
        std::vector<int> temp = currNode.getChildren();
        for (int i = 0; i < (int)temp.size(); i++) {
            currNode = tree[temp[i]];
            currNode = tree[currNode.getChildren()[0]];
            target = currNode.getName();
            if (target.compare("attrRef") == 0) {
                if (currNode.getChildren().size() == 2) {
                    std::string holder;
                    int temp;
                    int ret = attrRefChecker(&target, &holder, &temp, currNode, pkb);
                    if (ret == -1)
                        return;
                }
            }
            else {
                int ret = evaluateType(pkb, target);
                if (ret == -1)
                    return;
            }
            tupleTarget.push_back(target);
        }
    }
    else
        return;

    // Initial Query Optimizing
    std::vector<std::vector<int>> queryOrder = optimizeQuery(tree, curr+1, pkb);
    if (queryOrder.size() == 0)
        return;

    // Evaluating clauses
    for (int i = 0; i < (int)queryOrder.size(); i++) {
        std::unordered_set<int> queryChoices;
        int graphIndex;
		// Comment out when turning off dynamic optimization
        if (i == 2) 
            graphIndex = 0;
        else if (i == 4) 
            graphIndex = 1;
        else if (i == 6) 
            graphIndex = 2;
        if (i == 2 || i == 4 || i == 6)
            optiGraphs[graphIndex].setWeight(resultStore.getVVVector());
        // Dynamic optimization end
        
        while (queryOrder[i].size() > 0) {
            
            /*
            // Autotester timeout
            if (AbstractWrapper::GlobalStop) {
                return;
            } */

            int nextQuery;

			// To off dynamic optimization, change to if(true)
            if (i != 2 && i != 4 && i != 6) {
                nextQuery = queryOrder[i].back();
                queryOrder[i].pop_back();
            }
            else {
                if (queryChoices.size() == 0) {
                    int ret = optiGraphs[graphIndex].findMinWeight();
                    if (ret != -1)
                        queryChoices.insert(ret);
                    else    
                        break;  // Exit while loop
                }

                int min = *queryChoices.begin();
                for (auto it = queryChoices.begin(); it != queryChoices.end(); ++it) {
                    if (optiGraphs[graphIndex].graph[*it].weight < optiGraphs[graphIndex].graph[min].weight)
                        min = *it;
                }
                nextQuery = queryOrder[i][min];
                std::vector<int> connect = optiGraphs[graphIndex].graph[min].connections;
                for (int j = 0; j < (int)connect.size(); j++) {
                    if (!optiGraphs[graphIndex].graph[connect[j]].evaluated) {
                        queryChoices.insert(connect[j]);
                    }
                }
                queryChoices.erase(min);
                optiGraphs[graphIndex].graph[min].evaluated = true;
            }

            currNode = tree[rootChildren[nextQuery]];
            // Evaluating such that
            if (currNode.getName().compare("such that") == 0) {
                QueryNode relCon = tree[currNode.getChildren()[0]];
                std::vector<int> clause = relCon.getChildren();

                QueryNode relation = tree[clause[0]];
                int paraNode1 = relation.getChildren()[0];
                std::string para1 = tree[paraNode1].getName();
                int paraNode2 = relation.getChildren()[1];
                std::string para2 = tree[paraNode2].getName();

                // Figure out type of parameter 1 and 2
                bool para1IsNum = false, para2IsNum = false;
                bool para1IsEnt = false, para2IsEnt = false;
                bool para1IsPlaceholder = false, para2IsPlaceholder = false;
                int para1Num = -1, para2Num = -1;
                int para1Type = findTypeOf(para1, &para1IsNum, &para1IsEnt, &para1IsPlaceholder, &para1Num);
                int para2Type = findTypeOf(para2, &para2IsNum, &para2IsEnt, &para2IsPlaceholder, &para2Num);
                if (para1Type == -2 || para2Type == -2) {   // Cannot figure out parameter type
                    return;
                }
                    // Exit if a "constant" type variable is found (not allowed)
                if (para1Type == DeclarationTable::constant_ || para1Type == DeclarationTable::constant_) {
                    // std::cout << "Constant type not allowed in query\n";
                    return;
                }

                // Get rid of " " if parameters are entities
                if (para1IsEnt) {
                    para1 = para1.substr(1, para1.size()-2);
                }
                if (para2IsEnt) {
                    para2 = para2.substr(1, para2.size()-2);
                }

                // follows query
                if (relation.getName().compare("follows") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::call_ || para2Type == DeclarationTable::if_ || para2Type == DeclarationTable::while_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateFollows(false, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // follows* query
                else if (relation.getName().compare("followst") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {                
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::call_ || para2Type == DeclarationTable::if_ || para2Type == DeclarationTable::while_ || para2Type == DeclarationTable::stmt_) {   
                            int ret = evaluateFollows(true, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // parent query
                else if (relation.getName().compare("parent") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::call_ || para2Type == DeclarationTable::if_ || para2Type == DeclarationTable::while_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateParent(false, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // parent* query
                else if (relation.getName().compare("parentt") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::call_ || para2Type == DeclarationTable::if_ || para2Type == DeclarationTable::while_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateParent(true, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // modifies query, statements
                else if (relation.getName().compare("modifiess") == 0) {
                    if (para2IsEnt || para2IsPlaceholder || para2Type == DeclarationTable::variable_) {
                        if (para1IsNum || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {   
                            int ret = evaluateModifiesS(para1IsNum, para2IsEnt, para2IsPlaceholder, para1, para2, para1Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        // modifies query, procedures, parser misclassification
                        else if (para1Type == DeclarationTable::procedure_) {
                            int ret = evaluateModifiesP(para1IsEnt, para2IsEnt, para2IsPlaceholder, para1, para2, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // modifies query, procedures
                else if (relation.getName().compare("modifiesp") == 0) {
                    if (para1IsEnt) {                       
                        if (para2IsEnt || para2IsPlaceholder || para2Type == DeclarationTable::variable_) {
                            int ret = evaluateModifiesP(para1IsEnt, para2IsEnt, para2IsPlaceholder, para1, para2, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // uses query, statements
                else if (relation.getName().compare("usess") == 0) {
                    if (para2IsEnt || para2IsPlaceholder || para2Type == DeclarationTable::variable_) {
                        if (para1IsNum || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) { 
                            int ret = evaluateUsesS(para1IsNum, para2IsEnt, para2IsPlaceholder, para1, para2, para1Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        // uses query, procedures, parser misclassification
                        else if (para1Type == DeclarationTable::procedure_) {
                            int ret = evaluateUsesP(para1IsEnt, para2IsEnt, para2IsPlaceholder, para1, para2, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // uses query, procedures
                else if (relation.getName().compare("usesp") == 0) {
                    if (para1IsEnt) { 
                        if (para2IsEnt || para2IsPlaceholder || para2Type == DeclarationTable::variable_) {
                            int ret = evaluateUsesP(para1IsEnt, para2IsEnt, para2IsPlaceholder, para1, para2, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // calls query
                else if (relation.getName().compare("calls") == 0) {
                    if (para1IsEnt || para1IsPlaceholder || para1Type == DeclarationTable::procedure_) {
                        if (para2IsEnt || para2IsPlaceholder || para2Type == DeclarationTable::procedure_) {
                            int ret = evaluateCalls(false, para1IsEnt, para1IsPlaceholder, para2IsEnt, para2IsPlaceholder, para1, para2,pkb);
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // calls* query
                else if (relation.getName().compare("callst") == 0) {
                    if (para1IsEnt || para1IsPlaceholder || para1Type == DeclarationTable::procedure_) {
                        if (para2IsEnt || para2IsPlaceholder || para2Type == DeclarationTable::procedure_) {
                            int ret = evaluateCalls(true, para1IsEnt, para1IsPlaceholder, para2IsEnt, para2IsPlaceholder, para1, para2,pkb);
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // next query
                else if (relation.getName().compare("next") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::call_ || para2Type == DeclarationTable::if_ || para2Type == DeclarationTable::while_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateNext(false, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // next* query
                else if (relation.getName().compare("nextt") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::call_ || para2Type == DeclarationTable::if_ || para2Type == DeclarationTable::while_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateNext(true, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // nextBip query
                else if (relation.getName().compare("nextbip") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::call_ || para2Type == DeclarationTable::if_ || para2Type == DeclarationTable::while_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateNextBip(false, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // nextBip* query
                else if (relation.getName().compare("nextbipt") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::call_ || para1Type == DeclarationTable::if_ || para1Type == DeclarationTable::while_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::call_ || para2Type == DeclarationTable::if_ || para2Type == DeclarationTable::while_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateNextBip(true, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // affects query
                else if (relation.getName().compare("affects") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateAffects(false, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // affects* query
                else if (relation.getName().compare("affectst") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateAffects(true, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // affectsBip query
                else if (relation.getName().compare("affectsbip") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateAffectsBip(false, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // affectsBip* query
                else if (relation.getName().compare("affectsbipt") == 0) {
                    if (para1IsNum || para1IsPlaceholder || para1Type == DeclarationTable::assign_ || para1Type == DeclarationTable::prog_line_ || para1Type == DeclarationTable::stmt_) {                        
                        if (para2IsNum || para2IsPlaceholder || para2Type == DeclarationTable::assign_ || para2Type == DeclarationTable::prog_line_ || para2Type == DeclarationTable::stmt_) {
                            int ret = evaluateAffectsBip(true, para1IsNum, para1IsPlaceholder, para2IsNum, para2IsPlaceholder, para1, para2, para1Num, para2Num, pkb); 
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                    else
                        return;
                }
                // sibling query
                else if (relation.getName().compare("sibling") == 0) {
                    if (!para1IsPlaceholder && !para2IsPlaceholder && !para1IsEnt && !para2IsEnt) {
                        int ret = evaluateSibling(para1IsNum, para2IsNum, para1, para2, para1Num, para2Num, para1Type, para2Type, pkb);
                        if (ret == -1)
                            return;
                    }
                    else
                        return;
                }
                // contains query
                else if (relation.getName().compare("contains") == 0) {
                    if (!para1IsPlaceholder && !para2IsPlaceholder && !para1IsEnt && !para2IsEnt) {
                        int ret = evaluateContains(false, para1IsNum, para2IsNum, para1, para2, para1Num, para2Num, para1Type, para2Type, pkb);
                        if (ret == -1)
                            return;
                    }
                    else
                        return;
                }
                // contains* query
                else if (relation.getName().compare("containst") == 0) {
                    if (!para1IsPlaceholder && !para2IsPlaceholder && !para1IsEnt && !para2IsEnt) {
                        int ret = evaluateContains(true, para1IsNum, para2IsNum, para1, para2, para1Num, para2Num, para1Type, para2Type, pkb);
                        if (ret == -1)
                            return;
                    }
                    else
                        return;
                }

                if (i == 2 || i == 4 || i == 6) {
                    for (auto it = queryChoices.begin(); it != queryChoices.end(); ++it) {
                        optiGraphs[graphIndex].graph[*it].setWeight(para1, resultStore.getValuesFor(para1).size());
                        optiGraphs[graphIndex].graph[*it].setWeight(para2, resultStore.getValuesFor(para2).size());
                    }
                }
            }

            // Evaluates pattern query Eg. "pattern a1(x, _"y"_)" ===> a1 = pattern, x = var, _"y"_ = expr
		    // Eg. "pattern w1(x, _)" ===> w1 = pattern, x = var
		    // Eg. "pattern if1(x, _, _)" ===> if1 = pattern, x = var
            else if (currNode.getName().compare("pattern") == 0) {
                QueryNode patternCond = tree[currNode.getChildren()[0]];
                std::vector<int> clause = patternCond.getChildren();
        
                QueryNode temp = tree[clause[0]];
                std::string pattern = tree[temp.getChildren()[0]].getName();
                std::string var = tree[temp.getChildren()[1]].getName();
                bool hasStmtLst = false;
                std::string stmtLst1, stmtLst2;

                if (temp.getName().compare("pattern_assign_or_while_") == 0) {
                    temp = tree[temp.getChildren()[2]];
                    int expressionRoot = -1;
                    bool hasUnderscore = false;
                    if (temp.getName().compare("expr_with_underscore") == 0) {
                        expressionRoot = temp.getChildren()[0];
                        hasUnderscore = true;
                    }
                    else if (temp.getName().compare("expr_no_underscore") == 0) {
                        expressionRoot = temp.getChildren()[0];
                    }
                    else if (temp.getName().compare("_") == 0) {
                        hasUnderscore = true;
                    }
                    else {
                        hasStmtLst = true;
                        stmtLst1 = temp.getName();
                    }

                    // Figure out type of parameter 1 and 2
                    bool patternIsNum = false, varIsNum = false;
                    bool patternIsEnt = false, varIsEnt = false;
                    bool patternIsPlaceholder = false, varIsPlaceholder = false;
                    bool isWhile = false;
                    int patternNum = -1, varNum = -1;
                    int patternType = findTypeOf(pattern, &patternIsNum, &patternIsEnt, &patternIsPlaceholder, &patternNum);
                    int varType = findTypeOf(var, &varIsNum, &varIsEnt, &varIsPlaceholder, &varNum);
                    if (patternType == -2 || varType == -2) {   // Cannot figure out type
                        return;
                    }
                    
                    // Check that patternType is assign or while
                    if (patternType == DeclarationTable::while_) {
                        if (expressionRoot != -1) {
                            return;
                        }
                        else {
                            isWhile = true;
                        }
                    }
                    else if (patternType != DeclarationTable::assign_) {
                        return;
                    }

                    // Get rid of " " if var is entitity
                    if (varIsEnt) {
                        var = var.substr(1, var.size()-2);
                    }
                    // Return if var is the wrong type
                    else if (!varIsPlaceholder && varType != DeclarationTable::variable_) {
                        return;
                    }

                    if (!hasStmtLst) {
                        if (!isWhile) {
                            int ret = evaluateAssignPattern(pattern, var, expressionRoot, varIsEnt, varIsPlaceholder, hasUnderscore, pkb);
                            if (ret == -1)
                                return;
                        }
                        else {
                            int ret = evaluateIfWhilePattern(pattern, var, varIsEnt, varIsPlaceholder, pkb);
                            if (ret == -1)
                                return;
                        }
                    }
                    else {
                        if (declarationTable.getType(stmtLst1) == DeclarationTable::stmtLst_ && isWhile) {
                            int ret = evaluateWhilePatternStmtLst(pattern, var, varIsEnt, varIsPlaceholder, stmtLst1, pkb);
                            if (ret == -1)
                                return;
                        }
                        else
                            return;
                    }
                }
                else if (temp.getName().compare("pattern_if_") == 0) {
                    // Figure out type of parameter 1 and 2
                    bool patternIsNum = false, varIsNum = false;
                    bool patternIsEnt = false, varIsEnt = false;
                    bool patternIsPlaceholder = false, varIsPlaceholder = false;
                    int patternNum = -1, varNum = -1;
                    int patternType = findTypeOf(pattern, &patternIsNum, &patternIsEnt, &patternIsPlaceholder, &patternNum);
                    int varType = findTypeOf(var, &varIsNum, &varIsEnt, &varIsPlaceholder, &varNum);
                    if (patternType == -2 || varType == -2) {   // Cannot figure out type
                        return;
                    }

                    if (patternType != DeclarationTable::if_) {
                        return;
                    }

                    // Get rid of " " if var is entitity
                    if (varIsEnt) {
                        var = var.substr(1, var.size()-2);
                    }
                    // Return if var is the wrong type
                    else if (!varIsPlaceholder && varType != DeclarationTable::variable_) {
                        return;
                    }

                    stmtLst1 = tree[temp.getChildren()[2]].getName();
                    stmtLst2 = tree[temp.getChildren()[3]].getName();
                    bool SL1IsPlaceholder = true, SL2IsPlaceholder = true;
                    if (stmtLst1.compare("_") != 0) {
                        hasStmtLst = true;
                        SL1IsPlaceholder = false;
                    }
                    if (stmtLst2.compare("_") != 0) {
                        hasStmtLst = true;
                        SL2IsPlaceholder = false;
                    }

                    if (!hasStmtLst) {
                        int ret = evaluateIfWhilePattern(pattern, var, varIsEnt, varIsPlaceholder, pkb);
                        if (ret == -1)
                            return;
                    }
                    else {
                        int ret = evaluateIfPatternStmtLst(pattern, var, varIsEnt, varIsPlaceholder, stmtLst1, SL1IsPlaceholder, stmtLst2, SL2IsPlaceholder, pkb);
                        if (ret == -1)
                            return;
                    }
                }
                else {
                    // std::cout << "Unknown pattern format\n";
                    return;
                }

                if (i == 2) {
                    for (auto it = queryChoices.begin(); it != queryChoices.end(); ++it) {
                        optiGraphs[graphIndex].graph[*it].setWeight(pattern, resultStore.getValuesFor(pattern).size());
                        optiGraphs[graphIndex].graph[*it].setWeight(var, resultStore.getValuesFor(var).size());
                    }
                }
            }

            // Evaluates with query
            else if (currNode.getName().compare("with") == 0) {
                std::vector <std::string> toStore; 
                int ret;
                QueryNode attrCompare = tree[currNode.getChildren()[0]];
                std::vector<int> clause = attrCompare.getChildren();

                QueryNode temp = tree[clause[0]];
                if (temp.getName().compare("attrCompare_attrRef") == 0) {
                    std::string syno, attrName;
                    int synoType;
                    ret = attrRefChecker(&syno, &attrName, &synoType, tree[temp.getChildren()[0]], pkb);
                    if (ret == -1)
                        return;
                    temp = tree[temp.getChildren()[1]];
                    if (temp.getName().compare("ref_integer") == 0) {
                        if (synoType == DeclarationTable::variable_ || synoType == DeclarationTable::procedure_)
                            return;
                        std::string integerString = tree[temp.getChildren()[0]].getName(); 
                        int integerHolder;
                        bool dummy, isNum = false;
                        ret = findTypeOf(integerString, &isNum, &dummy, &dummy, &integerHolder);
                        if (ret == -2 || !isNum) {
                            // Error
                            return;
                        }
                        toStore.push_back(integerString);
                        ret = resultStore.insertResult(syno, toStore);
                        if (ret == -1)
                            return;
                    }
                    else if (temp.getName().compare("ref_ident") == 0) {
                        if (synoType != DeclarationTable::variable_ && synoType != DeclarationTable::procedure_)
                            return;
                        std::string varName = tree[temp.getChildren()[0]].getName();
                        toStore.push_back(varName);
                        ret = resultStore.insertResult(syno, toStore);
                        if (ret == -1)
                            return;
                    }
                    else if (temp.getName().compare("ref_attrRef") == 0) {
                        std::string syno2, attrName2;
                        int synoType2;
                        ret = attrRefChecker(&syno2, &attrName2, &synoType2, tree[temp.getChildren()[0]], pkb);
                        if (ret == -1)
                            return;

                        if (synoType == DeclarationTable::variable_ || synoType2 == DeclarationTable::variable_) {
                            if (!(synoType == DeclarationTable::variable_ && synoType2 == DeclarationTable::variable_))
                                return;
                        }
                        if (synoType == DeclarationTable::procedure_ || synoType2 == DeclarationTable::procedure_) {
                            if (!(synoType == DeclarationTable::procedure_ && synoType2 == DeclarationTable::procedure_))
                                return;
                        }

                        toStore = resultStore.getValuesFor(syno);
                        std::vector<std::vector<std::string>> toStoreTuple;
                        for (int i = 0; i < (int)toStore.size(); i++) {
                            std::vector<std::string> tempVec;
                            tempVec.push_back(toStore[i]);
                            tempVec.push_back(toStore[i]); 
                            toStoreTuple.push_back(tempVec);
                        } 
                        ret = resultStore.insertResult(syno, syno2, toStoreTuple);
                        if (ret == -1)
                            return;
                    }
                    else {
                        // Error
                        return;
                    }
                }
                else if (temp.getName().compare("attrCompare_synonym") == 0) {
                    std::string syno = (tree[temp.getChildren()[0]]).getName();
                    int synoType = evaluateType(pkb, syno);
                    if (synoType != DeclarationTable::prog_line_) {
                        // Error
                        return;
                    }
                    temp = tree[temp.getChildren()[1]];
                    if (temp.getName().compare("ref_pl_integer") == 0) {
                        std::string integerString = tree[temp.getChildren()[0]].getName(); 
                        int integerHolder;
                        bool dummy, isNum = false;
                        ret = findTypeOf(integerString, &isNum, &dummy, &dummy, &integerHolder);
                        if (ret == -2 || !isNum) {
                            // Error
                            return;
                        }
                        toStore.push_back(integerString);
                        ret = resultStore.insertResult(syno, toStore);
                        if (ret == -1)
                            return;
                    }
                    else if (temp.getName().compare("ref_pl_attrRef") == 0) {
                        std::string syno2, attrName2;
                        int synoType2;
                        ret = attrRefChecker(&syno2, &attrName2, &synoType2, tree[temp.getChildren()[0]], pkb);
                        if (ret == -1)
                            return;
                        if (synoType2 == DeclarationTable::variable_ || synoType2 == DeclarationTable::procedure_)
                            return;
                        toStore = resultStore.getValuesFor(syno2);
                        ret = resultStore.insertResult(syno, toStore);
                        if (ret == -1)
                            return;
                    }
                }
                else {
                    // Error
                    return;
                }
            }
        }
    } 

    // This is the final step, inserting into result, please do not do any evaluation after this
    // Only insert into result here (besides the false at the start) as results will be cleared here
    result.clear(); 
    if (isBool) 
        result.push_back("true");
    else {
        if (!isTuple) {
            result = resultStore.getValuesFor(target);
            if (declarationTable.getType(target) == DeclarationTable::stmtLst_) {
                result = pkb.convertStmtLst(stringVecToIntVec(result));
            }
        }
        else {
            std::vector<std::vector<std::string>> holder = resultStore.getValuesFor(tupleTarget); 
            std::vector<int> stmtLstTarget;
            for (int i = 0; i < (int)tupleTarget.size(); i++) {
                if (declarationTable.getType(tupleTarget[i]) == DeclarationTable::stmtLst_)
                    stmtLstTarget.push_back(i);
            }
            for (int i = 0; i < (int)holder.size(); i++) {
                for (int j = 0; j < (int)stmtLstTarget.size(); j++) {
                    std::istringstream convert(holder[i][stmtLstTarget[j]]);
                    int temp = -1;
                    if (!(convert >> temp)) {
                        return;
                    }
                    std::vector<int> tempVec;
                    tempVec.push_back(temp);
                    holder[i][stmtLstTarget[j]] = pkb.convertStmtLst(tempVec)[0];
                }
            }
            for (int i = 0; i < (int)holder.size(); i++) {
                std::string tupleResult = holder[i][0];
                for (int j = 1; j < (int)holder[i].size(); j++) {
                    tupleResult = tupleResult + " " + holder[i][j];
                }
                result.push_back(tupleResult);
            }
        }
    }
}

int QueryProcessor::attrRefChecker(std::string* synonym, std::string* attrName, int* synoType, QueryNode attrRef, PKB pkb) {
    std::vector<QueryNode> tree = queryTree.getTree();
    *synonym = (tree[attrRef.getChildren()[0]]).getName();
    *synoType = evaluateType(pkb, *synonym);
    if (*synoType == DeclarationTable::prog_line_ || *synoType == -1) {
        // Error
        return -1;
    }

    *attrName = (tree[attrRef.getChildren()[1]]).getName();
    if (*synoType == DeclarationTable::assign_ || *synoType == DeclarationTable::if_ || *synoType == DeclarationTable::stmt_ || *synoType == DeclarationTable::while_ || *synoType == DeclarationTable::call_) {
        if ((*attrName).compare("stmt#") != 0) {
            // Error
            return -1;
        }
    }
    else if (*synoType == DeclarationTable::variable_) {
        if ((*attrName).compare("varName") != 0) {
            // Error
            return -1;
        }
    }
    else if (*synoType == DeclarationTable::constant_) {
        if ((*attrName).compare("value") != 0) {
            // Error
            return -1;
        }
    }
    else if (*synoType == DeclarationTable::procedure_) {
        if ((*attrName).compare("procName") != 0) {
            // Error
            return -1;
        }
    }
    else {
        // Error
        return -1;
    }
    return 0;
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

// Evaluates pattern query Eg. "pattern a1(x, _"y"_)" ===> a1 = pattern, x = var, _"y"_ = expr
// Eg. "pattern w1(x, _)" ===> w1 = pattern, x = var
// Eg. "pattern if1(x, _, _)" ===> if1 = pattern, x = var
int QueryProcessor::evaluateAssignPattern(std::string pattern, std::string var, int exprRoot, bool varIsEnt, bool varIsPlaceholder, bool hasUnderscore, PKB pkb)
{
    std::vector<QueryNode> tree = queryTree.getTree();
    int patternType = -1, varType = -1, ret;
    patternType = declarationTable.getType(pattern);
    if (!varIsEnt && !varIsPlaceholder) {
        varType = declarationTable.getType(var);
    } 

    std::vector<std::string> toStore;
    std::vector<std::vector<std::string>> toStoreTuple;
    std::vector<int> statements;
	
    if (patternType == DeclarationTable::assign_) {
        if (varIsEnt) {     // The var == "var", where var is in our simple source code
            statements = pkb.matchAssignPattern(var, tree, exprRoot, hasUnderscore);
            toStore = intVecToStringVec(statements);
            ret = resultStore.insertResult(pattern, toStore);
            if (ret == -1)
                return -1;  
        }
        else if (varIsPlaceholder) {
            std::vector<std::string> varList = pkb.getVarTable();
            std::set<int> temp;
            for (int i=0; i<(int)varList.size(); i++)
			{
				statements = pkb.matchAssignPattern(varList[i], tree, exprRoot, hasUnderscore);
                temp.insert(statements.begin(), statements.end());  // Set ensures no duplicates
			}
            statements.clear();
            statements.insert(statements.begin(), temp.begin(), temp.end());
            toStore = intVecToStringVec(statements);
            ret = resultStore.insertResult(pattern, toStore);
            if (ret == -1)
                return -1; 
        }
        else if (varType == DeclarationTable::variable_) {  
            std::vector<std::string> varList = resultStore.getValuesFor(var);
            for (int i=0; i<(int)varList.size(); i++)
			{
				statements = pkb.matchAssignPattern(varList[i], tree, exprRoot, hasUnderscore);
                toStore = intVecToStringVec(statements);
                for (int j = 0; j < (int)toStore.size(); j++) {
                    std::vector<std::string> temp;
                    temp.push_back(toStore[j]);
                    temp.push_back(varList[i]);
                    toStoreTuple.push_back(temp);
                }
			}
            int ret = resultStore.insertResult(pattern, var, toStoreTuple);
            if (ret == -1)
                return -1;
        }
        else {
            // std::cout << var << " is not declared as a variable, query cannot be evaluated/n"; 
            return -1;
        }
    }
    else {
        // std::cout << pattern << " is not declared as an assign, query cannot be evaluated/n"; 
        return -1;
    }
    return 0;
}

int QueryProcessor::evaluateIfWhilePattern(std::string pattern, std::string var, bool varIsEnt, bool varIsPlaceholder, PKB pkb) {
    int patternType = -1, varType = -1, ret;
    patternType = declarationTable.getType(pattern);
    if (!varIsEnt && !varIsPlaceholder) {
        varType = declarationTable.getType(var);
    }

    std::vector<std::string> toStore;
    std::vector<std::vector<std::string>> toStoreTuple;
    std::vector<int> statements;
	
    if (varIsEnt) {     // The var == "var", where var is in our simple source code
        if (patternType == DeclarationTable::while_)
            statements = pkb.matchIfWhilePattern(Node::whileNode, var);
        else
            statements = pkb.matchIfWhilePattern(Node::ifNode, var);
        toStore = intVecToStringVec(statements);
        ret = resultStore.insertResult(pattern, toStore);
        if (ret == -1)
            return -1;  
    }
    else if (varIsPlaceholder) {
        std::vector<std::string> varList = pkb.getVarTable();
        std::set<int> temp;
        for (int i=0; i<(int)varList.size(); i++)
		{
            if (patternType == DeclarationTable::while_)
			    statements = pkb.matchIfWhilePattern(Node::whileNode, varList[i]);
            else
                statements = pkb.matchIfWhilePattern(Node::ifNode, varList[i]);
            temp.insert(statements.begin(), statements.end());  // Set ensures no duplicates
		}
        statements.clear();
        statements.insert(statements.begin(), temp.begin(), temp.end());
        toStore = intVecToStringVec(statements);
        ret = resultStore.insertResult(pattern, toStore);
        if (ret == -1)
            return -1; 
    }
    else if (varType == DeclarationTable::variable_) {  
        std::vector<std::string> varList = resultStore.getValuesFor(var);
        for (int i=0; i<(int)varList.size(); i++)
		{
            if (patternType == DeclarationTable::while_)
			    statements = pkb.matchIfWhilePattern(Node::whileNode, varList[i]);
            else
                statements = pkb.matchIfWhilePattern(Node::ifNode, varList[i]);
            toStore = intVecToStringVec(statements);
            for (int j = 0; j < (int)toStore.size(); j++) {
                std::vector<std::string> temp;
                temp.push_back(toStore[j]);
                temp.push_back(varList[i]);
                toStoreTuple.push_back(temp);
            }
		}
        int ret = resultStore.insertResult(pattern, var, toStoreTuple);
        if (ret == -1)
            return -1;
    }
    else {
        // std::cout << var << " is not declared as a variable, query cannot be evaluated/n"; 
        return -1;
    }
    return 0;
}

int QueryProcessor::evaluateIfPatternStmtLst(std::string pattern, std::string var, bool varIsEnt, bool varIsPlaceholder, std::string stmtLst1, bool SL1IsPlaceholder, std::string stmtLst2, bool SL2IsPlaceholder, PKB pkb) {
    int varType = -1, ret;
    if (!varIsEnt && !varIsPlaceholder) {
        varType = declarationTable.getType(var);
    }
    if (stmtLst1.compare(stmtLst2) == 0) 
        return -1;

    std::vector<std::vector<std::string>> toStoreTuple;
    std::vector<int> statements;
	
    if (varIsEnt) {     // The var == "var", where var is in our simple source code
        statements = pkb.matchIfWhilePattern(Node::ifNode, var);
        toStoreTuple = pkb.addStmtLst(statements, !SL1IsPlaceholder, !SL2IsPlaceholder);
        if (SL1IsPlaceholder) {
            ret = resultStore.insertResult(pattern, stmtLst2, toStoreTuple);
            if (ret == -1)
                return -1; 
        }
        else if (SL2IsPlaceholder) {
            ret = resultStore.insertResult(pattern, stmtLst1, toStoreTuple);
            if (ret == -1)
                return -1; 
        }
        else {
            ret = resultStore.insertResult(pattern, stmtLst1, stmtLst2, toStoreTuple);
            if (ret == -1)
                return -1;
        }
    }
    else if (varIsPlaceholder) {
        std::vector<std::string> varList = pkb.getVarTable();
        std::set<int> temp;
        for (int i=0; i<(int)varList.size(); i++)
		{
            statements = pkb.matchIfWhilePattern(Node::ifNode, varList[i]);
            temp.insert(statements.begin(), statements.end());  // Set ensures no duplicates
		}
        statements.clear();
        statements.insert(statements.begin(), temp.begin(), temp.end());
        toStoreTuple = pkb.addStmtLst(statements, !SL1IsPlaceholder, !SL2IsPlaceholder);
        if (SL1IsPlaceholder) {
            ret = resultStore.insertResult(pattern, stmtLst2, toStoreTuple);
            if (ret == -1)
                return -1; 
        }
        else if (SL2IsPlaceholder) {
            ret = resultStore.insertResult(pattern, stmtLst1, toStoreTuple);
            if (ret == -1)
                return -1; 
        }
        else {
            ret = resultStore.insertResult(pattern, stmtLst1, stmtLst2, toStoreTuple);
            if (ret == -1)
                return -1;
        }
    }
    else if (varType == DeclarationTable::variable_) {  
        std::vector<std::string> varList = resultStore.getValuesFor(var);
        for (int i=0; i<(int)varList.size(); i++)
		{
            statements = pkb.matchIfWhilePattern(Node::ifNode, varList[i]);
            std::vector<std::vector<std::string>> holder = pkb.addStmtLst(statements, !SL1IsPlaceholder, !SL2IsPlaceholder);
            for (int j = 0; j < (int)holder.size(); j++) {
                holder[j].push_back(varList[i]);
                toStoreTuple.push_back(holder[j]);
            }
		}
        if (SL1IsPlaceholder) {
            ret = resultStore.insertResult(pattern, stmtLst2, var, toStoreTuple);
            if (ret == -1)
                return -1; 
        }
        else if (SL2IsPlaceholder) {
            ret = resultStore.insertResult(pattern, stmtLst1, var, toStoreTuple);
            if (ret == -1)
                return -1; 
        }
        else {
            ret = resultStore.insertResult(pattern, stmtLst1, stmtLst2, var, toStoreTuple);
            if (ret == -1)
                return -1;
        }
    }
    else {
        // std::cout << var << " is not declared as a variable, query cannot be evaluated/n"; 
        return -1;
    }
}

int QueryProcessor::evaluateWhilePatternStmtLst(std::string pattern, std::string var, bool varIsEnt, bool varIsPlaceholder, std::string stmtLst, PKB pkb) {
    int varType = -1, ret;
    if (!varIsEnt && !varIsPlaceholder) {
        varType = declarationTable.getType(var);
    }

    std::vector<std::vector<std::string>> toStoreTuple;
    std::vector<int> statements;
	
    if (varIsEnt) {     // The var == "var", where var is in our simple source code
        statements = pkb.matchIfWhilePattern(Node::whileNode, var);
        toStoreTuple = pkb.addStmtLst(statements, true, false);
        ret = resultStore.insertResult(pattern, stmtLst, toStoreTuple);
        if (ret == -1)
            return -1;  
    }
    else if (varIsPlaceholder) {
        std::vector<std::string> varList = pkb.getVarTable();
        std::set<int> temp;
        for (int i=0; i<(int)varList.size(); i++)
		{
			statements = pkb.matchIfWhilePattern(Node::whileNode, varList[i]);
            temp.insert(statements.begin(), statements.end());  // Set ensures no duplicates
		}
        statements.clear();
        statements.insert(statements.begin(), temp.begin(), temp.end());
        toStoreTuple = pkb.addStmtLst(statements, true, false);
        ret = resultStore.insertResult(pattern, stmtLst, toStoreTuple);
        if (ret == -1)
            return -1; 
    }
    else if (varType == DeclarationTable::variable_) {  
        std::vector<std::string> varList = resultStore.getValuesFor(var);
        for (int i=0; i<(int)varList.size(); i++)
		{
            statements = pkb.matchIfWhilePattern(Node::whileNode, varList[i]);
            std::vector<std::vector<std::string>> holder = pkb.addStmtLst(statements, true, false);
            for (int j = 0; j < (int)holder.size(); j++) {
                holder[j].push_back(varList[i]);
                toStoreTuple.push_back(holder[j]);
            }
		}
        int ret = resultStore.insertResult(pattern, stmtLst, var, toStoreTuple);
        if (ret == -1)
            return -1;
    }
    else {
        // std::cout << var << " is not declared as a variable, query cannot be evaluated/n"; 
        return -1;
    }
    return 0;
}

std::list<std::string> QueryProcessor::getResult() {
    std::list<std::string> temp (result.begin(), result.end());
    return temp;
}