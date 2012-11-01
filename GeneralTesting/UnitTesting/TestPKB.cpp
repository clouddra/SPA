#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "PKB.h"
#include "TestPKB.h"

#include <iostream>
#include <string>


void 
PKBTest::setUp()
{
}


void 
PKBTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PKBTest );

// method to test the constructor
void PKBTest::testConstructor() {  
// create a PKB object
PKB MyPKB();

}