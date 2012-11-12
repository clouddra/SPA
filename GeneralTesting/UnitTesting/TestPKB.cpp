#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "TestPKB.h"

#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

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
PKB myPKB = PKB();

}