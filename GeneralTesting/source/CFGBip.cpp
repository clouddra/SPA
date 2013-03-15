#include "CFGBip.h"
#include <queue>


std::vector<int> CFGBip::getNext(int stmt1, int nodeIndex){
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

bool CFGBip::isNext(int stmt1, int node1, int stmt2){
	std::vector<int> stmtList;

	// if fall in range
	if (stmt1>=cfg[node1].getStart() && stmt1 < cfg[node1].getEnd()) {
		if (stmt1+1 == stmt2)
			return true;
	}

	else {
		for (int i=0, nextNode; i<(int)cfg[node1].getNext().size(); i++) {
			nextNode = cfg[node1].getNext().at(i);
			if (cfg[nextNode].getStart() == stmt2)
				return true;
		}
	}

	return false;
}



// non-recursive cause i hate recursion
std::vector<int> CFGBip::getNextT(int stmt1, int nodeIndex){
	std::unordered_set<int> stmtList;
	std::vector<bool> visited(cfg.size(), false);	//initialising cfg
	std::queue<int> nodesToVisit;
	int nextNode;


	// push statements of current node
	for (int i=stmt1+1; i<=cfg[nodeIndex].getEnd(); i++)
		stmtList.emplace(i);

	//visited[nodeIndex] = true;
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
			for (int i=0; i<(int)cfg[nextNode].getNext().size(); i++) {
				nodesToVisit.push(cfg[nextNode].getNext().at(i));
			}
		}

		else {
			//stmtList = fillStmtInNode(stmtList, cfg[nextNode]);
		}
	}

	std::vector<int> results(stmtList.cbegin(), stmtList.cend());
	return results;
}


std::unordered_set<int> CFGBip::fillStmtInNode(std::unordered_set<int> stmtList, CFGBipNode nextNode){
	for (int i=nextNode.getStart(); i<=nextNode.getEnd(); i++)
		stmtList.emplace(i);

	return stmtList;
}

std::vector<int> CFGBip::getPrev(int stmt2, int nodeIndex){
	std::vector<int> stmtList;

	

	// if fall in range
	if (stmt2 > cfg[nodeIndex].getStart() && stmt2 <= cfg[nodeIndex].getEnd()) {
		stmtList.push_back(stmt2 - 1);
		return stmtList;
	}

	else {

		for( auto it = cfg[nodeIndex].getPrev().begin(); it != cfg[nodeIndex].getPrev().end(); ++it){
		}

		for (int i=0, nextNode; i<(int)cfg[nodeIndex].getNext().size(); i++) {
			nextNode = cfg[nodeIndex].getPrev().at(i);
			stmtList.push_back(cfg[nextNode].getEnd());
		}

		return stmtList;
	}

}

CFGBipNode CFGBip::getCFGNode(int nodeIndex){
	return cfg[nodeIndex];
}

std::vector<CFGBipNode> CFGBip::getCFG(){
	return cfg;
}


int CFGBip::insertCFGNode(int start, int end, std::vector<int> prev){
	CFGBipNode temp = CFGBipNode(start, end, prev);
	cfg.push_back(temp);
	int newNode = cfg.size() - 1;

	// find all previous nodes and add index of new node to their next
	for (int i=0; i<(int)prev.size(); i++){
		cfg[prev[i]].addNext(newNode);
	}
	return newNode;
}

int CFGBip::insertCFGNode(int start, int end, int prev){
	CFGBipNode temp = CFGBipNode(start, end, prev);
	cfg.push_back(temp);
	int newNode = cfg.size() - 1;

	// find previous node and add index of new node to their next
	if (prev>0 && prev < (int)cfg.size())
		cfg[prev].addNext(newNode);

	return newNode;
}

int CFGBip::insertCFGNode(){
	CFGBipNode temp = CFGBipNode();
	cfg.push_back(temp);
	int newNode = cfg.size() - 1;
	return newNode;
}

void CFGBip::addStmt(int nodeIndex, int stmtNum)
{
	cfg[nodeIndex].addStmt(stmtNum);
}

void CFGBip::addNext(int nodeIndex, int nextIndex)
{
	cfg[nodeIndex].addNext(nextIndex);
}

void CFGBip::addPrev(int nodeIndex, int prevIndex)
{
	cfg[nodeIndex].addPrev(prevIndex);
}

void CFGBip::addBip(int nodeIndex, int procIndex){
	bip[nodeIndex] = procIndex;
}

int CFGBip::getBip(int nodeIndex){
	if (isBip(nodeIndex))
		return bip[nodeIndex];

	return -1;
}

bool CFGBip::isBip(int nodeIndex){
	try{
		bip.at(nodeIndex);
	}catch (const std::out_of_range& oor) {
		return false;
	}
	return true;
}


// For debugging
void CFGBip::print()
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