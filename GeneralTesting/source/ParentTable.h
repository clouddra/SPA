#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class ParentTable {

private:
	std::vector<std::pair <int ,int>> parentTable;


public:
	ParentTable();
	int insertParent(int stmt1, int stmt2);
	int getParent(int stmt);
	std::vector<int> getChild(int stmt);
	bool isParent(int stmt1, int stmt2);
	int getParentIndex(int stmt1, int stmt2);
	std::pair <int ,int> extractParent (int ind);
	int getSize();
};