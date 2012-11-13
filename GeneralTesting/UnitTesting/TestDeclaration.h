#ifndef TestDeclaration_h
#define TestDeclaration_h
#include <cppunit/extensions/HelperMacros.h>
#endif

class DeclarationTest : public CPPUNIT_NS::TestFixture
{ 
    CPPUNIT_TEST_SUITE( DeclarationTest );
    CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testInsertion );
    CPPUNIT_TEST_SUITE_END();

    public:
    void setUp();
    void tearDown();

    // method to test the constructor
    void testConstructor();

	// method to test insertion of declarations
	void testInsertion();
};