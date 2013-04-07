#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class UsesTable {

private:

	/**
	 * Index: procedure, Elements: variable used by procedures
	 */
	std::vector<std::vector <int>> procToVarTable;

	/**
	 * Index: variable, Elements: procedures that use variables
	 */
	std::vector<std::vector <int>> varToProcTable;

	/**
	 * Index: variable, Elements: statment that use variables
	 */
	std::vector<std::vector <int>> varToStmtTable;

	
	/**
	 * Index: statement, Elements: variables that used by statement
	 */
	std::vector<std::vector <int>> stmtToVarTable;

	int size, stmtCount, varCount;
	int procSize, procCount, procVarCount;

public:
	UsesTable();
//	int insertUses(int stmt, int var);
	bool insertUses(int stmt, int var);

	/**
	 * Returns all statements that uses variable var.
	 */
	std::vector<int> getUsesVar(int var);

	/**
	 * Returns all statements that uses any variables.
	 */
    std::vector<int> getUsesVar();

	/**
	 * Returns all variables that is used by statement stmt.
	 */
	std::vector<int> getUsedBy(int stmt);

	/**
	 * Returns all variables that is used by any statements.
	 */
    std::vector<int> getUsedBy();
	
	/**
	 * If the proc-var Uses pair is not in the Uses Table, inserts it to the Uses Table and returns TRUE
	 * Otherwise, returns FALSE and the table remain unchanged.
	 */
	bool insertProcUses(int proc, int var);

	/**
	 * Return all procedures that use variable var.
	 */
	std::vector<int> getUsesVarProc(int var);

    /**
	 * Returns all procedures that uses any variables.
	 */
    std::vector<int> getUsesVarProc();

	/**
	 * Returns all variables used by procedure proc
	 */
  	std::vector<int> getUsedByProc(int proc);

    /**
	 * Returns all variables that is used by any procedures.
	 */
    std::vector<int> getUsedByProc();

	/**
	 * Returns TRUE if proc uses var.
	 * Otherwise, return FALSE.
	*/
	bool isUsesProc(int proc, int var);


	bool isUses(int stmt, int var);
	//int getUsesIndex(int stmt, int var);
	//std::pair <int ,int> extractUses (int ind);
	int getSize();
	int getSizeProc();

	/**
	 * Remove empty entries in Calls Table. To be invoked after extraction of calls relationship
	 */
	void compressTables();
};
