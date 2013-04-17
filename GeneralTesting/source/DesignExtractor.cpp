#ifndef DE_HEAD
#define DE_HEAD
#include "DesignExtractor.h"
#endif
//currently adds modifies & uses, calls, parent and follows relationship
DesignExtractor::DesignExtractor(PKB* pkb)
{
    _ast = pkb->getAST();
    _mt = pkb->getModifiesTable();
    _pt = pkb->getParentTable();
    _ft = pkb->getFollowsTable();
    _ut = pkb->getUsesTable();
	_ct = pkb->getCallsTable();
	_cont = pkb->getContainsTable();
	_st = pkb->getSiblingTable();
	_proct = pkb->getProcTable();
    _stmtt = pkb->getStmtNodeTable();
	_pkb = pkb;
}

DesignExtractor::DesignExtractor() 
{
}

void DesignExtractor::populateTables()
{
	for(int i = 0; i<(int)_ast.getTree().size(); i++)
	{
		int parent = _ast.getNode(i).getParent();
		std::vector<int> children = _ast.getNode(i).getChildren();
		if(children.size()==1)
		{
			_cont->insertContains(i, children[0]);
		}
		else if(children.size()>1)
		{
			for(int j=0;j< (int)children.size();j++)
			{
				_cont->insertContains(i, children[j]);
				for(int k=j+1;k<(int)children.size();k++)
					_st->insertSibling(children[j], children[k]);
				if( j == children.size()-1) 
					break;				
				if(_ast.getNode(i).getNodeType()==Node::stmtLstNode)
				{
					insertFollows(children[j], children[j+1]);//children have the follows relationship
				}
			}
		}
		//if(_ast.getNode(i).getNodeType()==Node::stmtLstNode)//look for stmtLst nodes
		//{	
			/*
			if(children.size()>1)//if the stmtLst node has more than one child
			{
				for(int j = 0; j < (int)children.size()-1; j++)
				{
					int rightSibling = _ast.getNode(children[j]).getRight();
					insertFollows(children[j], rightSibling);//children have the follows relationship			
				}
			}
			*/
			//i.e. if this stmtLst Node is a then:stmtLst or else:stmtLst or just a stmtLst which is a child of a while node
		if(_ast.getNode(i).getNodeType()==Node::stmtLstNode && 
			(_ast.getNode(parent).getNodeType()==Node::ifNode ||_ast.getNode(parent).getNodeType()==Node::whileNode))
		{
			for(int k = 0;k<(int)children.size();k++)
			{
				insertParent(parent, children[k]);
			}
		}
		//}
	}
		
	std::vector<int> assignNodes = _stmtt->getASTWithType(Node::assignNode);
	for(int t = 0; t< (int)assignNodes.size(); t++)
	{
		int tempNode = assignNodes[t];
		std::vector<int> childrenAssign = _ast.getNode(tempNode).getChildren();
		std::vector<int> parents = _pkb->getParentT(_ast.getNode(tempNode).getStmtNum());//get parent and indirect parents of assign node
		insertModifies(tempNode, childrenAssign[0]);//add the first child of the assign node to modifies table
		for(int w = 0; w< (int)parents.size();w++)
		{				
			_mt->insertModifies(parents[w], _ast.getNode(childrenAssign[0]).getValue());//indirect parents modifies this variable too				
		}
		if(_ast.getNode(childrenAssign[1]).getNodeType()==Node::varNode)//if 2nd child is a variable
		{
			insertUses(tempNode,childrenAssign[1]);//add it to the uses table
			for(int v = 0; v< (int)parents.size();v++)
			{
				_ut->insertUses(parents[v],_ast.getNode(childrenAssign[1]).getValue());//indirect parents use this variable too					
			}
		}
		else//if it isn't a variable
		{
			checkChildrenUses(childrenAssign[1], parents);
		}		
	}
	std::vector<int> controlNodes = _stmtt->getASTWithType(Node::whileNode);//while node
	std::vector<int> temp = _stmtt->getASTWithType(Node::ifNode);//if node
	for(int x=0;x<(int)temp.size();x++)
	{
		controlNodes.push_back(temp[x]);
	}
	for(int y=0;y< (int)controlNodes.size();y++)
	{
		std::vector<int> parentsIfWhile = _pkb->getParentT(_ast.getNode(controlNodes[y]).getStmtNum());
		std::vector<int> childrenControl = _ast.getNode(controlNodes[y]).getChildren();
		insertUses(controlNodes[y], childrenControl[0]);
		for(int p = 0; p< (int)parentsIfWhile.size();p++)
		{
			 _ut->insertUses(parentsIfWhile[p],_ast.getNode(childrenControl[0]).getValue());//indirect parents use this variable too					
		}
	}
	//insert calls
	std::vector<int> callNodes = _stmtt->getASTWithType(Node::callNode);//call node
	for(int z=0;z< (int)callNodes.size();z++)
	{
		checkParentIfProc(callNodes[z],callNodes[z]);
	}
	
	
	//inserts modifies and uses for procs (assign, control stmts)
	for(int q=0;q< (int)_proct->getSize();q++)//go through all procs in the proc table
	{
		int firstLine = _proct->getProcFirstln(q);//get first line of proc
		int lastLine = _proct->getProcLastln(q);//get last line of proc
		for(int r = firstLine;r<lastLine+1;r++)//go through every stmt of proc
		{
			if(_stmtt->getType(r)==Node::assignNode)//assign node
			{
				std::vector<int> varsModified = _mt->getModifiedBy(r);//get vars modified by the stmt
				std::vector<int> varsUsed = _ut->getUsedBy(r);//get vars used by the stmt
				for(int s=0;s< (int)varsModified.size();s++)
				{
					_mt->insertProcModifies(q, varsModified[s]);
				}
				for(int m=0;m< (int)varsUsed.size();m++)
				{
					_ut->insertProcUses(q, varsUsed[m]);
				}
			}
			if(_stmtt->getType(r)==Node::whileNode||_stmtt->getType(r)==Node::ifNode)//control node
			{
				std::vector<int> varsUsed = _ut->getUsedBy(r);//get vars used by the stmt
				for(int m=0;m< (int)varsUsed.size();m++)
				{
					_ut->insertProcUses(q, varsUsed[m]);
				}
			}
		}
	}
	
	//inserts modifies and uses for procs (for procs called directly or indirectly)
	for(int q=0;q<_proct->getSize();q++)//go through all procs in the proc table
	{
		std::vector<std::string> procsCalled = _pkb->getCalledByT(_proct->getProcName(q));//get all procs called directly or indirectly from q
		for(int n=0;n<(int)procsCalled.size();n++)
		{
			std::string procName = procsCalled[n];
			int procIndex = _proct->getProcIndex(procName);
			std::vector<int> varsModified = _mt->getModifiedByProc(_proct->getProcIndex(procsCalled[n]));//get all vars modified by that particular proc
			std::vector<int> varsUsed = _ut->getUsedByProc(_proct->getProcIndex(procsCalled[n]));//get all vars used by that particular proc
			for(int a=0;a<(int)varsModified.size();a++)
			{
				_mt->insertProcModifies(q, varsModified[a]);//insert all the vars modified
			}
			for(int b=0;b<(int)varsUsed.size();b++)
			{
				_ut->insertProcUses(q, varsUsed[b]);//insert all the vars used
			}
		}		
	}
	
	//insert modifies and uses for call stmts and their parents (control stmts)
	for(int z=0;z< (int)callNodes.size();z++)
	{
		int stmtNum = _ast.getNode(callNodes[z]).getStmtNum();
		std::vector<int> parents = _pkb->getParentT(stmtNum);
		int procCalled = _ast.getNode(callNodes[z]).getValue();
		addModifiesUsesForCallsStmt(stmtNum, procCalled);

		std::vector<std::string> procsCalled = _pkb->getCalledByT(_proct->getProcName(procCalled));//get all procs called indirectly 
		for(int x=0;x< (int)procsCalled.size();x++)
		{
			addModifiesUsesForCallsStmt(stmtNum, _proct->getProcIndex(procsCalled[x]));
		}
		std::vector<int> varsModified = _mt->getModifiedBy(stmtNum);
		std::vector<int> varsUsed = _ut->getUsedBy(stmtNum);
		for(int y=0;y<(int)parents.size();y++)
		{
			for(int w=0;w< (int)varsModified.size();w++)
			{
				_mt->insertModifies(parents[y],varsModified[w]);
			}
			for(int v=0;v< (int)varsUsed.size();v++)
			{
				_ut->insertUses(parents[y],varsUsed[v]);
			}
		}
	}

    
    _mt->compressTables();
    _ft->compressTables();
    _ut->compressTables();
    _ct->compressTables();
    _pt->compressTables();
    _cont->compressTables();
    _st->compressTable(); 
}
	
