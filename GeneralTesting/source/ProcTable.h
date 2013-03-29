#ifndef PROCEDURE
#define PROCEDURE
#include "Procedure.h"
#endif

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#ifndef UNORDERED_HEAD
#define UNORDERED_HEAD
#include <unordered_map>
#endif

class ProcTable {

private:
	std::vector<Procedure> numToProc;
	std::unordered_map<std::string, int> procToNum;
	

public:
	ProcTable();
	int insertProc(std::string procName);
    void updateProc(int index, int first, int last);
	int getSize();
	std::string getProcName (int ind);
	int getProcIndex (std::string procName);
	int getProcFirstln(int ind);
	int getProcLastln(int ind);
	Procedure getProc(int ind);
    std::vector<std::string> getAllProc();
	void setCFGStart(int ind, int start);
	void setCFGEnd(int ind, int end);
};