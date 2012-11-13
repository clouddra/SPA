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

using namespace std;

int main(int argc, char* argv[])
{
	// Get the top level suite from the registry
	CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();
	CppUnit::TextUi::TestRunner runner;

	//parse the SIMPLE source code into the parser
	//Parse();
	char const* simpleFile = "sample_input\\simple00.txt";
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
	std::cout << "Modifies Table" << std::endl;
	for(int i=0;i<pkb.getModifiesTable()->getSize();i++)
	{
		std::cout<< pkb.getModifiesTable()->extractModifies(i).first <<" "<<pkb.getModifiesTable()->extractModifies(i).second<<std::endl;
	}

	// check uses table
	std::cout << "Uses Table" << std::endl;
	for(int i=0;i<pkb.getUsesTable()->getSize();i++)
	{
		std::cout<< pkb.getUsesTable()->extractUses(i).first <<" "<<pkb.getUsesTable()->extractUses(i).second<<std::endl;
	}

	runner.addTest(suite);
	bool wasSuccessful = runner.run();


	//getchar();

	return wasSuccessful ? 0 : 1;
}