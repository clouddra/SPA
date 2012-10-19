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
	std::string insertVar(std::string varName);
	int getSize();
	std::string getVarName (int ind) throw(std::out_of_range);
	int getVarIndex (std::string varName);
};