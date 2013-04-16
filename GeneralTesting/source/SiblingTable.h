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

	/**
	 * Returns all siblings of stmt
	 */
	std::vector<int> getSiblings(int stmt);
	bool isSibling(int stmt1, int stmt2);

	/**
	 * Returns index of relation with stmt and stmt2
	 * If not found, return -1
	 */
	int getSiblingIndex(int stmt1, int stmt2);

	/**
	 * Returns total number of sibling relations
	 */
	int getSize();
};