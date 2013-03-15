#include "CFGBipNode.h"

CFGBipNode::CFGBipNode(int startStmt, int endStmt, std::vector<int> prevNodes){
	start = startStmt;
	end = endStmt;
	prev = prevNodes;
}

CFGBipNode::CFGBipNode(){
	start = -1;
	end = -1;
}


CFGBipNode::CFGBipNode(int startStmt, int endStmt, int prevNode){
	start = startStmt;
	end = endStmt;
	prev.push_back(prevNode);
}

// This function below needs to be updated for compression
void CFGBipNode::addStmt(int stmt){
	if (start == -1)
	{
		start = stmt;
		end = stmt;
	}
	else
		end = stmt;
}

int CFGBipNode::getStart(){
	return start;
}

int CFGBipNode::getEnd(){
	return end;
}

std::vector<int> CFGBipNode::getNext(){
	return next;
}
std::vector<int> CFGBipNode::getPrev(){
	return prev;
}

void CFGBipNode::addNext(int nextNode){
	next.push_back(nextNode);
}
void CFGBipNode::addPrev(int prevNode){
	prev.push_back(prevNode);
}

void CFGBipNode::setNext(std::vector<int> nextNodes){
	next = nextNodes;
}