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

std::vector<QueryNode> QueryTree::getTree()
{
	return tree;
}

void QueryTree::reorgTree() {
    int curr = 0;
    QueryNode currNode = tree[curr];
    std::vector<int> rootChildren = currNode.getChildren();
    
    for (int i = 0; i < (int)rootChildren.size(); i++) {
        curr = rootChildren[i];
        currNode = tree[curr];
        std::string relation = currNode.getName();
        
        if (relation.compare("declaration") != 0 && relation.compare("result") != 0) {
            int cond = currNode.getChildren()[0];
            std::string condName = tree[cond].getName();
            std::vector<int> clauses = tree[cond].getChildren();

            for (int j = 1; j < (int)clauses.size(); j++) {
                // Adding another such that/pattern/with
                QueryNode temp = QueryNode(relation, "");
	            tree.push_back(temp);
	            int newNodeIndex = tree.size() - 1;
	            tree[newNodeIndex].setParent(0);
                tree[0].addChild(newNodeIndex);

                // Adding the cond below
                temp = QueryNode(condName, "");
	            tree.push_back(temp);
	            int newNodeIndex2 = tree.size() - 1;
	            tree[newNodeIndex2].setParent(newNodeIndex);
                tree[newNodeIndex].addChild(newNodeIndex2);

                // Attach and to new node
                tree[newNodeIndex2].addChild(clauses[j]);
                tree[clauses[j]].setParent(newNodeIndex2);
            }
            tree[cond].keep1Child();
        }
    }
}