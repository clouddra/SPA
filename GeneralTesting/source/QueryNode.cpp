#ifndef QUERYNODE_HEAD
#define QUERYNODE_HEAD
#include "QueryNode.h"
#endif

QueryNode::QueryNode(std::string _name, std::string _value)
{
	name = _name;
	value = _value;
}

std::string QueryNode::getName()
{
	return name;
}

std::string QueryNode::getValue()
{
	return value;
}

std::vector<int> QueryNode::getChildren()
{
	return children;
}

void QueryNode::addChild(int child)
{
	children.push_back(child);
}

void QueryNode::keep1Child()
{
    if (children.size() == 0)
        return;

	int temp = children[0];
    children.clear();
    children.push_back(temp);
}

void QueryNode::setParent(int _parent)
{
	parent = _parent;
}