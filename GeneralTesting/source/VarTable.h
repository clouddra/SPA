#ifndef STRING_HEAD
#define STRING_HEAD
#include <string>
#endif

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#ifndef UNORDERED_HEAD
#define UNORDERED_HEAD
#include <unordered_map>
#endif


class VarTable {

private:
	std::vector<std::string> numToVar;
	std::unordered_map<std::string, int> varToNum;

public:
	VarTable();
	int insertVar(std::string varName);
	int getSize();
	std::string getVarName (int ind);
	int getVarIndex (std::string varName);
    std::vector<std::string> getTable();
};