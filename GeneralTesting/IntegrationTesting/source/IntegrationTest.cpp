#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>

#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

#ifndef PARSER_HEAD
#define PARSER_HEAD
#include "Parser.h"
#endif

#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

#include "DesignExtractor.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

int main(int argc, char* argv[])
{
	// Get the top level suite from the registry
	CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();
	CppUnit::TextUi::TestRunner runner;

	//parse the SIMPLE source code into the parser
	//Parse();
	char const* simpleFile = "sample_input\\simple_int.txt";
    std::ifstream in(simpleFile, std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open input file: "
            << simpleFile << std::endl;
		int x;
		std::cin >> x;
        return 1;
    }

    std::string storage; // We will read the contents here.
    in.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(storage));

    PKB pkb = PKB();
    Parser temp = Parser();
    temp.parseCode(storage, &pkb);

	//Call of DesignExtractor
	//Extract();
	DesignExtractor de = DesignExtractor(&pkb);
    de.populateTables();

	// check modifies table
	/* std::cout << "Modifies Table" << std::endl;
	for(int i=0;i<pkb.getModifiesTable()->getSize();i++)
	{
		std::cout<< pkb.getModifiesTable()->extractModifies(i).first <<" "<<pkb.getModifiesTable()->extractModifies(i).second<<std::endl;
	} */

	// check size of modifies table
	// 6 vars from line 1-6
	CPPUNIT_ASSERT_EQUAL(6, pkb.getModifiesTable()->getSize());
	

	// check uses table
	/* std::cout << "Uses Table" << std::endl;
	for(int i=0;i<pkb.getUsesTable()->getSize();i++)
	{
		std::cout<< pkb.getUsesTable()->extractUses(i).first <<" "<<pkb.getUsesTable()->extractUses(i).second<<std::endl;
	} */

	// check size of uses table, 11 vars is used from line 1-6
	CPPUNIT_ASSERT_EQUAL(11, pkb.getUsesTable()->getSize());

	// check size of follows table, there should be 5
	CPPUNIT_ASSERT_EQUAL(5, pkb.getFollowsTable()->getSize());
	// check that each statement follows the other
	CPPUNIT_ASSERT(pkb.getFollowsTable()->isFollows(1,2));
	CPPUNIT_ASSERT(pkb.getFollowsTable()->isFollows(2,3));
	CPPUNIT_ASSERT(pkb.getFollowsTable()->isFollows(3,4));
	CPPUNIT_ASSERT(pkb.getFollowsTable()->isFollows(4,5));

	runner.addTest(suite);
	bool wasSuccessful = runner.run();

	// test case 2

	char const* simpleFile2 = "sample_input\\simple_int_2.txt";
    std::ifstream in2(simpleFile2, std::ios_base::in);

    if (!in2)
    {
        std::cerr << "Error: Could not open input file: "
            << simpleFile2 << std::endl;
		int x;
		std::cin >> x;
        return 1;
    }

    std::string myStorage; // We will read the contents here.
    in2.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in2),
        std::istream_iterator<char>(),
        std::back_inserter(myStorage));

    PKB myPkb = PKB();
    Parser myParser = Parser();
    myParser.parseCode(myStorage, &myPkb);

	DesignExtractor myDE = DesignExtractor(&myPkb);
    myDE.populateTables();

	// check size of parent table, should be 7
	CPPUNIT_ASSERT_EQUAL(7, myPkb.getParentTable()->getSize());

	// check each parent relationships
	CPPUNIT_ASSERT(myPkb.getParentTable()->isParent(1,2));
	CPPUNIT_ASSERT(myPkb.getParentTable()->isParent(1,3));
	CPPUNIT_ASSERT(myPkb.getParentTable()->isParent(3,4));
	CPPUNIT_ASSERT(myPkb.getParentTable()->isParent(3,5));
	CPPUNIT_ASSERT(myPkb.getParentTable()->isParent(3,6));
	CPPUNIT_ASSERT(myPkb.getParentTable()->isParent(3,7));
	CPPUNIT_ASSERT(myPkb.getParentTable()->isParent(1,8));

	//getchar();

	return wasSuccessful ? 0 : 1;
}