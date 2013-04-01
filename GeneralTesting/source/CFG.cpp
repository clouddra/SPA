#include "CFG.h"
#include <queue>
#include <unordered_set>

std::vector<int> CFG::getNext(int stmt1, int nodeIndex){
	std::vector<int> stmtList;
	// if fall in range
	if (stmt1>=cfg[nodeIndex].getStart() && stmt1 < cfg[nodeIndex].getEnd()) {
		stmtList.push_back(stmt1+1);
	}

	else {
		std::vector<int> nextNodes = cfg[nodeIndex].getNext();
		for (std::vector<int>::iterator j = nextNodes.begin() ; j != nextNodes.end(); ++j) {
			// push only non dummy
			if (cfg[*j].getStart()!=-1)
				stmtList.push_back(cfg[*j].getStart());
		}
	}

	return stmtList;
}

std::vector<int> CFG::getNextBip(int stmt1, int nodeIndex){

	//if not last statement of node, do normal next
	std::vector<int> stmtList;
	if (stmt1>=cfg[nodeIndex].getStart() && stmt1 < cfg[nodeIndex].getEnd()) {
		stmtList.push_back(stmt1+1);
		return stmtList;
	}

	// if Bip, must return every procedure branched into
	std::vector<int> nextBip = cfg[nodeIndex].getBipNext();
	if (!nextBip.empty())
		getNextBip(stmt1, nodeIndex, stmtList);

	else {
		// if no Bip
		std::vector<int> nextNodes = cfg[nodeIndex].getNext();
		for (std::vector<int>::iterator j = nextNodes.begin() ; j != nextNodes.end(); ++j) {
			int nextStmt = cfg[*j].getStart();

			if (nextStmt!=-1)
				stmtList.push_back(nextStmt);
			
			// if dummy, we must do recursive bip
			else
				getNextBip(-1, nodeIndex, stmtList);
		}
	}

	return stmtList;
}

void CFG::getNextBip(int stmt1, int nodeIndex, std::vector<int> &stmtList){


	std::vector<int> nextBip = cfg[nodeIndex].getBipNext();

	for (std::vector<int>::iterator j = nextBip.begin() ; j != nextBip.end(); ++j) {
		int nextStmt = cfg[*j].getStart();

		if (nextStmt!=-1)
			stmtList.push_back(nextStmt);

		// recursive bip for dummy until we find valid node
		else
			getNextBip(-1, *j, stmtList);
	}

}

bool CFG::isNext(int stmt1, int node1, int stmt2){
	std::vector<int> stmtList;

	// if fall in range
	if (stmt1>=cfg[node1].getStart() && stmt1 < cfg[node1].getEnd()) {
		if (stmt1+1 == stmt2)
			return true;
	}

	else {
		std::vector<int> nextNodes = cfg[node1].getNext();
		for (std::vector<int>::iterator j = nextNodes.begin() ; j != nextNodes.end(); ++j) {
			if (cfg[*j].getStart() == stmt2)
				return true;
		}
	}

	return false;
}

bool CFG::isNextBip(int stmt1, int node1, int stmt2){
	std::vector<int> stmtList = getNextBip(stmt1, node1);
	std::vector<int>::iterator it = std::find(stmtList.begin(), stmtList.end(), stmt2);
	if (it != stmtList.end())
		return true;
	return false;
}




// non-recursive cause i hate recursion
std::vector<int> CFG::getNextT(int stmt1, int nodeIndex){
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
			fillStmtInNode(stmtList, cfg[nextNode]);

			// get next nodes to visit
			for (int i=0; i<(int)cfg[nextNode].getNext().size(); i++) {
				nodesToVisit.push(cfg[nextNode].getNext().at(i));
			}
		}

		else {
			//stmtList = fillStmtInNode(stmtList, cfg[nextNode]);
		}
	}

	stmtList.erase(-1);
	std::vector<int> results(stmtList.cbegin(), stmtList.cend());
	return results;
}


