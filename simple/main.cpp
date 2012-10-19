#include <iostream>
#include "Node.h"
#include "ModifiesTable.h"

#include "VarTable.h"


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

	ModifiesTable t = ModifiesTable();

	std::cout << t.insertModifies(1,1) << std::endl ;
	std::cout << t.insertModifies(2,1) << std::endl ;
	std::cout << t.insertModifies(3,1) << std::endl ;
	std::cout << t.insertModifies(1,1) << std::endl ;	 

	std::cout << t.getModifiesIndex(1,1) << std::endl ;
	std::cout << t.getModifiesIndex(3,1) << std::endl ;
	std::cout << t.getModifiesIndex(2,2) << std::endl ;
	//	 t.extractModifies(10)  ;
	*/

	VarTable t = VarTable();

	std::cout << t.insertVar("a") << std::endl ;
	std::cout << t.insertVar("b") << std::endl ;	 
	std::cout << t.insertVar("c") << std::endl ;
	std::cout << t.insertVar("b") << std::endl ;

	std::cout << t.getVarName(2) << std::endl ;

	//	std::cout << t.getVarName(5) << std::endl ;
	system("pause");
	return 0;
}