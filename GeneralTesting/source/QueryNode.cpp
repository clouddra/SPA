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

void QueryNode::setParent(int _parent)
{
	parent = _parent;
}