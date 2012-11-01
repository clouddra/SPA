#ifndef PROCEDURE
#define PROCEDURE
#include "Procedure.h"
#endif

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class ProcTable {

private:
	std::vector<Procedure> procTable;

public:
	ProcTable();
	int insertProc(std::string procName, int start, int end);
	int getSize();
	std::string getProcName (int ind);
	int getProcIndex (std::string procName);
	int getProcFirstln(int ind);
	int getProcLastln(int ind);
};