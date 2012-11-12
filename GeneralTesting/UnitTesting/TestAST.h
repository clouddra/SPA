#ifndef TestAST_h
#define TestAST_h
#include <cppunit/extensions/HelperMacros.h>
#endif

#ifndef NODE_HEAD
#define NODE_HEAD
#include "Node.h"
#endif

class ASTTest : public CPPUNIT_NS::TestFixture
{ 
    CPPUNIT_TEST_SUITE( ASTTest );
    CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testInsertion );
    CPPUNIT_TEST_SUITE_END();

    public:
    void setUp();
    void tearDown();

    // method to test the constructor
    void testConstructor();

	// method to test insertion of nodes
	void testInsertion();
};