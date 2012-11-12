#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "TestQuery.h"

#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

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
QueryProcessor myQuery = QueryProcessor();

}