void DesignExtractor::addModifiesUsesForCallsStmt(int callStmt, int proc)
{
	std::vector<int> varsModified = _mt->getModifiedByProc(proc);
	std::vector<int> varsUsed = _ut->getUsedByProc(proc);
	for(int y=0;y < (int)varsModified.size();y++)
	{
		_mt->insertModifies(callStmt,varsModified[y]);
	}
	for(int y1=0;y1< (int)varsUsed.size();y1++)
	{
		_ut->insertUses(callStmt,varsUsed[y1]);
	}
}
void DesignExtractor::checkParentIfProc(int nodeIndex, int callNodeIndex)
{
	int callsParent = _ast.getNode(nodeIndex).getParent();//get parent of the callNode
	if(_ast.getNode(callsParent).getNodeType()==Node::procedureNode)//find the caller
	{
		insertCalls(callsParent, callNodeIndex);
		return;
	}
	else//not the caller, continue traversing upwards to find caller
	{
		checkParentIfProc(callsParent, callNodeIndex);
	}
}

 void DesignExtractor::checkChildrenUses(int nodeIndex, std::vector<int> parents)
{
	std::vector<int> children1 = _ast.getNode(nodeIndex).getChildren();
	if(children1.size()>0)
	{					
		for(int m = 0;m< (int)children1.size();m++)
		{
			if(_ast.getNode(children1[m]).getNodeType()==Node::varNode)//check if each child is a variable
			{
				insertUses(nodeIndex,children1[m]);//add it to the uses table
				for(int n = 0;n< (int)parents.size();n++)
				{
					_ut->insertUses(parents[n],_ast.getNode(children1[m]).getValue());					
				}				
			}
			checkChildrenUses(children1[m], parents);
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
	 _ct->insertCalls(_ast.getNode(stmt1).getValue(), _ast.getNode(stmt2).getValue());
 }

