#ifndef TestQuery_h
#define TestQuery_h

#include <cppunit/extensions/HelperMacros.h>

class QueryTest : public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( QueryTest );
CPPUNIT_TEST( testConstructor );
CPPUNIT_TEST_SUITE_END();

public:
void setUp();
void tearDown();

// method to test the constructor
void testConstructor();

};
#endif
    