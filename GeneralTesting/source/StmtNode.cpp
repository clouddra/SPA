#include "StmtNode.h"

StmtNode::StmtNode(int ast, int cfg, int type ){
	astNode = ast;
	cfgNode = cfg;
	nodeType = type;
}

int StmtNode::getAST(){
	return astNode;

}
int StmtNode::getType(){
	return nodeType;
}

int StmtNode::getCFG(){
	return cfgNode;
}

void StmtNode::setAST(int ast){
	astNode = ast;
}
void StmtNode::setType(int type){
	nodeType = type;
}
void StmtNode::setCFG(int cfg){
	cfgNode = cfg;
}
