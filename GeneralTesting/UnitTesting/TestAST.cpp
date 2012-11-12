#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "AST.h"
#include "TestAST.h"

#include <iostream>
#include <string>


void 
ASTTest::setUp()
{
}


void 
ASTTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ASTTest );

// method to test the constructor
void ASTTest::testConstructor() { 
// create an AST object
AST myAST = AST();

}

void ASTTest::testInsertion(){
// create an AST
AST myAST = AST();

// insert a node with type assign (3) from statement number 1
CPPUNIT_ASSERT_EQUAL(0 , myAST.insertNode(3,2,1,-1));

// insert a node with type assign (3) from statement number 2
CPPUNIT_ASSERT_EQUAL(1 , myAST.insertNode(3,3,2, 1));

// verify that insertions are correct

CPPUNIT_ASSERT_EQUAL(3, myAST.getNode(0).getNodeType());
CPPUNIT_ASSERT_EQUAL(2, myAST.getNode(0).getValue());
CPPUNIT_ASSERT_EQUAL(1, myAST.getNode(0).getStmtNum());

CPPUNIT_ASSERT_EQUAL(3, myAST.getNode(1).getNodeType());
CPPUNIT_ASSERT_EQUAL(3, myAST.getNode(1).getValue());
CPPUNIT_ASSERT_EQUAL(2, myAST.getNode(1).getStmtNum());

// attempt to retrieve a node that does not exist (out of range) - not working
// CPPUNIT_ASSERT_EQUAL(-1, myAST.getNode(2).getValue());
}