std::vector<int> CFG::getNextBipT(int stmt1, int nodeIndex){
	std::unordered_set<int> stmtList;
	std::vector<bool> visited(cfg.size(), false);	//initialising cfg

	// push statements of current node
	// For nodes with bip, only non-calls wil be affected since call is singleton, i.e end of procedure
	for (int i=stmt1+1; i<=cfg[nodeIndex].getEnd(); i++)
		stmtList.emplace(i);


	// if there is bip, we do recursive call
	std::vector<int> nextBip = cfg[nodeIndex].getBipNext();
	for (std::vector<int>::iterator j = nextBip.begin() ; j != nextBip.end(); ++j) {
		getNextBipT(*j, stmtList, visited, true);
	}


	// for normal next, no recusive return. Instead we branch to all possible callers

	std::vector<int> nextNodes = cfg[nodeIndex].getBipNext();
	for (std::vector<int>::iterator j = nextNodes.begin() ; j != nextNodes.end(); ++j) {
		getNextBipT(*j, stmtList, visited, false);
	}


	stmtList.erase(-1);
	std::vector<int> results(stmtList.cbegin(), stmtList.cend());
	return results;
}


void CFG::getNextBipT(int nodeIndex, std::unordered_set<int> &stmtList, std::vector<bool> &visited, bool recursive){

	// if visited or end of call
	if (visited[nodeIndex] == true)
		return;

	visited[nodeIndex] = true;

	// if we branched in from a call and reached the end, return
	if (cfg[nodeIndex].getStart()==-1 && recursive == true)
		return;

	// put statements of current node
	fillStmtInNode(stmtList, cfg[nodeIndex]);

	std::vector<int> nextBip = cfg[nodeIndex].getBipNext();
	for (std::vector<int>::iterator j = nextBip.begin() ; j != nextBip.end(); ++j) {

		// if we are at the end of node. Branching for non-calls
		if (cfg[nodeIndex].getStart()==-1)
			getNextBipT(*j, stmtList, visited, false);

		// Branching for calls
		else
			getNextBipT(*j, stmtList, visited, true);
	}

	// do normal next after branching, returning from function
	std::vector<int> nextNodes = cfg[nodeIndex].getBipNext();
	for (std::vector<int>::iterator j = nextNodes.begin() ; j != nextNodes.end(); ++j) {
		getNextBipT(*j, stmtList, visited, false);
	}

}


std::vector<int> CFG::getPrevBip(int stmt2, int nodeIndex){
    /*
int prevNode, prevBip;


// stmt is not at the first statement of node, do normal prev
if (stmt2 > cfg[nodeIndex].getStart() && stmt2 <= cfg[nodeIndex].getEnd() && stmt2!=-1) {
return getPrev(stmt2, nodeIndex);
}

else {
std::unordered_set<int> stmtList;
for (int i=0; i<(int)cfg[nodeIndex].getPrev().size(); i++) {
prevNode = cfg[nodeIndex].getPrev().at(i);
prevBip = cfg[prevNode].getBipEnd();

// if previous node is call then add previous nodes of dummy node in branched procedure
if (prevBip!=-1) {
std::vector<int> bipStmtList = getPrev(-1, prevBip);
stmtList.insert(bipStmtList.begin(), bipStmtList.end());
}

// if previous node is non-call, fill statements belonging to previous node
else {
fillStmtInNode(stmtList, cfg[prevNode]);
}		
}
std::vector<int> results(stmtList.cbegin(), stmtList.cend());
return results;
}
*/
    return std::vector<int>();
}


std::vector<int> CFG::getPrevBipT(int stmt2, int nodeIndex){
    /*
int prevNode, prevBip;
CFGNode current = cfg[nodeIndex];
std::unordered_set<int> stmtList;
std::vector<bool> visited(cfg.size(), false);	//initialising boolean vector

// push statements of current node
for (int i=stmt2-1; i>=cfg[nodeIndex].getStart(); i--)
stmtList.emplace(i);

for (int i=0; i<(int)cfg[nodeIndex].getPrev().size(); i++) {
prevNode = cfg[nodeIndex].getPrev().at(i);
prevBip = cfg[prevNode].getBipEnd();

// if previous node is call then recurse on end of branched procedure
if (prevBip!=-1) {
getPrevBipT(prevBip, stmtList, visited);
}

// if previous node is non-call, do recurse on previous node
else {
getPrevBipT(prevNode, stmtList, visited);
}		
}

stmtList.erase(-1);
std::vector<int> results(stmtList.cbegin(), stmtList.cend());
return results; */
    return std::vector<int>();
}


