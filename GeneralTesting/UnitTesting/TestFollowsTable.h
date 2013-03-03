#ifndef TestFollowsTable_h
#define TestFollowsTable_h
#include <cppunit/extensions/HelperMacros.h>
#endif

class FollowsTableTest : public CPPUNIT_NS::TestFixture
{ 
    CPPUNIT_TEST_SUITE( FollowsTableTest );
    CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testInsertion );
    CPPUNIT_TEST_SUITE_END();

    public:
    void setUp();
    void tearDown();

    // method to test the constructor
    void testConstructor();

	// method to test insertions
	void testInsertion();
};