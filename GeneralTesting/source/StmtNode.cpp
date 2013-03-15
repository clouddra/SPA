#include "StmtNode.h"

StmtNode::StmtNode(int ast, int cfg, int cfgBip, int type ){
	astNode = ast;
	cfgNode = cfg;
	cfgBipNode = cfgBip;
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

int StmtNode::getCFGBip(){
	return cfgBipNode;
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

void StmtNode::setCFGBip(int cfgBip){
	cfgBipNode=cfgBip;
}