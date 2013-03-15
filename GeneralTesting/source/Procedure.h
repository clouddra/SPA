#ifndef STRING_HEAD
#define STRING_HEAD
#include <string>
#endif

class Procedure {

private:
	std::string procName;
	int firstLine;
	int lastLine;
	int cfgEnd;

public:
	Procedure(std::string pName, int first, int last);
	std::string getProcName();
	int getFirstLine();
	int getLastLine();
	int getCFGEnd();
	void setCFGEnd(int end);
    void setFirstAndLast(int first, int last);
};