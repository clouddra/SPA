#include <vector>


class UsesTable {

private:
	std::vector<std::pair <int ,int>> usesTable;


public:
	UsesTable();
	int insertUses(int stmt, int var);
	std::vector<int> getUsesVar(int var);
	std::vector<int> getUsedBy(int stmt);
	bool isUses(int stmt, int var);
	int getUsesIndex(int stmt, int var);
	std::pair <int ,int> extractUses (int ind);
	int getSize();

};