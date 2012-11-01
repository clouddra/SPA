#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

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

void QueryProcessor::processQuery()
{
	// Walk our Query Tree and store results in result vector

	// For now, try to see what is stored in the tree
	std::vector<QueryNode> temp = queryTree.getTree();
	for (int i=0; i<(int)temp.size(); i++)
		std::cout << "Name: " << temp[i].getName() << ", Value: " << temp[i].getValue() << std::endl;

}

void QueryProcessor::printResult()
{
	for (int i=0; i<(int)result.size(); i++)
		std::cout << result[i] << std::endl;
}