#ifndef STRING_HEAD
#define STRING_HEAD
#include <string>
#endif

class Procedure {

private:

	std::string procName;
	int firstLine;
	int lastLine;

public:

	Procedure(std::string pName, int first, int last);
	std::string getProcName();
	int getFirstLine();
	int getLastLine();

};