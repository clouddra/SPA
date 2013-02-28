#include "CFG.h"
#include <queue>

std::vector<int> CFG::getNext(int stmt1, int nodeIndex){
	std::vector<int> stmtList;

	// if fall in range
	if (stmt1>=cfg[nodeIndex].getStart() && stmt1 < cfg[nodeIndex].getEnd()) {
		stmtList.push_back(stmt1+1);
		return stmtList;
	}

	else {
		for (int i=0, nextNode; i<(int)cfg[nodeIndex].getNext().size(); i++) {
			nextNode = cfg[nodeIndex].getNext().at(i);
			stmtList.push_back(cfg[nextNode].getStart());
		}

		return stmtList;
	}

}

// non-recursive cause i hate recursion
std::vector<int> CFG::getNextT(int stmt1, int nodeIndex){
	std::vector<int> stmtList;
	std::vector<bool> visited(cfg.size(), false);	//initialising cfg
	std::queue<int> nodesToVisit;
	int nextNode;


	// push statements of current node
	for (int i=stmt1+1; i<=cfg[nodeIndex].getEnd(); i++)
		stmtList.push_back(i);

	visited[nodeIndex] = true;
	// push next nodes to nodesToVisit
	for (int i=0; i<(int)cfg[nodeIndex].getNext().size(); i++) {
		nextNode = cfg[nodeIndex].getNext().at(i);
		nodesToVisit.push(nextNode);
	}

	while (!nodesToVisit.empty())
	{
		nextNode = nodesToVisit.front() ;
		nodesToVisit.pop();
		if (visited[nextNode] == false)
		{
			visited[nextNode]=true;
			// add statements to list
			stmtList = fillStmtInNode(stmtList, cfg[nextNode]);

			// get next nodes to visit
			for (int i=0; i<(int)cfg[nodeIndex].getNext().size(); i++) {
				nextNode = cfg[nodeIndex].getNext().at(i);
				nodesToVisit.push(nextNode);
			}
		}
	}

	return stmtList;
}


std::vector<int> CFG::fillStmtInNode(std::vector<int> stmtList, CFGNode nextNode){
	for (int i=nextNode.getStart(); i<=nextNode.getEnd(); i++)
		stmtList.push_back(i);

	return stmtList;
}

std::vector<int> CFG::getPrev(int stmt2, int nodeIndex){
	std::vector<int> stmtList;

	// if fall in range
	if (stmt2 > cfg[nodeIndex].getStart() && stmt2 <= cfg[nodeIndex].getEnd()) {
		stmtList.push_back(stmt2 - 1);
		return stmtList;
	}

	else {
		for (int i=0, nextNode; i<(int)cfg[nodeIndex].getNext().size(); i++) {
			nextNode = cfg[nodeIndex].getPrev().at(i);
			stmtList.push_back(cfg[nextNode].getEnd());
		}

		return stmtList;
	}

}

CFGNode CFG::getCFGNode(int nodeIndex){
	return cfg[nodeIndex];
}

std::vector<CFGNode> CFG::getCFG(){
	return cfg;
}


int CFG::insertCFGNode(int start, int end, std::vector<int> prev){
	CFGNode temp = CFGNode(start, end, prev);
	cfg.push_back(temp);
	int newNode = cfg.size() - 1;

	// find all previous nodes and add index of new node to their next
	for (int i=0; i<(int)prev.size(); i++)
		cfg[prev[i]].addNext(newNode);

	return newNode;
}

int CFG::insertCFGNode(){
	CFGNode temp = CFGNode();
	cfg.push_back(temp);
	int newNode = cfg.size() - 1;
	return newNode;
}

// For debugging
void CFG::print()
{
	for (int i=0; i<(int)cfg.size(); i++)
	{
		std::cout << i << " => Start: " << cfg[i].getStart() << ", End: " << cfg[i].getEnd() << std::endl;
		std::cout << "Next: ";
		for (int j=0; j<(int)cfg[i].getNext().size(); j++)
			std::cout << cfg[i].getNext()[j] << ", ";
		std::cout << std::endl;
		std::cout << "Prev: ";
		for (int j=0; j<(int)cfg[i].getPrev().size(); j++)
			std::cout << cfg[i].getPrev()[j] << ", ";
		std::cout << std::endl;
	}
}