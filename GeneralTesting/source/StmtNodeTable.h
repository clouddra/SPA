// Temporary name, feel free to change
// Links stmtNum (Index) to ast node + nodeType

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#include "StmtNode.h"


class StmtNodeTable {

private:
	std::vector<StmtNode> stmtNodeTable;

public:
	StmtNodeTable();
	int insertStmtNode(int stmt, int astNode, int cfgNode, int cfgNodeBip, int nodeType);
	int insertStmtNode(int stmt, int astNode, int nodeType);
	std::vector<int> getASTWithType(int nodeType);
	std::vector<int> getCFGWithType(int nodeType);
	std::vector<int> getCFGBipWithType(int nodeType);
	std::vector<int> getStmtWithType(int nodeType);
	int getAST(int stmt);
	int getCFG(int stmt);
	int getCFGBip(int stmt);
	int getType(int stmt);
	void setAST(int stmt, int ast);
	void setCFG(int stmt, int cfg);
	void setCFGBip(int stmt, int cfg);
	void setType(int stmt, int type);
	int getSize();
};