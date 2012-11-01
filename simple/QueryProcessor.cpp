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

void QueryProcessor::processQuery(std::string query)
{
	// Walk our Query Tree and store results in result vector
}

void QueryProcessor::printResult()
{
	for (int i=0; i<(int)(result.size()); i++)
		std::cout << result[i] << std::endl;
}