#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class UsesTable {

private:
	std::vector<std::pair <int ,int>> usesTable;

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
	 * Returns all variables used by procedure proc
	 */
  	std::vector<int> getUsedByProc(int proc);

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


/*
5.7 Uses for statements
STMT[] getUsesVar (VAR var)
Description:

Returns all statements that uses variable var.
Else return NULL.

VAR[] getUsedBy (STMT stmtNum)
Description:

Returns all variables used by a statement.
Else return NULL.


BOOL isUses (STMT stmtNum, VAR var)
Description:
    Returns TRUE if stmtNum uses var.
    Otherwise, return FALSE.

INTEGER getSize()
Description:
    Returns the total number of relationships in Uses Table

INDEX getUsesIndex (STMT stmtNum, VAR var)
Description:

If the Uses relationship between the statement and the variable is in the Uses Table, returns its index.
    Otherwise, returns -1 (special value)

PAIR<STMT, VAR> extractUses (INDEX ind)
Description:
    Returns the relationship of two statements at Uses Table [ind]
If “ind” is out of range:
Throws:
    InvalidReferenceException

5.8 Uses for procedures
PROC[] getUsesVarProc (VAR var)
Description:

Return all procedures that use variable var.

Else return NULL.

VAR[] getUsedByProc (PROC proc)
Description:

Returns all variables used by procedure proc

Else return NULL.

BOOL isUsesProc (PROC proc, VAR var)
Description:
    Returns TRUE if proc uses var.
    Otherwise, return FALSE.
*/