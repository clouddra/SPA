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

    /**
	 * Insert a stmt with coresponding astNode, cfgNode and nodeType
     * Returns stmt (same as INDEX) if successful, -1 if not
     * Expects stmts to be inserted in order
	 */
	int insertStmtNode(int stmt, int astNode, int cfgNode, int nodeType);

    /**
	 * Insert a stmt with coresponding astNode and nodeType
     * Returns stmt (same as INDEX) if successful, -1 if not
     * Expects stmts to be inserted in order
	 */
	int insertStmtNode(int stmt, int astNode, int nodeType);

    /**
	 * Returns every ast node with a given type
	 */
	std::vector<int> getASTWithType(int nodeType);

    /**
	 * Returns every cfg node with a given type
	 */
	std::vector<int> getCFGWithType(int nodeType);

    /**
	 * Returns every stmt with a given type
	 */
	std::vector<int> getStmtWithType(int nodeType);

    /**
	 * Returns ast node of a given stmt
	 */
	int getAST(int stmt);

    /**
	 * Returns cfg node of a given stmt
	 */
	int getCFG(int stmt);

    /**
	 * Returns nodeType of a given stmt
	 */
	int getType(int stmt);

    /**
	 * Set the ast node of a given stmt
	 */
	void setAST(int stmt, int ast);

    /**
	 * Set the cfg node of a given stmt
	 */
	void setCFG(int stmt, int cfg);

    /**
	 * Set the nodeType of a given stmt
	 */
	void setType(int stmt, int type);

    /**
	 * Get the size of the table (1 more than number of stmts due to dummy)
	 */
	int getSize();
};