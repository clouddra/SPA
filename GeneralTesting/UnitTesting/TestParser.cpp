#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "TestParser.h"

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

#include <iostream>
#include <string>


void 
ParserTest::setUp()
{
}


void 
ParserTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest );

// method to test the constructor
void ParserTest::testConstructor() { 
	Parser myParser = Parser();

}