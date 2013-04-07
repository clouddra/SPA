#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class ModifiesTable {

private:

	/**
	 * Index: procedure, Elements: variable modified by procedures
	 */
	std::vector<std::vector <int>> procToVarTable;

	/**
	 * Index: variable, Elements: procedures that modify variables
	 */
	std::vector<std::vector <int>> varToProcTable;

	/**
	 * Index: variable, Elements: statement that modify variables
	 */
	std::vector<std::vector <int>> varToStmtTable;

	
	/**
	 * Index: statement, Elements: variables that are modified by statement
	 */
	std::vector<std::vector <int>> stmtToVarTable;

	int size, stmtCount, varCount;
	int procSize, procCount, procVarCount;

public:
	ModifiesTable();
	bool insertModifies(int stmt, int var);

	/**
	 * Returns all statements that modifies variable var.
	 */
	std::vector<int> getModifiesVar(int var);

	/**
	 * Returns all statements that modifies any variables.
	 */
    std::vector<int> getModifiesVar();

	/**
	 * Returns all variables that are modified by statement stmt.
	 */
	std::vector<int> getModifiedBy(int stmt);

	/**
	 * Returns all variables that are modified by any statement.
	 */
    std::vector<int> getModifiedBy();
	
	/**
	 * If the proc-var Modifies pair is not in the Modifies Table, inserts it to the Modifies Table and returns TRUE
	 * Otherwise, returns FALSE and the table remain unchanged.
	 */
	bool insertProcModifies(int proc, int var);

	/**
	 * Return all procedures that modify variable var.
	 */
	std::vector<int> getModifiesVarProc(int var);

    /**
	 * Returns all procedures that modify any variables.
	 */
    std::vector<int> getModifiesVarProc();

	/**
	 * Returns all variables modified by procedure proc
	 */
  	std::vector<int> getModifiedByProc(int proc);

    /**
	 * Returns all variables that is modified by any procedure.
	 */
    std::vector<int> getModifiedByProc();

	/**
	 * Returns TRUE if proc modifies var.
	 * Otherwise, return FALSE.
	*/
	bool isModifiesProc(int proc, int var);


	bool isModifies(int stmt, int var);
	int getSize();
	int getSizeProc();

	/**
	 * Remove empty entries in Calls Table. To be invoked after extraction of calls relationship
	 */
	void compressTables();
};
