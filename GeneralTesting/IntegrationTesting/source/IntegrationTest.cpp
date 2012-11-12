#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>


using namespace std;

int main(int argc, char* argv[])
{
// Get the top level suite from the registry
CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();
CppUnit::TextUi::TestRunner runner;

//parse the SIMPLE source code into the parser
//Parse();

//Call of DesignExtractor
//Extract();

runner.addTest(suite);
bool wasSuccessful = runner.run();


//getchar();

return wasSuccessful ? 0 : 1;
}