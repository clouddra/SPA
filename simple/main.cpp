#include <iostream>

#ifndef PARSER_HEAD
#define PARSER_HEAD
#include "Parser.h"
#endif

#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

#ifndef PQLPARSER_HEAD
#define PQLPARSER_HEAD
#include "PqlParser.h"
#endif

#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

//using namespace std;
#include "DesignExtractor.h"

int main() {

	/* Testing code
	Node temp = Node(1, 1);
	for (int i = 0; i < 10; i++) {
	temp.addChild(i);
	}

	std::vector<int> temp2 = temp.getChildren();
	for (int i = 0; i < temp2.size(); i++) {
	std::cout << temp2[i] << "\n";
	} 

	ModifiesTable v = ModifiesTable();

	std::cout << v.insertModifies(1,1) << std::endl ;
	std::cout << v.insertModifies(2,1) << std::endl ;
	std::cout << v.insertModifies(3,1) << std::endl ;
	std::cout << v.insertModifies(1,1) << std::endl ;	 

	std::cout << v.getModifiesIndex(1,1) << std::endl ;
	std::cout << v.getModifiesIndex(3,1) << std::endl ;
	std::cout << v.getModifiesIndex(2,2) << std::endl ;
	//	 t.extractModifies(10)  ;
	

	VarTable t = VarTable();

	std::cout << t.insertVar("a") << std::endl ;
	std::cout << t.insertVar("b") << std::endl ;	 
	std::cout << t.insertVar("c") << std::endl ;
	std::cout << t.insertVar("b") << std::endl ;

	std::cout << t.getVarName(2) << std::endl ;

	//	std::cout << t.getVarName(5) << std::endl ;

	UsesTable s = UsesTable();

	std::cout << s.insertUses(1,1) << std::endl ;
	std::cout << s.insertUses(2,1) << std::endl ;
	std::cout << s.insertUses(3,1) << std::endl ;
	std::cout << s.insertUses(1,1) << std::endl ;	 

	std::cout << s.getUsesIndex(1,1) << std::endl ;
	std::cout << s.getUsesIndex(3,1) << std::endl ;
	std::cout << s.getUsesIndex(2,2) << std::endl ;

	s.extractUses(1) ;
	s.extractUses(10) ;

	ProcTable q = ProcTable();

	std::cout << q.insertProc("function1", 1, 2) << std::endl ;
	std::cout << q.insertProc("function2", 4, 5) << std::endl ;
	std::cout << q.insertProc("function3", 6, 7) << std::endl ;

	std::cout << q.getProcIndex("function1") << std::endl ;
	std::cout << q.getProcIndex("function3") << std::endl ;
	std::cout << q.getProcIndex("lol") << std::endl ;

	q.getSize() ;
	std::cout << q.getProcName(2) ;
    */
	/*
    PKB temp = PKB();
    int proc = temp.insertNode(Node::procedureNode, "first", -1, 0);
    int stmtLst = temp.insertNode(Node::stmtLstNode, "", -1, proc);
    int assign = temp.insertNode(Node::assignNode, "", 1, stmtLst);
    int var = temp.insertNode(Node::varNode, "x", 1, assign);
    temp.insertNode(Node::constNode, "5", 1, assign);
    assign = temp.insertNode(Node::assignNode, "", 2, stmtLst);
    var = temp.insertNode(Node::varNode, "y", 2, assign);
    int plus = temp.insertNode(Node::plusNode, "", 2, assign);
    temp.insertNode(Node::constNode, "5", 2, plus);
    temp.insertNode(Node::varNode, "x", 2, plus);

	DeclarationTable declare;
	declare.insertDeclaration(declare.stmt_, "s2");
	declare.insertDeclaration(declare.assign_, "a2");
	declare.insertDeclaration(declare.variable_, "x");
	declare.insertDeclaration(declare.stmt_, "s2");
	declare.insertDeclaration(declare.stmt_, "s1");

	declare.printDeclarationTable();

    */
    
 //   PKB pkb = PKB();
 //   Parser temp = Parser();
 //   temp.parseCode("..\\sample_input\\simple.txt", &pkb);
	//DesignExtractor* de = new DesignExtractor(pkb.getAST(),pkb.getStmtNodeTable(),pkb.getModifiesTable(),pkb.getParentTable(),pkb.getFollowsTable(),pkb.getUsesTable(),&pkb);
 //   system("pause");

	//return 0;

	PKB temp = PKB();
    int proc = temp.insertNode(Node::procedureNode, "first", -1, 0);
    int stmtLst = temp.insertNode(Node::stmtLstNode, "", -1, proc);
    int assign = temp.insertNode(Node::assignNode, "", 1, stmtLst);
    int var = temp.insertNode(Node::varNode, "x", 1, assign);
    temp.insertNode(Node::constNode, "5", 1, assign);
    assign = temp.insertNode(Node::assignNode, "", 2, stmtLst);
    var = temp.insertNode(Node::varNode, "y", 2, assign);
    int plus = temp.insertNode(Node::plusNode, "", 2, assign);
    temp.insertNode(Node::constNode, "5", 2, plus);
    temp.insertNode(Node::varNode, "x", 2, plus);
	DesignExtractor* de = new DesignExtractor(temp.getAST(),temp.getStmtNodeTable(),temp.getModifiesTable(),temp.getParentTable(),temp.getFollowsTable(),temp.getUsesTable(),&temp);
	//ModifiesTable* mt = temp.getModifiesTable();
	temp.printModifiesTable();
	temp.printUsesTable();
	temp.printFollowsTable();
	system("pause");
	return 0;
}