#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class ContainsTable {

private:
	/**
	 * Stores stmts which are containers of vector index.
	 */
	std::vector<std::vector <int>> containerTable;

	/**
	 * Stores stmts which are contained by vector index.
	 */
	std::vector<std::vector <int>> containedInTable;
	int size, containerCount, containeeCount;


public:
	
	ContainsTable();
	/**
	 * If the contains relationship between stmt 1 and stmt 2 is not in the Contains Table, inserts it to the Contains Table and returns TRUE
     * Otherwise, returns FALSE and the table remain unchanged
	 */
	bool insertContains(int stmt1, int stmt2);

	/**
	 * Return all containers of stmt2.
	 */
	std::vector<int> getContainer(int stmt2);

	/**
	 * Return all containers.
	 */
	std::vector<int> getContainer();
	/**
	 * Return all stmts contained in stmt1.
	 */
  	std::vector<int> getContainedIn(int stmt1);

	/**
	 * Return all stmts contained.
	 */
	std::vector<int> getContainedIn();

	/**
	 * Returns TRUE if stmt1 is the container of stmt2.
     * Otherwise, return FALSE.
	 */
	bool isContains(int stmt1, int stmt2); 

	/**
	 * Returns the total number of relationships in Contains Table
	 */
	int getSize();
	
	/**
	 * Remove empty entries in Contains Table. To be invoked after extraction of contains relationship
	 */
	void compressTables();
};