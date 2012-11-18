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

#include "DesignExtractor.h"

/*
int main() {
    char const* simpleFile = "..\\sample_input\\simple00.txt";
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
	DesignExtractor de = DesignExtractor(&pkb);
    de.populateTables();

	for(int i=0;i<pkb.getUsesTable()->getSize();i++)
	{
		std::cout<< pkb.getUsesTable()->extractUses(i).first <<" "<<pkb.getUsesTable()->extractUses(i).second<<std::endl;
	}
    // pql.txt stores currently working queries, pqlShort.txt stores queries in development (may not work)
    char const* pqlFile = "..\\sample_input\\pqlShort.txt";
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
        int ret = temp2.parseQuery(queries[i], &qp);
        if (ret == 0) {
            qp.processQuery(pkb);
            std::cout << "Result of Query " << i+1 << std::endl;
            qp.printResult();
            std::cout << std::endl << std::endl;
        }
        qp = QueryProcessor();  // Reset qp to empty for next query
        qp.getResult();
    }

    system("pause");
	return 0;
}
*/