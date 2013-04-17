#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

/**
 * Stores the nodeIndexes of various types of nodes (types not stored in StmtNodeTable)
 */
class NodeTable {

private:
    std::vector<int> plus;
    std::vector<int> minus;
    std::vector<int> times;
    std::vector<int> divide;
    std::vector<int> constants;
    std::vector<int> stmtLst;
    std::vector<int> procedures;
    std::vector<int> variables;

public:
    /**
	 * Getter function for plus vector
	 */
    std::vector<int> getPlus();

    /**
	 * Getter function for minus vector
	 */
    std::vector<int> getMinus();

    /**
	 * Getter function for times vector
	 */
    std::vector<int> getTimes();

    /**
	 * Getter function for divide vector
	 */
    std::vector<int> getDivide();

    /**
	 * Getter function for constants vector
	 */
    std::vector<int> getConstants();

    /**
	 * Getter function for procedures vector
	 */
    std::vector<int> getProcs();

    /**
	 * Getter function for stmtLst vector
	 */
    std::vector<int> getStmtLst();

    /**
	 * Getter function for variables vector
	 */
    std::vector<int> getVariables();

    /**
	 * Insert a nodeIndex with the given type into the correct vector
	 */
    void insert(int nodeIndex, int type);
};