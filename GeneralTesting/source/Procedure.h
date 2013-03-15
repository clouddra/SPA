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
	int cfgStart;

public:
	Procedure(std::string pName, int first, int last);
	std::string getProcName();
	int getFirstLine();
	int getLastLine();
	int getCFGStart();
	int getCFGEnd();
	void setCFGEnd(int end);
	void setCFGStart(int start);
    void setFirstAndLast(int first, int last);
};