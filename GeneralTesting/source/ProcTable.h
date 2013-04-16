#ifndef PROCEDURE
#define PROCEDURE
#include "Procedure.h"
#endif

#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#ifndef UNORDERED_HEAD
#define UNORDERED_HEAD
#include <unordered_map>
#endif

class ProcTable {

private:
	std::vector<Procedure> numToProc;
	std::unordered_map<std::string, int> procToNum;
	

public:
	ProcTable();

	/**
	 * If procName not in the ProcTable, inserts it to the ProcTable and returns its index.
	 * Otherwise, returns its INDEX and the table remain unchanged
	 */
	int insertProc(std::string procName);

	/**
	 * Sets the first and last line of the proc at INDEX
	 */
    void updateProc(int index, int first, int last);

	/**
	 * Returns the total number of procedures in ProcTable
	 */
	int getSize();

	/**
	 * Returns the name of a variable at ProcTable [ind]
     * If ind is out of range,
     * Throws: InvalidReferenceException
	 */
	std::string getProcName (int ind);

	/**
	 * If procName is in ProcTable, returns its index;
     * Otherwise, return -1 (special value)
	 */
	int getProcIndex (std::string procName);

	/**
	 * Sets the first line of the proc at ind
	 */
	int getProcFirstln(int ind);

	/**
	 * Sets the last line of the proc at ind
	 */
	int getProcLastln(int ind);

	/**
	 * Returns the proc that the stmt with stmtNo is in
	 * If stmtNo is not in any procedure, return -1
	 */
	int getProcOfStmt(int stmtNo);

	/**
	 * Returns procedure at ind
	 */
	Procedure getProc(int ind);

	/**
	 * Returns the names of all procedures
	 */
    std::vector<std::string> getAllProc();

	/**
	 * Sets CFG node 'start' as the first node of the CFG for proc at ind
	 */
	void setCFGStart(int ind, int start);

	/**
	 * Sets CFG node 'end' as the last node of the CFG for proc at ind
	 */
	void setCFGEnd(int ind, int end);

	/**
	 * Gets the starting CFG node for proc at ind
	 */
	int getCFGStart(int ind);

	/**
	 * Gets the ending CFG node for proc at ind
	 */
	int getCFGEnd(int ind);
};