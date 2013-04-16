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

	/**
	 *If varName not in the VarTable, inserts it to the VarTable and returns its index. 
	 * Otherwise, returns its INDEX and the table remain unchanged
	 */
	int insertVar(std::string varName);

	/**
	 * Returns the total number of variables in VarTable
	 */
	int getSize();

	/**
	 * Gets name of variable at ind
	 */
	std::string getVarName (int ind);

	/**
	 * Gets index of variable with name 'varName'
	 */
	int getVarIndex (std::string varName);

	/**
	 * Returns entire numtoVar table
	 */
    std::vector<std::string> getTable();
};