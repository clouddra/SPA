#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class ModifiesTable {

private:
	std::vector<std::pair <int ,int>> modifiesTable;
	std::vector<std::vector <int>> modifiesProcTable;

public:
	ModifiesTable();
	int insertModifies(int stmt, int var);
	int insertProcModifies(int procIndex, int var);

	std::vector<int> getModifiesVar(int var);
    std::vector<int> getModifiesVar();
	//returns all variables modified by procedure of procIndex
	std::vector<int> getModifiesVarProc(int procIndex);

	std::vector<int> getModifiedBy(int stmt);
    std::vector<int> getModifiedBy();

	//returns all procedures which modifies var
	std::vector<int> getModifiedByProc(int var);

	bool isModifies(int stmt, int var);
	bool isModifiesProc(int procIndex, int var);

	int getModifiesIndex(int stmt, int var);
	int getModifiesProcIndex(int procIndex, int var);

	std::pair <int ,int> extractModifies (int ind);
	std::vector<int> extractModifiesProc (int ind);
	int getModifiesTableSize();
	int getModifiesProcTableSize();
};