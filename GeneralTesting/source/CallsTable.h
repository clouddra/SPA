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
	int size;

public:


	CallsTable();
	/**
	 * If the call relationship between procedure 1 and procedure 2 is not in the Calls Table, inserts it to the Calls Table and returns TRUE
     * Otherwise, returns FALSE and the table remain unchanged
	 */
	bool insertCalls(int proc1, int proc2);

	/**
	 * Return all procedures calling procedure2.
	 */
	std::vector<int> getCalls(int proc2);

	/**
	 * Return all procedures called by procedure1.
	 */
  	std::vector<int> getCalledBy(int proc1);

	/**
	 * Returns TRUE if proc uses var.
     * Otherwise, return FALSE.
	 */
	bool isCalls(int proc1, int proc2); 

	/**
	 * Returns the total number of relationships in Calls Table
	 */
	int getSize();
};

/*int size;
	CallsTable test ;
	vector<int> result, result2 ;
	//test.insertCalls(100,2);
	bool val = test.insertCalls(1,2);
	val =	test.insertCalls(1,2);
	val =  test.insertCalls(1,9);
	val = test. insertCalls(1,50);
	val = false ;
	result = test.getCalls(2);
	result2 = test.getCalledBy(1) ;
	val = test.isCalls(1,50);
	val = test.isCalls(1,5); 
	size = test.getSize();
*/    