#ifndef TestPKB_h
#define TestPKB_h

#include <cppunit/extensions/HelperMacros.h>

class PKBTest : public CPPUNIT_NS::TestFixture
{ 
CPPUNIT_TEST_SUITE( PKBTest );
CPPUNIT_TEST( testConstructor );
CPPUNIT_TEST_SUITE_END();

public:
void setUp();
void tearDown();

// method to test the constructor
void testConstructor();

};
#endif