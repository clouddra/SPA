#include <iostream>

#ifndef PKB
#define PKB
#include "PKB.h"
#endif
//using namespace std;


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
*/

	ProcTable q = ProcTable();

	std::cout << q.insertProc("function1", 1, 2) << std::endl ;
	std::cout << q.insertProc("function2", 4, 5) << std::endl ;
	std::cout << q.insertProc("function3", 6, 7) << std::endl ;

	std::cout << q.getProcIndex("function1") << std::endl ;
	std::cout << q.getProcIndex("function3") << std::endl ;
	std::cout << q.getProcIndex("lol") << std::endl ;

	q.getSize() ;
	std::cout << q.getProcName(2) ;



	system("pause");
	return 0;
}