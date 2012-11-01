#ifndef QUERYTREE_HEAD
#define QUERYTREE_HEAD
#include "QueryTree.h"
#endif

QueryTree::QueryTree(){}

int QueryTree::insertNode(std::string _name, std::string _value, int _parent)
{
	QueryNode temp = QueryNode(_name, _value);
	tree.push_back(temp);
	int newNodeIndex = tree.size() - 1;
	tree[newNodeIndex].setParent(_parent);

	if (_parent != -1)
	{
		tree[_parent].addChild(newNodeIndex);
	}

	return newNodeIndex;
}