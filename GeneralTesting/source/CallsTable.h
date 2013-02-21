#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class CallsTable {

private:
	/**
	 * Stores procedure which calls vector index.
	 */
	std::vector<std::vector <int>> callsTable;

	/**
	 * Stores procedures called by vector index.
	 */
	std::vector<std::vector <int>> calledByTable;
	int size, calleeCount, callerCount;


public:


	CallsTable();
	/**
	 * If the call relationship between procedure 1 and procedure 2 is not in the Calls Table, inserts it to the Calls Table and returns TRUE
     * Otherwise, returns FALSE and the table remain unchanged
	 */
	bool insertCalls(int proc1, int proc2);

	/**
	 * Return all procedures which calls procedure2.
	 */
	std::vector<int> getCalls(int proc2);

	/**
	 * Return all procedures which calls another procedure.
	 */
	std::vector<int> getCalls();
	/**
	 * Return all procedures called by procedure1.
	 */
  	std::vector<int> getCalledBy(int proc1);

	/**
	 * Return all procedures which are called by another procedure.
	 */
	std::vector<int> getCalledBy();

	/**
	 * Returns TRUE if proc uses var.
     * Otherwise, return FALSE.
	 */
	bool isCalls(int proc1, int proc2); 

	/**
	 * Returns the total number of relationships in Calls Table
	 */
	int getSize();
	
	/**
	 * Remove empty entries in Calls Table. To be invoked after extraction of calls relationship
	 */
	void compressTables();
};

/*
INDEX[] insertCalls (PROC procedure1 , PROC procedure2);
Description:

If the call relationship between procedure 1 and procedure 2 is not in the Parent Table, inserts it to the Calls Table and returns its index.
    Otherwise, returns its INDEX and the table remain unchanged

INTEGER getSize()
Description:
    

INDEX getCallsIndex (STMT statement1 , STMT statement2)
Description:

If the Calls relationship between procedure 1 and procedure 2 is in the Calls Table, returns its index.
    Otherwise, returns -1 (special value)

PAIR<PROC, PROC> extractCalls (INDEX ind)
Description:
    Returns the relationship of two statements at Calls Table [ind]
If “ind” is out of range:
Throws:
    InvalidReferenceException.

PROC[] getCalls (PROC procedure2)
Description:
    Return all procedures calling procedure procedure2.

PROC[] getCalledBy (PROC procedure1)
    Return all procedures called by procedure procedure1.
*/    