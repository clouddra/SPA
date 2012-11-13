#ifndef STRING_HEAD
#define STRING_HEAD
#include <string>
#endif

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class PatternRow {

private:
	std::string pattern; // Eg. if declaration was "assign a1;" and "pattern a1(...)", then this stores declaration "a1".
	std::string declaredVar; // "pattern a(v,...)" then this stores v. This field could be empty, but is needed for intersecting queries.
	std::string varName; // "pattern a("x",...)" then this stores x. This field is always non-empty.
	std::vector<int> stmtNumList;

public:
	PatternRow(std::string, std::string, std::string, std::vector<int>);
	std::string getPattern();
	std::string getVarName();
	std::string getDeclaredVar();
	std::vector<int> getStmtNumList();

};