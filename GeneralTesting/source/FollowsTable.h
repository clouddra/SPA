#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class FollowsTable {

private:
	std::vector<std::pair <int ,int>> followsTable;


public:
	FollowsTable();
	int insertFollows(int stmt1, int stmt2);
	std::vector<int> getFollows(int stmt);
	std::vector<int> getFollowedBy(int stmt);
	bool isFollows(int stmt1, int stmt2);
	int getFollowsIndex(int stmt1, int stmt2);
	std::pair <int ,int> extractFollows (int ind);
	int getSize();
};