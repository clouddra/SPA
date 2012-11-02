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

#ifndef QUERYPROCESSOR_HEAD
#define QUERYPROCESSOR_HEAD
#include "QueryProcessor.h"
#endif

#ifndef PQLPARSER_HEAD
#define PQLPARSER_HEAD
#include "PqlParser.h"
#endif

//using namespace std;


int main() {

	/* Testing code
	DeclarationTable declare;
	declare.insertDeclaration(declare.stmt_, "s2");
	declare.insertDeclaration(declare.assign_, "a2");
	declare.insertDeclaration(declare.variable_, "x");
	declare.insertDeclaration(declare.stmt_, "s2");
	declare.insertDeclaration(declare.stmt_, "s1");
	declare.printDeclarationTable();
    */


    char const* simpleFile = "..\\sample_input\\simple.txt";
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


    char const* pqlFile = "..\\sample_input\\pql.txt";
    std::ifstream in2(pqlFile, std::ios_base::in);

    if (!in2)
    {
        std::cerr << "Error: Could not open input file: "
            << pqlFile << std::endl;
		int x;
		std::cin >> x;
        return 1;
    }

    std::string storage2; // We will read the contents here.
    in2.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in2),
        std::istream_iterator<char>(),
        std::back_inserter(storage2));

    QueryProcessor qp = QueryProcessor();
    PqlParser temp2 = PqlParser();
    std::vector<std::string> queries = temp2.splitQuery(storage2);
    for (int i = 0; i < (int)queries.size(); i++) {
        temp2.parseQuery(queries[i], &qp);
        qp.processQuery(pkb);
        std::cout << "Result of Query " << i+1 << std::endl;
        qp.printResult();
        std::cout << std::endl << std::endl;
        qp = QueryProcessor();
    }

    system("pause");
	return 0;
}