void CFG::getPrevBipT(int nodeIndex, std::unordered_set<int> &stmtList, std::vector<bool> &visited){
    /*
int prevNode, prevBip;
CFGNode current = cfg[nodeIndex];

if (visited[nodeIndex] == true)
return;

visited[nodeIndex] = true;
fillStmtInNode(stmtList, current);


for (int i=0; i<(int)cfg[nodeIndex].getPrev().size(); i++) {
prevNode = cfg[nodeIndex].getPrev().at(i);
prevBip = cfg[prevNode].getBipEnd();

// if previous node is call then recurse on end of branched procedure
if (prevBip!=-1) {
getPrevBipT(prevBip, stmtList, visited);
}

// if previous node is non-call, do recurse on previous node
else {
getPrevBipT(prevNode, stmtList, visited);
}		
}
*/
}




std::vector<int> CFG::getPrevT(int stmt2, int nodeIndex){
	std::unordered_set<int> stmtList;
	std::vector<bool> visited(cfg.size(), false);	//initialising cfg
	std::queue<int> nodesToVisit;
	int prevNode;


	// push statements of current node
	for (int i=stmt2-1; i>=cfg[nodeIndex].getStart(); i--)
		stmtList.emplace(i);

	//visited[nodeIndex] = true;
	// push next nodes to nodesToVisit
	for (int i=0; i<(int)cfg[nodeIndex].getPrev().size(); i++) {
		prevNode = cfg[nodeIndex].getPrev().at(i);
		nodesToVisit.push(prevNode);
	}

	while (!nodesToVisit.empty())
	{
		prevNode = nodesToVisit.front() ;
		nodesToVisit.pop();
		if (visited[prevNode] == false)
		{
			visited[prevNode]=true;
			// add statements to list
			fillStmtInNode(stmtList, cfg[prevNode]);

			// get next nodes to visit
			for (int i=0; i<(int)cfg[prevNode].getPrev().size(); i++) {
				nodesToVisit.push(cfg[prevNode].getPrev().at(i));
			}
		}

		else {
			//stmtList = fillStmtInNode(stmtList, cfg[nextNode]);
		}
	}

	stmtList.erase(-1);
	std::vector<int> results(stmtList.cbegin(), stmtList.cend());
	return results;
}



std::vector<int> CFG::getPrev(int stmt2, int nodeIndex){
	std::vector<int> stmtList;


	// if fall in range and not dummy
	if (stmt2 > cfg[nodeIndex].getStart() && stmt2 <= cfg[nodeIndex].getEnd() && stmt2!=-1) {
		stmtList.push_back(stmt2 - 1);
		return stmtList;
	}

	else {
		for (int i=0, nextNode; i<(int)cfg[nodeIndex].getPrev().size(); i++) {
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
	for (int i=0; i<(int)prev.size(); i++){
		cfg[prev[i]].addNext(newNode);
	}
	return newNode;
}

int CFG::insertCFGNode(int start, int end, int prev){
	CFGNode temp = CFGNode(start, end, prev);
	cfg.push_back(temp);
	int newNode = cfg.size() - 1;

	// find previous node and add index of new node to their next
	if (prev>0 && prev < (int)cfg.size())
		cfg[prev].addNext(newNode);

	return newNode;
}

int CFG::insertCFGNode(){
	CFGNode temp = CFGNode();
	cfg.push_back(temp);
	int newNode = cfg.size() - 1;
	return newNode;
}

void CFG::addStmt(int nodeIndex, int stmtNum)
{
	cfg[nodeIndex].addStmt(stmtNum);
}

void CFG::addNext(int nodeIndex, int nextIndex)
{
	cfg[nodeIndex].addNext(nextIndex);
}

void CFG::addPrev(int nodeIndex, int prevIndex)
{
	cfg[nodeIndex].addPrev(prevIndex);
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

/*
void CFG::setBipStart(int nodeIndex, int start){
cfg.at(nodeIndex).setBipStart(start);
}

void CFG::setBipEnd(int nodeIndex, int end){
cfg.at(nodeIndex).setBipEnd(end);
}
*/

void CFG::fillStmtInNode(std::unordered_set<int> &stmtList, CFGNode nextNode){
	for (int i=nextNode.getStart(); i<=nextNode.getEnd(); i++)
		stmtList.emplace(i);
}