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
    /**
	 * Constructor with data members initialised to given values
	 */
	TupleTable(std::vector<std::string> vars, std::vector<std::vector<std::string>> vals);

    /**
	 * Getter function for variable vector
	 */
    std::vector<std::string> getVars();

    /**
	 * Merge 2 TupleTables together, doing the relevant intersections
	 */
    int merge(TupleTable other);

    /**
	 * Get values for a given variable, returns empty set if not found
	 */
    std::set<std::string> getValuesFor(std::string var); 

    /**
	 * Get values for some given variables, returns empty set if not found (All given variables have to be found)
	 */
    std::vector<std::vector<std::string>> getValuesFor(std::vector<std::string> vars); 
};