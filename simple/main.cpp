#include <iostream>

#ifndef NODE_HEAD
#define NODE_HEAD
#include "Node.h"
#endif

#ifndef MODIFIES_HEAD
#define MODIFIES_HEAD
#include "ModifiesTable.h"
#endif

#ifndef VARTABLE_HEAD
#define VARTABLE_HEAD
#include "VarTable.h"
#endif

#ifndef USESTABLE_HEAD
#define USESTABLE_HEAD
#include "UsesTable.h"
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
*/
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

	system("pause");
	return 0;
}