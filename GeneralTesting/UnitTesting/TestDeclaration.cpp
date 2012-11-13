#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "DeclarationTable.h"
#include "TestDeclaration.h"

#include <iostream>
#include <string>


void 
DeclarationTest::setUp()
{
}


void 
DeclarationTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( DeclarationTest );

// method to test the constructor
void DeclarationTest::testConstructor() { 
// create an AST object
DeclarationTable declare;

}

void DeclarationTest::testInsertion(){
	// create a declaration table
	DeclarationTable declare;

	declare.insertDeclaration(declare.stmt_, "s2");
	declare.insertDeclaration(declare.assign_, "a2");
	declare.insertDeclaration(declare.variable_, "x");
	declare.insertDeclaration(declare.stmt_, "s2"); // inserting an already present value
	declare.insertDeclaration(declare.stmt_, "s1");

	// check size of table
	CPPUNIT_ASSERT_EQUAL(4, declare.getSize());

	// check if everything is declared
	CPPUNIT_ASSERT(declare.isDeclared(declare.stmt_, "s1"));
	CPPUNIT_ASSERT(declare.isDeclared(declare.stmt_, "s2"));
	CPPUNIT_ASSERT(declare.isDeclared(declare.assign_, "a2"));
	CPPUNIT_ASSERT(declare.isDeclared(declare.variable_, "x"));

	// check the type of every declaration
	CPPUNIT_ASSERT_EQUAL(0, declare.getType("s1"));
	CPPUNIT_ASSERT_EQUAL(0, declare.getType("s2"));
	CPPUNIT_ASSERT_EQUAL(1, declare.getType("a2"));
	CPPUNIT_ASSERT_EQUAL(4, declare.getType("x"));
}