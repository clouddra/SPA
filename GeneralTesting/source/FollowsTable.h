#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif
class FollowsTable {

private:
	/**
	 * Stores stmts which is follows vector index.
	 */
	std::vector<std::vector <int>> followsTable;

	/**
	 * Stores stmts which lead vector index.
	 */
	std::vector<std::vector <int>> followedByTable;
	int size, followerCount, leaderCount;


public:
	
	FollowsTable();
	/**
	 * If the follows relationship between stmt 1 and stmt 2 is not in the Follows Table, inserts it to the Follows Table and returns TRUE
     * Otherwise, returns FALSE and the table remain unchanged
	 */
	bool insertFollows(int stmt1, int stmt2);

	/**
	 * Return all stmts which lead stmt2.
	 */
	std::vector<int> getFollows(int stmt2);

	/**
	 * Return all stmts which leads another stmt.
	 */
	std::vector<int> getFollows();
	/**
	 * Return all stmts which follow stmt1.
	 */
  	std::vector<int> getFollowedBy(int stmt1);

	/**
	 * Return all stmts which are followed by another stmt.
	 */
	std::vector<int> getFollowedBy();

	/**
	 * Returns TRUE if stmt2 follows stmt1.
     * Otherwise, return FALSE.
	 */
	bool isFollows(int stmt1, int stmt2); 

	/**
	 * Returns the total number of relationships in Follows Table
	 */
	int getSize();
	
	/**
	 * Remove empty entries in Follows Table. To be invoked after extraction of follows relationship
	 */
	void compressTables();
};
