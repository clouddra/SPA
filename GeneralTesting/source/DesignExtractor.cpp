#ifndef DE_HEAD
#define DE_HEAD
#include "DesignExtractor.h"
#endif
//currently adds modifies & uses for statements, parent and follows relationship
DesignExtractor::DesignExtractor(PKB* pkb)
{
    _ast = pkb->getAST();
    _mt = pkb->getModifiesTable();
    _pt = pkb->getParentTable();
    _ft = pkb->getFollowsTable();
    _ut = pkb->getUsesTable();
	_ct = pkb->getCallsTable();
    _stmtt = pkb->getStmtNodeTable();
	_pkb = pkb;
}

DesignExtractor::DesignExtractor() 
{
}

void DesignExtractor::populateTables()
{
	for(int i = 0; i<_ast.getTree().size(); i++)
	{
		int parent = _ast.getNode(i).getParent();
		std::vector<int> children = _ast.getNode(i).getChildren();
		//if(_ast.getNode(i).getNodeType()==Node::procedureNode)
		//{
		//	checkChildrenCalls(i);
		//}
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
	}

	std::vector<int> assignNodes = _stmtt->getNodeWithType(3);
	for(int t = 0; t<assignNodes.size(); t++)
	{
		int tempNode = assignNodes[t];
		std::vector<int> childrenAssign = _ast.getNode(tempNode).getChildren();
		std::vector<int> parents = _pkb->getParentT(_ast.getNode(tempNode).getStmtNum());//get parent and indirect parents of assign node
		insertModifies(tempNode, childrenAssign[0]);//add the first child of the assign node to modifies table
		for(int w = 0; w<parents.size();w++)
		{				
			_mt->insertModifies(parents[w], _ast.getNode(childrenAssign[0]).getValue());//indirect parents modifies this variable too				
		}
		if(_ast.getNode(childrenAssign[1]).getNodeType()==Node::varNode)//if 2nd child is a variable
		{
			insertUses(tempNode,childrenAssign[1]);//add it to the uses table
			for(int v = 0; v<parents.size();v++)
			{
				_ut->insertUses(parents[v],_ast.getNode(childrenAssign[1]).getValue());//indirect parents use this variable too					
			}
		}
		else//if it isn't a variable
		{
			checkChildrenUses(childrenAssign[1], parents);
		}		
	}
	std::vector<int> controlNodes = _stmtt->getNodeWithType(4);//while node
	std::vector<int> temp = _stmtt->getNodeWithType(5);//if node
	for(int x=0;x<temp.size();x++)
	{
		controlNodes.push_back(temp[x]);
	}
	for(int y=0;y<controlNodes.size();y++)
	{
		std::vector<int> parentsIfWhile = _pkb->getParentT(_ast.getNode(y).getStmtNum());
		std::vector<int> childrenControl = _ast.getNode(controlNodes[y]).getChildren();
		insertUses(controlNodes[y], childrenControl[0]);
		for(int p = 0; p<parentsIfWhile.size();p++)
		{
			 _ut->insertUses(parentsIfWhile[p],_ast.getNode(childrenControl[0]).getValue());//indirect parents use this variable too					
		}
	}
	//std::vector<int> callNodes = _stmtt->getNodeWithType(6);//call node
	//for(int z=0;z<callNodes.size();z++)
	//{
	//	checkChildrenCalls(callNodes[z]);
	//}
	/* DO NOT DELETE
	//Need to loop through again because Modifies and Uses tables require the Parent* relationship which is computed in earlier loop
	for(int t = 0; t<_ast.getTree().size(); t++)
	{
		std::vector<int> childrenAssign = _ast.getNode(t).getChildren();		
		if(_ast.getNode(t).getNodeType()==Node::assignNode)//look for assign nodes
		{		
			std::vector<int> parents = _pkb->getParentT(_ast.getNode(t).getStmtNum());//get parent and indirect parents of assign node
			
			insertModifies(t, childrenAssign[0]);//add the first child of the assign node to modifies table
			for(int w = 0; w<parents.size();w++)
			{				
				_mt->insertModifies(parents[w], _ast.getNode(childrenAssign[0]).getValue());//indirect parents modifies this variable too				
			}
			if(_ast.getNode(childrenAssign[1]).getNodeType()==Node::varNode)//if 2nd child is a variable
			{
				insertUses(t,childrenAssign[1]);//add it to the uses table
				for(int v = 0; v<parents.size();v++)
				{
					 _ut->insertUses(parents[v],_ast.getNode(childrenAssign[1]).getValue());//indirect parents use this variable too					
				}
			}
			else//if it isn't a variable
			{
				checkChildrenUses(childrenAssign[1], parents);
			}
		}
		if(_ast.getNode(t).getNodeType()==Node::ifNode||_ast.getNode(t).getNodeType()==Node::whileNode)//child is a control variable
		{
			std::vector<int> parentsIfWhile = _pkb->getParentT(_ast.getNode(t).getStmtNum());
			insertUses(t, childrenAssign[0]);
			for(int p = 0; p<parentsIfWhile.size();p++)
			{
					 _ut->insertUses(parentsIfWhile[p],_ast.getNode(childrenAssign[0]).getValue());//indirect parents use this variable too					
			}
		}
	}*/
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
					_ut->insertUses(parents[n],_ast.getNode(children1[m]).getValue());					
				}				
			}
			checkChildrenUses(children1[m], parents);
		}
	}
}
 void DesignExtractor::checkChildrenCalls(int nodeIndex)
 {
	 std::cout<< "1" <<std::endl;
	 std::vector<int> children = _ast.getNode(nodeIndex).getChildren();
	 std::cout<< "2" <<std::endl;
	 if(children.size()>0)
	 {
		 std::cout<< "3" <<std::endl;
		 for(int m = 0;m<children.size();m++)
		 {
			 std::cout<< "4" <<std::endl;
			 if(_ast.getNode(children[m]).getNodeType()==Node::callNode)//check if each child is a procedure call
			 {
				 std::cout<< "5" <<std::endl;
				 std::cout<<"children[m]"<<children[m]<<std::endl;
				 insertCalls(nodeIndex, children[m]);//add it to Calls table
			 }
			 std::cout<< "6" <<std::endl;
			 checkChildrenCalls(children[m]);
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
 
 void DesignExtractor::insertCalls(int stmt1, int stmt2)
 {
	 Node n1=_ast.getNode(stmt1);
	 Node n2=_ast.getNode(stmt2);
	 _ct->insertCalls(_ast.getNode(stmt1).getValue(), _ast.getNode(stmt2).getValue());
 }