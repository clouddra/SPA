#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "QueryProcessor.h"
#include "TestQuery.h"

#include <iostream>
#include <string>


void 
QueryTest::setUp()
{
}


void 
QueryTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryTest );

// method to test the constructor
void QueryTest::testConstructor() {  
// create a query object
QueryProcessor MyQuery();

}