#ifndef IOSTREAM_HEAD
#define IOSTREAM_HEAD
#include <iostream>
#endif

#ifndef STRING_HEAD
#define STRING_HEAD
#include <string>
#endif

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class DeclarationTable {

private:
	std::vector<std::pair<int, std::string>> declarationTable;

public:
	DeclarationTable();
	enum {stmt_, assign_, while_, if_, variable_, constant_, prog_line_};
	int insertDeclaration(int nodeType, std::string name);
	int getSize();
	bool isDeclared(int nodeType, std::string name);
    int getType(std::string name);
	void printDeclarationTable(); // For testing
};
