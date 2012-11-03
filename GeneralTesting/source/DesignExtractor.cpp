#include "DesignExtractor.h"
//currently adds modifies & uses for statements, parent and follows relationship
DesignExtractor::DesignExtractor(PKB* pkb)
{
    _ast = pkb->getAST();
    _mt = pkb->getModifiesTable();
    _pt = pkb->getParentTable();
    _ft = pkb->getFollowsTable();
    _ut = pkb->getUsesTable();
    _stmtt = pkb->getStmtNodeTable();
	_pkb = pkb;
}

void DesignExtractor::populateTables()
{
	for(int i = 0; i<_ast.getTree().size(); i++)
	{
		int parent = _ast.getNode(i).getParent();
		std::vector<int> children = _ast.getNode(i).getChildren();
		if(_ast.getNode(i).getNodeType()==Node::stmtLstNode)//look for stmtLst nodes
		{			
			if(children.size()>1)//if the stmtLst node has more than one child
			{
				for(int j = 0; j < children.size()-1; j++)
				{
					int rightSibling = _ast.getNode(children[j]).getRight();
					insertFollows(children[j], rightSibling);//children have the follows relationship
			
				}
			}
			//i.e. if this stmtLst Node is a then:stmtLst or else:stmtLst or just a stmtLst which is a child of a while node
			if(_ast.getNode(parent).getNodeType()==Node::ifNode ||_ast.getNode(parent).getNodeType()==Node::whileNode)
			{
				for(int k = 0;k<children.size();k++)
				{
					insertParent(parent, children[k]);
				}
			}
		}
		else if(_ast.getNode(i).getNodeType()==Node::assignNode)//look for assign nodes
		{
			std::vector<int> parents = _pkb->getParentT(_ast.getNode(i).getStmtNum());//get parent and indirect parents of node
			insertModifies(i, children[0]);//add the first child of the assign node to modifies table
			for(int w = 0; w<parents.size();w++)
			{
				insertModifies(parents[w], children[0]);//indirect parents modify this variable too
			}
			if(_ast.getNode(children[1]).getNodeType()==Node::varNode)//if 2nd child is a variable
			{
				insertUses(i,children[1]);//add it to the uses table
				for(int v = 0; v<parents.size();v++)
				{
					insertUses(parents[v], children[1]);//indirect parents use this variable too
				}
			}
			else//if it isn't a variable
			{
				checkChildrenUses(children[1], parents);
			}
		}
	}
}

 void DesignExtractor::checkChildrenUses(int nodeIndex, std::vector<int> parents)
{
	std::vector<int> children1 = _ast.getNode(nodeIndex).getChildren();
	if(children1.size()>0)
	{					
		for(int m = 0;m<children1.size();m++)
		{
			if(_ast.getNode(children1[m]).getNodeType()==Node::varNode)//check if each child is a variable
			{
				insertUses(nodeIndex,children1[m]);//add it to the uses table
				for(int n = 0;n<parents.size();n++)
				{
					insertUses(parents[n],children1[m]);
				}
				checkChildrenUses(children1[m], parents);
			}
		}

	}
}

 void DesignExtractor::insertFollows(int stmt1, int stmt2)
 {
	 _ft->insertFollows(_ast.getNode(stmt1).getStmtNum(), _ast.getNode(stmt2).getStmtNum());
 }

 void DesignExtractor::insertParent(int stmt1, int stmt2)
 {
	 _pt->insertParent(_ast.getNode(stmt1).getStmtNum(), _ast.getNode(stmt2).getStmtNum());
 }

 void DesignExtractor::insertUses(int stmt1, int stmt2)
 {
	 _ut->insertUses(_ast.getNode(stmt1).getStmtNum(),_ast.getNode(stmt2).getValue());
 }

 void DesignExtractor::insertModifies(int stmt1, int stmt2)
 {
	 _mt->insertModifies(_ast.getNode(stmt1).getStmtNum(), _ast.getNode(stmt2).getValue());
 }