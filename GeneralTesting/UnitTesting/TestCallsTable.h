#ifndef TestCallsTable_h
#define TestCallsTable_h
#include <cppunit/extensions/HelperMacros.h>
#endif

class CallsTableTest : public CPPUNIT_NS::TestFixture
{ 
    CPPUNIT_TEST_SUITE( CallsTableTest );
    CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testInsertion );
    CPPUNIT_TEST_SUITE_END();

    public:
    void setUp();
    void tearDown();

    // method to test the constructor
    void testConstructor();

	// method to test insertion of procedures
	void testInsertion();
};