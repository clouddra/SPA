#ifndef NODETABLE_HEAD
#define NODETABLE_HEAD
#include "NodeTable.h"
#endif

#ifndef NODE_HEAD
#define NODE_HEAD
#include "Node.h"
#endif

std::vector<int> NodeTable::getPlus() {
    return plus;
}

std::vector<int> NodeTable::getMinus() {
    return minus;
}

std::vector<int> NodeTable::getTimes() {
    return times;
}

std::vector<int> NodeTable::getDivide() {
    return divide;
}

std::vector<int> NodeTable::getConstants() {
    return constants;
}

std::vector<int> NodeTable::getProcs() {
    return procedures;
}

std::vector<int> NodeTable::getStmtLst() {
    return stmtLst;
}

std::vector<int> NodeTable::getVariables() {
    return variables;
}

void NodeTable::insert(int nodeIndex, int type) {
    if (type == Node::constNode)
        constants.push_back(nodeIndex);
    else if (type == Node::divideNode)
        divide.push_back(nodeIndex);
    else if (type == Node::minusNode)
        minus.push_back(nodeIndex);
    else if (type == Node::plusNode)
        plus.push_back(nodeIndex);
    else if (type == Node::timesNode)
        times.push_back(nodeIndex);
    else if (type == Node::stmtLstNode)
        stmtLst.push_back(nodeIndex);
    else if (type == Node::procedureNode)
        procedures.push_back(nodeIndex);
    else if (type == Node::varNode)
        variables.push_back(nodeIndex);
}