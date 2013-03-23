#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class SiblingTable {

private:

	std::vector<std::pair <int ,int>> siblingTable;
	int size;

public:
	SiblingTable();
	int insertSibling(int stmt1, int stmt2);
	std::vector<int> getSiblings(int stmt);
	bool isSibling(int stmt1, int stmt2);
	int getSiblingIndex(int stmt1, int stmt2);
	int getSize();
};