#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class ParentTable {

private:
	/**
	 * Stores stmts which calls vector index.
	 */
	std::vector<std::vector <int>> parentTable;

	/**
	 * Stores procedures called by vector index.
	 */
	std::vector<std::vector <int>> childTable;
	int size, parentCount, childCount;


public:
	
	ParentTable();
	/**
	 * If the parent relationship between stmt 1 and stmt 2 is not in the Parent Table, inserts it to the Parent Table and returns TRUE
     * Otherwise, returns FALSE and the table remain unchanged
	 */
	bool insertParent(int stmt1, int stmt2);

	/**
	 * Return all parents of stmt2.
	 */
	std::vector<int> getParent(int stmt2);

	/**
	 * Return all parents.
	 */
	std::vector<int> getParent();
	/**
	 * Return all children of stmt1.
	 */
  	std::vector<int> getChild(int stmt1);

	/**
	 * Return all children.
	 */
	std::vector<int> getChild();

	/**
	 * Returns TRUE if stmt1 is the parent of stmt2.
     * Otherwise, return FALSE.
	 */
	bool isParent(int stmt1, int stmt2); 

	/**
	 * Returns the total number of relationships in Parent Table
	 */
	int getSize();
	
	/**
	 * Remove empty entries in Parent Table. To be invoked after extraction of parent relationship
	 */
	void compressTables();
};

/*
class ParentTable {

private:
	std::vector<std::pair <int ,int>> parentTable;


public:
	ParentTable();
	int insertParent(int stmt1, int stmt2);
	int getParent(int stmt);
    std::vector<int> getParent();
	std::vector<int> getChild(int stmt);
    std::vector<int> getChild();
	bool isParent(int stmt1, int stmt2);
	int getParentIndex(int stmt1, int stmt2);
	std::pair <int ,int> extractParent (int ind);
	int getSize();
};
*/