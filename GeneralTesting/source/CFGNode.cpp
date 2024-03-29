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


CFGNode::CFGNode(int startStmt, int endStmt, int prevNode){
	start = startStmt;
	end = endStmt;
	prev.push_back(prevNode);
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

void CFGNode::addBipNext(int next){
	bipNext.push_back(next);
}

void CFGNode::addBipPrev(int prev){
	bipPrev.push_back(prev);
}

std::vector<int> CFGNode::getBipNext(){
	return bipNext;
}
std::vector<int> CFGNode::getBipPrev(){
	return bipPrev;
}

int CFGNode::getTop(){
	return top;
}

int CFGNode::getBot(){
	return bot;
}

void CFGNode::setBot(int b){
	bot = b;
}

void CFGNode::setTop(int t){
	top = t;
}