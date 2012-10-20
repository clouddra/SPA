#ifndef STRING_HEAD
#define STRING_HEAD
#include <string>
#endif

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class VarTable {

private:
	std::vector<std::string> varTable;

public:
	VarTable();
	int insertVar(std::string varName);
	int getSize();
	std::string getVarName (int ind);
	int getVarIndex (std::string varName);
};