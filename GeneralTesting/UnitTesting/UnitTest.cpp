#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    // Get the top level suite from the registry
    CppUnit::TestSuite *unitSuite = new CppUnit::TestSuite( "All unit test" );
    unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("VarTableTest").makeTest());	
    unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ParserTest").makeTest());
    unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("QueryPreprocessorTest").makeTest());
	unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("CallsTableTest").makeTest());
	unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("FollowsTableTest").makeTest());
	// unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ProcTableTest").makeTest());	
    CppUnit::TestFactoryRegistry::getRegistry().addTestToSuite(unitSuite);
    CppUnit::TextUi::TestRunner runner;


    runner.addTest(unitSuite);
    bool wasSuccessful = runner.run();

    //getchar();

    return wasSuccessful ? 0 : 1;
}