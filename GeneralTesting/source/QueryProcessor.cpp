#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

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
        std::cout << "String conversion error: cannot convert to int";
        return std::vector<int>();
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
            else {  // Can't figure out type of para ?? Should never reach here
                std::cout << "Cannot figure out type of " << para << std::endl;
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
    int ret;
    if (!para1IsNum) {
        ret = evaluateType(pkb, para1);
        if (ret == -1)
            return -1;
    }
    if (!para2IsNum) {
        int ret2 = evaluateType(pkb, para2);
        if (ret2 == -1)
            return -1;
    }

    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            if (para2IsNum) {
                if (!pkb.isFollow(para1Num, para2Num)) {  // Follows(num1, num2) is false, whole query is false
                    std::cout << "Follows(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Follows(num1, num2) is true, do nothing
            }
            else {
                temp = pkb.getFollowedBy(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            temp = pkb.getFollows(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = vvTable.insert(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Follows(s1, s2)
            if (para1.compare(para2) == 0) {
                // Follows (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(vvTable.getValues(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    if (pkb.isFollow(para1Val[i], para1Val[i])) {
                        temp.push_back(i);
                    }
                }
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = vvTable.getValues(para1);
                std::vector<int> para1ValInt = stringVecToIntVec(para1ValString);
                std::vector<std::pair<std::string,std::string>> toStoreTuple;
                if (para1ValInt.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1ValInt.size(); i++) {
                    temp = pkb.getFollowedBy(para1ValInt[i]);
                    toStore = intVecToStringVec(temp);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::pair<std::string, std::string> holder (para1ValString[i], toStore[j]);
                        toStoreTuple.push_back(holder);
                    }
                }
                vvTupleTable.insert(para1, para2, toStoreTuple);
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
                    std::cout << "Follows*(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Follows*(num1, num2) is true, do nothing
            }
            else {
                temp = pkb.getFollowedByT(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            temp = pkb.getFollowsT(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = vvTable.insert(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Follows*(s1, s2)
            if (para1.compare(para2) == 0) {
                // Follows* (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(vvTable.getValues(para1));
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
                        temp.push_back(i);
                }
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para1ValString = vvTable.getValues(para1);
                std::vector<int> para1ValInt = stringVecToIntVec(para1ValString);
                std::vector<std::pair<std::string,std::string>> toStoreTuple;
                if (para1ValInt.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1ValInt.size(); i++) {
                    temp = pkb.getFollowedByT(para1ValInt[i]);
                    toStore = intVecToStringVec(temp);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::pair<std::string, std::string> holder (para1ValString[i], toStore[j]);
                        toStoreTuple.push_back(holder);
                    }
                }
                vvTupleTable.insert(para1, para2, toStoreTuple);
            }
        }
    }
    return 0;
}

// Evaluating parent and parent* query
int QueryProcessor::evaluateParent(bool T, bool para1IsNum, bool para2IsNum, std::string para1, std::string para2, int para1Num, int para2Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    // Inserting valid values based on parameter type (if variable)
    int ret;
    if (!para1IsNum) {
        ret = evaluateType(pkb, para1);
        if (ret == -1)
            return -1;
    }
    if (!para2IsNum) {
        int ret2 = evaluateType(pkb, para2);
        if (ret2 == -1)
            return -1;
    }

    // Inserting valid values based on PKB tables 
    if (!T) {
        if (para1IsNum) {
            if (para2IsNum) {
                if (!pkb.isParent(para1Num, para2Num)) {  // Parent(num1, num2) is false, whole query is false
                    std::cout << "Parent(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Parent(num1, num2) is true, do nothing
            }
            else {
                temp = pkb.getChild(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            temp = pkb.getParent(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = vvTable.insert(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Parent(s1, s2)
            if (para1.compare(para2) == 0) {
                // Parent (s1, s1)
                std::vector<int> para1Val = stringVecToIntVec(vvTable.getValues(para1));
                if (para1Val.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para1Val.size(); i++) {
                    if (pkb.isParent(para1Val[i], para1Val[i])) {
                        temp.push_back(i);
                    }
                }
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para2ValString = vvTable.getValues(para2);
                std::vector<int> para2ValInt = stringVecToIntVec(para2ValString);
                std::vector<std::pair<std::string,std::string>> toStoreTuple;
                if (para2ValInt.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para2ValInt.size(); i++) {
                    temp = pkb.getParent(para2ValInt[i]);
                    toStore = intVecToStringVec(temp);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::pair<std::string, std::string> holder (toStore[j], para2ValString[i]);
                        toStoreTuple.push_back(holder);
                    }
                }
                vvTupleTable.insert(para1, para2, toStoreTuple);
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
                    std::cout << "Parent*(" << para1 << "," << para2 << ") is false" << std::endl;
                    return -1;
                }
                // Parent*(num1, num2) is true, do nothing
            }
            else {
                temp = pkb.getChildT(para1Num);
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para2, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
        }
        else if (para2IsNum) {
            temp = pkb.getParentT(para2Num);
            toStore = intVecToStringVec(temp);
            int ret = vvTable.insert(para1, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
        else {
            // Double variable e.g Parent*(s1, s2)
            if (para1.compare(para2) == 0) {
                // Parent* (s1, s1)
                std::vector<int> para2Val = stringVecToIntVec(vvTable.getValues(para2));
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
                        temp.push_back(i);
                }
                toStore = intVecToStringVec(temp);
                int ret = vvTable.insert(para1, toStore);
                if (ret == -1) {  // Exit cond
                    return -1;
                }
            }
            else {
                std::vector<std::string> para2ValString = vvTable.getValues(para2);
                std::vector<int> para2ValInt = stringVecToIntVec(para2ValString);
                std::vector<std::pair<std::string,std::string>> toStoreTuple;
                if (para2ValInt.size() == 0) {
                    return -1;
                }
                for (int i = 0; i < (int)para2ValInt.size(); i++) {
                    temp = pkb.getParentT(para2ValInt[i]);
                    toStore = intVecToStringVec(temp);
                    for (int j = 0; j < (int)toStore.size(); j++) {
                        std::pair<std::string, std::string> holder (toStore[j], para2ValString[i]);
                        toStoreTuple.push_back(holder);
                    }
                }
                vvTupleTable.insert(para1, para2, toStoreTuple);
            }
        }
    }
    return 0;
}

// Evaluating modifies query
int QueryProcessor::evaluateModifiesS(bool para1IsNum, bool para2IsEnt, std::string para1, std::string para2, int para1Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    // Inserting valid values based on parameter type (if variable)
    // Note that parameter 2 does not need to be evaluated since it will not restrict valid values
    int ret;
    if (!para1IsNum) {
        ret = evaluateType(pkb, para1);
        if (ret == -1)
            return -1;
    }

    // Inserting valid values based on PKB tables 
    if (para1IsNum) {
        if (para2IsEnt) {
            if (!pkb.isModifies(para1Num, para2)) {  // Modifies(num, ent) is false, whole query is false
                std::cout << "Modifies(" << para1 << ",\"" << para2 << "\") is false" << std::endl;
                return -1;
            }
            // Modifies(num, ent) is true, do nothing
        }
        else {
            toStore = pkb.getModifiedBy(para1Num);
            int ret = vvTable.insert(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
    }
    else if (para2IsEnt) {
        temp = pkb.getModifiesVar(para2);
        toStore = intVecToStringVec(temp);
        int ret = vvTable.insert(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else {
        // Double variable e.g Modifies(s1, v)
        if (para1.compare(para2) == 0) {
            // Modifies (s1, s1)
            std::cout << "Parameters of Modifies(" << para1 << "," << para2 << ") are the wrong type\n";
            return -1;
        }
        else {
            std::vector<std::string> para1ValString = vvTable.getValues(para1);
            std::vector<int> para1ValInt = stringVecToIntVec(para1ValString);
            std::vector<std::pair<std::string,std::string>> toStoreTuple;
            if (para1ValInt.size() == 0) {
                return -1;
            }
            for (int i = 0; i < (int)para1ValInt.size(); i++) {
                toStore = pkb.getModifiedBy(para1ValInt[i]);
                for (int j = 0; j < (int)toStore.size(); j++) {
                    std::pair<std::string, std::string> holder (para1ValString[i], toStore[j]);
                    toStoreTuple.push_back(holder);
                }
            }
            vvTupleTable.insert(para1, para2, toStoreTuple);
        }
    }
    return 0;
}

// Evaluating uses query
int QueryProcessor::evaluateUsesS(bool para1IsNum, bool para2IsEnt, std::string para1, std::string para2, int para1Num, PKB pkb) {
    std::vector<int> temp;
    std::vector<std::string> toStore;

    // Inserting valid values based on parameter type (if variable)
    // Note that parameter 2 does not need to be evaluated since it will not restrict valid values
    int ret;
    if (!para1IsNum) {
        ret = evaluateType(pkb, para1);
        if (ret == -1) 
            return -1;
    }

    // Inserting valid values based on PKB tables 
    if (para1IsNum) {
        if (para2IsEnt) {
            if (!pkb.isUses(para1Num, para2)) {  // Uses(num, ent) is false, whole query is false
                std::cout << "Uses(" << para1 << ",\"" << para2 << "\") is false" << std::endl;
                return -1;
            }
            // Uses(num, ent) is true, do nothing
        }
        else {
            toStore = pkb.getUsedBy(para1Num);
            int ret = vvTable.insert(para2, toStore);
            if (ret == -1) {  // Exit cond
                return -1;
            }
        }
    }
    else if (para2IsEnt) {
        temp = pkb.getUsesVar(para2);
        toStore = intVecToStringVec(temp);
        int ret = vvTable.insert(para1, toStore);
        if (ret == -1) {  // Exit cond
            return -1;
        }
    }
    else {
        // Double variable e.g Uses(s1, v)
        if (para1.compare(para2) == 0) {
            // Uses (s1, s1)
            std::cout << "Parameters of Uses(" << para1 << "," << para2 << ") are the wrong type\n";
            return -1;
        }
        else {
            std::vector<std::string> para1ValString = vvTable.getValues(para1);
            std::vector<int> para1ValInt = stringVecToIntVec(para1ValString);
            std::vector<std::pair<std::string,std::string>> toStoreTuple;
            if (para1ValInt.size() == 0) {
                return -1;
            }
            for (int i = 0; i < (int)para1ValInt.size(); i++) {
                toStore = pkb.getUsedBy(para1ValInt[i]);
                for (int j = 0; j < (int)toStore.size(); j++) {
                    std::pair<std::string, std::string> holder (para1ValString[i], toStore[j]);
                    toStoreTuple.push_back(holder);
                }
            }
            vvTupleTable.insert(para1, para2, toStoreTuple);
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
            std::cout << target << " is not declared" << std::endl;
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

        case DeclarationTable::variable_:
        {
            toStore = pkb.getVarTable();
            int ret = vvTable.insert(target, toStore);
            if (ret == -1)  // Exit cond
                return -1;
            break;
        }
    }
    return targetType;
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
            // Get rid of " " if parameters are entities
            if (para1IsEnt) {
                para1 = para1.substr(1, para1.size()-2);
            }
            if (para2IsEnt) {
                para2 = para2.substr(1, para2.size()-2);
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
            // parent query, assuming no entities
            if (relation.getName().compare("parent") == 0) {
                int ret = evaluateParent(false, para1IsNum, para2IsNum, para1, para2, para1Num, para2Num, pkb); 
                if (ret == -1)
                    return;
            }
            // parent* query, assuming no entities
            if (relation.getName().compare("parentt") == 0) {
                int ret = evaluateParent(true, para1IsNum, para2IsNum, para1, para2, para1Num, para2Num, pkb); 
                if (ret == -1)
                    return;
            }
            // modifies query, only doing for statements
            if (relation.getName().compare("modifiess") == 0) {
                int ret = evaluateModifiesS(para1IsNum, para2IsEnt, para1, para2, para1Num, pkb); 
                if (ret == -1)
                    return;
            }
            // uses query, only doing for statements
            if (relation.getName().compare("usess") == 0) {
                int ret = evaluateUsesS(para1IsNum, para2IsEnt, para1, para2, para1Num, pkb); 
                if (ret == -1)
                    return;
            }
        }
    } 

    // Evaluating select
    // Note that this step is actually only necessary if the target did not appear in the clauses, might want to have a flag for performance
    bool isBool = false;
    if (target.compare("BOOLEAN") == 0) 
        isBool = true;
    else {
        int ret = evaluateType(pkb, target);
        if (ret == -1)
            return;
    }

    // Reconcile the vvTuple in vvTupleTable
    int ret = vvTupleTable.reconcile();
    if (ret == -1)
        return;

    // "intersect" vvTupleTable with vvTable
    ret = vvTupleTable.restrictTo(vvTable);
    if (ret == -1)
        return;

    // Check if tuple will affect target variable
    std::vector<std::string> temp = vvTupleTable.getValues(target);
    if (temp.size() != 0) {     // if temp is empty it should mean that the tuple will not affect the target variable
        ret = vvTable.insert(target, temp);
        if (ret == -1)      // Note: this exit con should never be reached (logically)  
            return;
    }

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