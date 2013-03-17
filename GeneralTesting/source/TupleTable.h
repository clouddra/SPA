#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#ifndef STD_HEAD
#define STD_HEAD
#include "Common.hpp"
#endif

class TupleTable {

private:
	std::vector<std::string> variables;
    std::vector<std::vector<std::string>> values;

public:
	TupleTable(std::vector<std::string> vars, std::vector<std::vector<std::string>> vals);
    std::vector<std::string> getVars();
    int merge(TupleTable other);
    std::set<std::string> getValuesFor(std::string var); 
    std::vector<std::vector<std::string>> getValuesFor(std::vector<std::string> vars); 
};