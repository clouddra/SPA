#include <vector>


class UsesTable {

private:
	std::vector<std::pair <int ,int>> usesTable;


public:
	UsesTable();
	int insertUses(int stmt, int var);
	std::vector<int> getUsesVar(int var);
    std::vector<int> getUsesVar();
	std::vector<int> getUsedBy(int stmt);
    std::vector<int> getUsedBy();
	bool isUses(int stmt, int var);
	int getUsesIndex(int stmt, int var);
	std::pair <int ,int> extractUses (int ind);
	int getSize();

};