#include "stdafx.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int PKBmain(int argc, char* argv[])
{
// Get the top level suite from the registry
CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

CppUnit::TextUi::TestRunner runner;
runner.addTest(suite);
bool wasSuccessful = runner.run();

char* c = (char*) malloc(10 * sizeof (char));
scanf (c,"%c");
return wasSuccessful ? 0 : 1;
}