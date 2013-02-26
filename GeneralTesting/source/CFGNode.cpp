#include "CFGNode.h"

CFGNode::CFGNode(int startStmt, int endStmt, std::vector<int> prevNodes){
	start = startStmt;
	end = endStmt;
	prev = prevNodes;
	top = -1;

}

int CFGNode::getTop(){
	return top;

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

void CFGNode::setTop(int topNode){
	top = topNode;

}

void CFGNode::addNext(int nextNode){
	next.push_back(nextNode);
}
void CFGNode::addPrev(int prevNode){
	prev.push_back(prevNode);
}
