#ifndef NODE_HEAD
#define NODE_HEAD
#include "Node.h"
#endif

Node::Node(int type, int v) {
    nodeType = type;
    value = v;
    parent = leftSibling = rightSibling = -1;
}

int Node::getNodeType() {
    return nodeType;
}

int Node::getValue() {
    return value;
}

int Node::getParent() {
    return parent;
}

int Node::getLeft() {
    return leftSibling;
}

int Node::getRight() {
    return rightSibling;
}

std::vector<int> Node::getChildren() {
    return children;
}

void Node::setParent(int p) {
    parent = p;
}

void Node::setLeft(int l) {
    leftSibling = l;
}

void Node::setRight(int r) {
    rightSibling = r;
}

void Node::addChild(int c) {
    children.push_back(c);
}