#ifndef DECLARATION_HEAD
#define DECLARATION_HEAD
#include "DeclarationTable.h"
#endif

DeclarationTable::DeclarationTable(){}

int DeclarationTable::insertDeclaration(int nodeType, std::string name)
{
	if (isDeclared(nodeType, name))
		return -1;

	declarationTable.push_back(std::pair<int, std::string>(nodeType, name));
	return getSize()-1;
}

int DeclarationTable::getSize()
{
	return declarationTable.size();
}

bool DeclarationTable::isDeclared(int nodeType, std::string name)
{
	for (int i=0; i<declarationTable.size(); i++)
	{
		if (declarationTable.at(i).first == nodeType && declarationTable.at(i).second == name)
			return true;
	}
	return false;
}

void DeclarationTable::printDeclarationTable()
{
	for (int i=0; i<declarationTable.size(); i++)
		std::cout << declarationTable.at(i).first << " " << declarationTable.at(i).second << std::endl;
	
	std::cout<< "Size of declaration table: " << getSize() << std::endl;
}