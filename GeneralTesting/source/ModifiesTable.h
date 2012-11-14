#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class ModifiesTable {

private:
	std::vector<std::pair <int ,int>> modifiesTable;


public:
	ModifiesTable();
	int insertModifies(int stmt, int var);
	std::vector<int> getModifiesVar(int var);
    std::vector<int> getModifiesVar();
	std::vector<int> getModifiedBy(int stmt);
    std::vector<int> getModifiedBy();
	bool isModifies(int stmt, int var);
	int getModifiesIndex(int stmt, int var);
	std::pair <int ,int> extractModifies (int ind);
	int getSize();
};