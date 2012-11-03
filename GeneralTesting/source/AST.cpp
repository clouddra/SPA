#ifndef AST_HEAD
#define AST_HEAD
#include "AST.h"
#endif

AST::AST() { }

int AST::insertNode(int nodeType, int value, int stmtNum, int parent) {
    Node temp = Node(nodeType, value, stmtNum);
    tree.push_back(temp);
    int newNode = tree.size() - 1;
    tree[newNode].setParent(parent);

    // Adds follow link
    if (parent != -1) {
        if (nodeType == Node::assignNode || nodeType == Node::whileNode || nodeType == Node::ifNode || nodeType == Node::callNode) {
            std::vector<int> children = tree[parent].getChildren();
            if (children.size() != 0) {
                int left = children.back();
                tree[left].setRight(tree.size() - 1);
                tree[newNode].setLeft(left);
            }
        }
        tree[parent].addChild(newNode);
    }

    return newNode;
}

std::vector<Node> AST::getTree() {
    return tree;
}

Node AST::getNode(int index)
{
	return tree[index];	
}