#include "CFGBip.h"
#include <queue>


std::vector<int> CFGBip::getNext(int stmt1, int nodeIndex, ProcTable procT){
	std::vector<int> stmtList;

	// current node is call, get the first statement of procedure called
	if (isBip(nodeIndex)){
		stmtList.push_back(procT.getProc(getBip(nodeIndex)).getFirstLine());
		return stmtList; 
	}


	// if fall in range
	if (stmt1>=cfg[nodeIndex].getStart() && stmt1 < cfg[nodeIndex].getEnd()) {
		stmtList.push_back(stmt1+1);
		return stmtList;
	}

	else {
		for (int i=0, nextNode; i<(int)cfg[nodeIndex].getNext().size(); i++) {
			nextNode = cfg[nodeIndex].getNext().at(i);
			int stmt = cfg[nextNode].getStart();
			if (stmt!=-1)
				stmtList.push_back(stmt);
		}
		return stmtList;
	}

}

bool CFGBip::isNext(int stmt1, int node1, int stmt2, ProcTable procT){
	std::vector<int> stmtList;

	if (isBip(node1)){
		int procStart = procT.getProc(getBip(node1)).getFirstLine();

		// if stmt2 is the starting statment of callee
		return (procStart==stmt2);
	}


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
std::vector<int> CFGBip::getNextT(int stmt1, int nodeIndex, ProcTable procT){
	std::unordered_set<int> stmtList;
	std::vector<bool> visited(cfg.size(), false);	//initialising cfg
	int nextNode;


	if (isBip(nodeIndex)){
		int test = procT.getProc(getBip(nodeIndex)).getCFGStart();
		getNextT(procT.getProc(getBip(nodeIndex)).getCFGStart(), stmtList, visited, procT);
		
	}
	
	// push statements of current node
	for (int i=stmt1+1; i<=cfg[nodeIndex].getEnd(); i++)
		stmtList.emplace(i);

	//visited[nodeIndex] = true;
	// push next nodes to nodesToVisit
	for (int i=0; i<(int)cfg[nodeIndex].getNext().size(); i++) {
		nextNode = cfg[nodeIndex].getNext().at(i);
		//nodesToVisit.push(nextNode);
		getNextT(nextNode, stmtList, visited, procT);
	}
	

	stmtList.erase(-1);
	std::vector<int> results(stmtList.cbegin(), stmtList.cend());
	return results;
}


void CFGBip::getNextT(int nodeIndex, std::unordered_set<int> &stmtList, std::vector<bool> &visited, ProcTable procT){

	int nextNode;
	CFGBipNode current = cfg[nodeIndex];

	if (visited[nodeIndex] == true || cfg[nodeIndex].getStart()==-1)
		return;

	visited[nodeIndex] = true;
	fillStmtInNode(stmtList, current);
	
	if (isBip(nodeIndex)){
		int test = procT.getProc(getBip(nodeIndex)).getCFGStart();
		getNextT(procT.getProc(getBip(nodeIndex)).getCFGStart(), stmtList, visited, procT);
	}
	for (int i=0; i<(int)cfg[nodeIndex].getNext().size(); i++) {
		nextNode = current.getNext().at(i);
		getNextT(nextNode, stmtList, visited, procT);
	}

}

void CFGBip::fillStmtInNode(std::unordered_set<int> &stmtList, CFGBipNode node){
	for (int i=node.getStart(); i<=node.getEnd(); i++)
		stmtList.emplace(i);

}

std::vector<int> CFGBip::getPrev(int stmt2, int nodeIndex, ProcTable procT){
	std::vector<int> stmtList;

	// if fall in range
	if (stmt2 > cfg[nodeIndex].getStart() && stmt2 <= cfg[nodeIndex].getEnd() && cfg[nodeIndex].getStart()!=-1) {
		stmtList.push_back(stmt2 - 1);
		return stmtList;
	}

	else {

		for (int i=0, prevNode; i<(int)cfg[nodeIndex].getNext().size(); i++) {

			prevNode = cfg[nodeIndex].getPrev().at(i);
			if (isBip(prevNode))
			{
				int procEnd = procT.getProc(getBip(prevNode)).getCFGEnd();
				return getPrev(-1, procEnd, procT); // find prev of dummyNode of variable called
			}
			int stmt = cfg[prevNode].getEnd();
			if (stmt!=-1)
				stmtList.push_back(stmt);
		}

		return stmtList;
	}

}

// Dummy, Yun Long please fill in
std::vector<int> CFGBip::getPrevT(int stmt2, int nodeIndex, ProcTable procT) {
    return std::vector<int>();
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

	// check this because it is faster. If not singleton node, means not calls, if -1 means dummy node
	if ((cfg[nodeIndex].getStart()!=cfg[nodeIndex].getEnd()) || cfg[nodeIndex].getStart()==-1)
		return false;

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