#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class SiblingTable {

private:

	std::vector<std::vector <int>> siblingTable;
	int size;
	/*
	std::vector<std::pair <int ,int>> siblingTable;
	int size;
	*/
public:
	SiblingTable();
	bool insertSibling(int stmt1, int stmt2);

	/**
	 * Returns all siblings of stmt
	 */
	std::vector<int> getSiblings(int stmt);

	/**
	 * Returns TRUE if input1 is a sibling of input2.
     * Otherwise, return FALSE.
	 */
	bool isSibling(int stmt1, int stmt2);
	
	/**
	 * Returns total number of sibling relations
	 */
	int getSize();

    void compressTable();
};