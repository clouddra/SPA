#include "CFGNode.h"

CFGNode::CFGNode(int startStmt, int endStmt, std::vector<int> prevNodes){
	start = startStmt;
	end = endStmt;
	prev = prevNodes;
}

CFGNode::CFGNode(){
	start = -1;
	end = -1;
}

// This function below needs to be updated for compression
void CFGNode::addStmt(int stmt){
	if (start == -1)
	{
		start = stmt;
		end = stmt;
	}
	else
		end = stmt;
}

int CFGNode::getStart(){
	return start;
}

int CFGNode::getEnd(){
	return end;
}

std::vector<int> CFGNode::getNext(){
	return next;
}
std::vector<int> CFGNode::getPrev(){
	return prev;
}

void CFGNode::addNext(int nextNode){
	next.push_back(nextNode);
}
void CFGNode::addPrev(int prevNode){
	prev.push_back(prevNode);
}
