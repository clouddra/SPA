// Temporary name, feel free to change
// Links stmtNum (Index) to ast node + nodeType

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class StmtNodeTable {

private:
	std::vector<std::pair <int ,int>> stmtNodeTable;


public:
	StmtNodeTable();
	int insertStmtNode(int stmt, int astNode, int nodeType);
    std::vector<int> getNodeWithType(int nodeType);
	std::vector<int> getStmtWithType(int nodeType);
    int getNode(int stmt);
    int getType(int stmt);
	int getSize();
};