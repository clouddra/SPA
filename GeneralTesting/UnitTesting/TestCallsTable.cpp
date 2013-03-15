#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "CallsTable.h"
#include "TestCallsTable.h"

#include <iostream>
#include <string>
#include <vector>

void 
CallsTableTest::setUp()
{
}


void 
CallsTableTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CallsTableTest );

// method to test the constructor
void CallsTableTest::testConstructor() { 
	// create a Calls Table
	CallsTable myCallsTable = CallsTable();

}

// method to test insertions
void CallsTableTest::testInsertion() {
	// create a Calls Table
	CallsTable myCallsTable = CallsTable();

	// successful insertions
	CPPUNIT_ASSERT(myCallsTable.insertCalls(1,2));
	CPPUNIT_ASSERT(myCallsTable.insertCalls(4,2));
	CPPUNIT_ASSERT(myCallsTable.insertCalls(1,5));
	
	// check for recursion, should return false - not handled by parser
	// CPPUNIT_ASSERT(!(myCallsTable.insertCalls(3,3)));

	// check if two procedures are calling each other - fail
	// CPPUNIT_ASSERT(!(myCallsTable.insertCalls(2,1)));

	// check getCalls
	CPPUNIT_ASSERT_EQUAL(1, myCallsTable.getCalls(2)[0]);
	CPPUNIT_ASSERT_EQUAL(4, myCallsTable.getCalls(2)[1]);

	// check getCalledBy
	CPPUNIT_ASSERT_EQUAL(2, myCallsTable.getCalledBy(1)[0]);
	CPPUNIT_ASSERT_EQUAL(5, myCallsTable.getCalledBy(1)[1]);

	// check isCalls
	CPPUNIT_ASSERT(myCallsTable.isCalls(1,2));
	CPPUNIT_ASSERT(!(myCallsTable.isCalls(2,3)));

	// check size
	CPPUNIT_ASSERT_EQUAL(3, myCallsTable.getSize());
}