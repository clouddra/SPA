#include "CFG.h"

std::vector<int> CFG::getNext(int stmt1, int nodeIndex){
	std::vector<int> stmtList;
	
	// if fall in range
	if (stmt1>=cfg[nodeIndex].getStart() && stmt1 < cfg[nodeIndex].getEnd()) {
		stmtList.push_back(stmt1+1);
		return stmtList;
	}
	
	else {
		for (int i=0, nextNode; i<cfg[nodeIndex].getNext().size(); i++) {
			nextNode = cfg[nodeIndex].getNext().at(i);
			stmtList.push_back(cfg[nextNode].getStart());
		}

		return stmtList;
	}

}

std::vector<int> CFG::getPrev(int stmt2, int nodeIndex){
	std::vector<int> stmtList;
	
	// if fall in range
	if (stmt2 > cfg[nodeIndex].getStart() && stmt2 <= cfg[nodeIndex].getEnd()) {
		stmtList.push_back(stmt2 - 1);
		return stmtList;
	}
	
	else {
		for (int i=0, nextNode; i<cfg[nodeIndex].getNext().size(); i++) {
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
	for (int i=0; i<prev.size(); i++)
		cfg[prev[i]].addNext(newNode);

	return newNode;
}
