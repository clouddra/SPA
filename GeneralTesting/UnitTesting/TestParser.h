#ifndef TestParser_h
#define TestParser_h
#include <cppunit/extensions/HelperMacros.h>
#endif

class ParserTest : public CPPUNIT_NS::TestFixture
{ 
    CPPUNIT_TEST_SUITE( ParserTest );
    CPPUNIT_TEST( testConstructor );
    CPPUNIT_TEST_SUITE_END();

    public:
    void setUp();
    void tearDown();

    // method to test the constructor
    void testConstructor();
};